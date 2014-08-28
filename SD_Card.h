/********************************************************************
Fichier      : SD_Card
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 19 août 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Librairie de fonctionnement de la Carte SD
--------------------------Modifications------------------------------
Version      : 1.0
Modification : Version initiale
*********************************************************************/

#include "SPI_function.h"
#include "General_func.h"
#include "FSconfig.h"

#ifndef SD_CARD_H
#define	SD_CARD_H

//Typedefs
typedef unsigned LBA;

//Commandes pour carte SD
#define RESET           0   //a.k.a GO_IDLE (CMD0)
#define INIT            1   //a.k.a SEND_OP_CMD (CMD1)
#define READ_SINGLE     17
#define WRITE_SINGLE    24

//definitions de macros
#define clockSPI()  writeSPI2(0xFF);
#define readSPI()   writeSPI2(0xFF);
#define disableSD() SD_CS = 1; clockSPI()
#define enableSD()  SD_CS = 0

//Définitions HardWare
#define SD_CS PORTGbits.RG15
#define SD_IO TRISGbits.TRISG15

//Définitions d'erreur
#define E_COMMAND_ACK   0x80
#define E_INIT_TIMEOUT  0x81
#define FAIL FALSE

//definitions pour les fonctions
#define I_TIMEOUT       100000 //100 ms
#define R_TIMEOUT       1000   //1   ms
#define W_TIMEOUT       100000 //100 ms
//Définitions SD
#define DATA_START  0xFE
#define DATA_ACCEPT 0x05

//Prototypes de fonctions
void InitSD(void);
int sendSDCmd (unsigned char cmd,unsigned address);
int initMedia(void);
int readSECTOR (LBA ,char *);
int writeSECTOR (LBA ,char *);

//Fonctions
void InitSD (void)
{
    SD_IO = 0;
    SD_CS = 1;

    //config SPI2
    SPI2CONbits.ON		= 0;	// disable SPI port
    SPI2CONbits.SIDL 	        = 0; 	// Continue module operation in Idle mode
    SPI2BUF                     = 0;   	// clear SPI buffer
    SPI2CONbits.MODE16          = 0;    //Mode 16 bits inactif
    SPI2CONbits.MODE32          = 0;    //Mode 32 bits inactif

    SPI2CONbits.DISSDO		= 0;	// SDOx pin is controlled by the module
    SPI2CONbits.MODE16          = 0;	// set in 8-bit mode, clear in 16-bit mode
    SPI2CONbits.SMP		= 0;	// Input data sampled at middle of data output time,dans ce cas middle of time
    SPI2CONbits.CKP 		= 0;	// On indique que IDLE est 0 et ACTIVE est 1
    SPI2CONbits.CKE 		= 1;	// Le changement de la donnée se fait au flanc descdant
    SPI2CONbits.MSTEN 		= 1; 	// 1 =  Master mode; 0 =  Slave mode

    SPI2CONbits.FRMEN		= 0;	// non-framed mode

    SPI2BRG = 71;                        //Il s'agit du diviseur de fréquence Fsck = Fpb / 2* (SPIxBRG+1)

    SPI2CONbits.ON 		= 1; 	// enable SPI port, clear status
}

int sendSDCmd (unsigned char cmd,unsigned address)
{
    //Variables locales
    char i;
    int reponse;

    writeSPI2(cmd | 0x40);//Envoi de la commande

    writeSPI2(address>>24);//MSB de l'adresse
    writeSPI2(address>>16);
    writeSPI2(address>>8);
    writeSPI2(address);//LSB de l'adresse

    writeSPI2( 0x95);   // send CMD0 CRC

    // now wait for a response, allow for up to 8 bytes delay
    for( i=0; i<8; i++)
    {
      reponse=writeSPI2(0xFF);
      if ( reponse != 0xFF) break;
    }
    return reponse;


    //la carte SD est toujours active !
}

//Fonction d'initialisation de la carte SD
int initMedia(void)
{
    int i,reponse;

    //La carte SD n'est pas sélectionné
    disableSD();

    //On envoit 80 clocks avant de sélectionner la carte Sd
    for (i=0;i<10;i++)
    {
      clockSPI();
    }

    //On sélectionne la carte
    enableSD();

    //Envoit de la commande RESET à la carte SD
    reponse = sendSDCmd(RESET,0);
    disableSD();
    //On retourne au mode IDLE
    if(reponse != 1)
    {
        return E_COMMAND_ACK;//On indique que la commande à été rejetée
    }

    //On envoie la commande INIT jusqu'à ce que IDLE se termine
    for (i=0; i<I_TIMEOUT; i++)
    {
      reponse = sendSDCmd( INIT, 0);
      disableSD();
      if ( !reponse) break;
    }
    if ( i == I_TIMEOUT)
    {
      return E_INIT_TIMEOUT; //Init timed out
    }

    //6.On reconfigure le SPI à Haute Vitesse
    SPI2CONbits.ON  = 0;
    SPI2BRG         = 15;
    SPI2CONbits.ON  = 1;
    
    return 0;//Fin de l'initialisation
}

//Fonction pour lire un secteur de la carte SD
int readSECTOR (LBA address,char *pointer)
{
    //Pointeur sur le buffer du secteur
    //LBA du secteur requis
    int reponse,i;

    //1. send READ command
    reponse = sendSDCmd(READ_SINGLE,(address<<9));

    //Si la commande à été acceptée
    if(reponse == 0)
    {
      //Attente d'une réponse
      for( i=0; i<R_TIMEOUT; i++)
      {
        reponse = readSPI();
        if ( reponse == DATA_START)
        break;
      }

      //Si il n'y a pas eu de Timeout on effectue la lecture
      if(i != R_TIMEOUT)
      {
          i = 512; //Pour la lecture des 512 bytes (du secteur en fait)
          do
          {
            *pointer++= readSPI();
          }while(--i>0);

      //On ignore le CRC
       readSPI();
       readSPI();
      }

      //On désactive la carte SD
      disableSD();

      return (reponse == DATA_START);//Si la transmission à été faite correctement on retourne TRUE

    }

}

//Fonction pour écrire dans un secteur de la carte SD 
int writeSECTOR (LBA address, char* pointer)
{   
    unsigned i,reponse;
    //On envoie la commande d'écriture
    reponse = sendSDCmd(WRITE_SINGLE,(address << 9));
    //On vérifie si la commande est acceptée
    if (reponse == 0)
    {
        //On envoie les données 
        writeSPI2(DATA_START);
        
        //On envoie ensuite les 512 bytes;
        for(i = 0;i<512;i++)
        {
            writeSPI2(*pointer++);
        }
        
        //On envoie un CRC bidon
        clockSPI();
        clockSPI();
   
        //On vérifie si les données sont acceptées
        reponse = readSPI();
   
        if((reponse & 0xF) == DATA_ACCEPT)
        {
            for(i = 0;i<W_TIMEOUT;i++)
            {
              reponse = readSPI();
              
              if(reponse != 0)//Si accepté on quitte l'attente
              {
                  break;
              }
            }
        }
        else
        {
            //Il n'y a pas eu d'acceptation
            reponse = FAIL;
        }  
    }
    
    //On désactive la carte SD
    disableSD();
    //On renvoit true si tout c'est bien passé
    return reponse;
}



#endif	/* SD_CARD_H */