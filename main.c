/********************************************************************
Fichier      : main.c
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 20.03.2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Fonctionnement du capteur de température.
--------------------------Modifications------------------------------
Version      : 1.8 - 13.08.2014
Modification : Fonctionnement de l'écriture de flottant sur la mémoire
*********************************************************************/


#include "General_func.h"

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <p32xxxx.h>
#include <conio.h>
#include <proc/p32mx795f512l.h>
#include <xc.h>


#include "KTY_81_110.h"
#include "SPI_function.h"
#include "TL2350R.h"
#include "Bluetooth_module.h"
#include "RTCC.h"
#include "User_Gestion.h"
#include "Timer1.h"
#include "Gestion_Chauffage.h"
#include "Gestion_Porte.h"
#include "Notes_Gestion.h"

//Definitions
#define UART
#define BUFFER_SIZE 128
#define DEBUG

#define CHECKUSER 0
#define LED4ON 1
#define LED4OFF 2
#define LED5ON 3
#define LED5OFF 4
#define CHECKPINCODE 5
#define SETPINCODE 6
#define ADDNOTE 7
#define DELETENOTE 8
#define READALLNOTE 9
#define READPERSONALNOTE 10
#define READPUBLICNOTE 11
#define CHECKDOORPASSWORD 81
#define READTEMP 82
#define WRITEBYTE 20
#define READBYTE 21
#define FLOATWRITE 22
#define FLOATREAD 23
#define STRINGWRITE 24
#define STRINGREAD 25
#define CHIPERASE 26
#define GETTIME 40
#define GETUSERS 51
#define READLOGADRESS 52
#define ADDUSERS 53
#define DELETEUSER 54
#define MODIFYPASSWORD 55
#define CHECKLOGIN 56
#define MODIFYUSERNAME 57
#define LISTINIT 70
#define INITNOTE 90
#define INITCHAUFFAGE 100
#define SETCHAUFFAGETEMP 101
#define GETCHAUFFAGEHISTORY 102
#define DISCONNECT 230

//Prototypes de fonctions
void WriteByte  (void);
void ReadByte   (void);
void FloatWrite (void);
void FloatRead  (void);
void StringWrite(void);
void StringRead (void);
void GetTime    (void);
void GetLight   (void);

/*Configurations Bits*/
// PIC32MX795F512L Configuration Bit Settings

// DEVCFG3
// USERID = No Setting
#pragma config FSRSSEL  = PRIORITY_0     // SRS Select (SRS Priority 0)
#pragma config FMIIEN   = OFF             // Ethernet RMII/MII Enable (RMII Enabled)
#pragma config FETHIO   = OFF             // Ethernet I/O Pin Select (Alternate Ethernet I/O)
#pragma config FCANIO   = OFF             // CAN I/O Pin Select (Alternate CAN I/O)
#pragma config FUSBIDIO = OFF           // USB USID Selection (Controlled by Port Function)
#pragma config FVBUSONIO= OFF          // USB VBUS ON Selection (Controlled by Port Function)
// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL  = MUL_18         // PLL Multiplier (18x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN   = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
// DEVCFG1
#pragma config FNOSC    = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN  = ON             // Secondary Oscillator Enable (Disabled)
#pragma config IESO     = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD  = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV   = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM    = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS    = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN   = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
// DEVCFG0
#pragma config DEBUG    = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL   = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP      = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP      = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP       = OFF                 // Code Protect (Protection Disabled)



//Variables globales externes
extern char buffer     [BUFFER_SIZE];
extern char user       [BUFFER_SIZE];
extern char password   [BUFFER_SIZE];
extern char function   [BUFFER_SIZE];
extern char done;
extern char cnt_debug;
extern int  CMD;
extern char buffer_s   [BUFFER_SIZE];


extern unsigned char isLogged;

//Variables globales
union myadress var;
int count  = 1;
int count2 = 1;
int count3 = 1;
char buffer2           [BUFFER_SIZE];

//Définitions Hardware
#define LED5 PORTCbits.RC1
#define LED4 PORTAbits.RA3



void Init_module (void)
{
  //Fonction d'initialisation
  InitADC_R();
  initUART3A();
  SPI2Init();
  RTCCInit();
  TimerInit();
  InitPWM();
  isLogged = 0;
  //Configuration des interrupts

  // Configure UART RX Interrupt
  INTEnable(INT_SOURCE_UART_RX(UART_MODULE_ID),INT_ENABLED);
  INTSetVectorPriority(INT_VECTOR_UART(UART_MODULE_ID),INT_PRIORITY_LEVEL_4);


  //Configure Timer 1 Interrupt
  ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

  //Configure Timer 45 Interrupt
  ConfigIntTimer45(T45_INT_ON | T45_INT_PRIOR_3);

  //configure interrupt capteur de mouvement.
  mINT0SetIntPriority(1); //On set la priorité de l'interrupt du capteur de mouvement.
  mINT0IntEnable(0); //On s'assure que l'interrupt du capteur de mouvement est désactivée au démarrage.
  INTCONbits.INT0EP = 1; // L'interrupt du capteur de mouvement s'activera lors des fronts montants uniquement.

  // Enable multi-vector interrupts
  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
  INTEnableInterrupts();



  //Initialisaiton des buffers
  CMD = 5555;
  sprintf(buffer,"");
  sprintf(user,"");
  sprintf(password,"");
  sprintf(function,"");
  sprintf(buffer_s,"");

  done = 0;
}

int main(int argc, char** argv)
{
     
    Init_module();


    char p_subject [9];
    char p_user[9];
    char p_date[11];
    
    float value;

    
    /*Configuration des entrées sorties*/
    PORTSetPinsDigitalOut(IOPORT_C,BIT_1);
    PORTSetPinsDigitalOut(IOPORT_A,BIT_3);
    PORTSetPinsDigitalOut(IOPORT_E,BIT_0|BIT_1|BIT_2|BIT_3); // On indique que les bits 0 à 3 du port E sont des sorties
    PORTSetPinsDigitalIn(IOPORT_E,BIT_4|BIT_5|BIT_6|BIT_7); // On idique que  les bits 4 à 7  du port E sont des entrées

    PORTSetPinsDigitalOut(IOPORT_G,BIT_6|BIT_8|BIT_9); // On indique que les bits 6,8 et 9du port E sont des sorties
    PORTSetPinsDigitalIn(IOPORT_G,BIT_7); // On indique que le bit 7  du port G sont des sorties
    /**/


#ifdef UART
    
    LED5 = 0;
    LED4 = 0;
    putsU3("Hello it's the Smarthome system\n");

    
    while(1)
    {     
           
      if (done == 1)
      {
     
      #ifdef DEBUG
          sprintf(buffer,"CMD : %d PARAM : %s\n",CMD,function);
          putsU3(buffer);
          isLogged = 1;
      #endif

      }

      #ifndef DEBUG
      if(done==1 && isLogged == 0)
      {
        switch (CMD)
        {
            case 0:
            {
              //test du rajout d'utilisateur
              sscanf(function,"%s,%s",buffer2,password);
              //Ecriture du compte en mémoire
              AddUser(buffer2,password);
              break;
            }

            case 1:
            {
                sscanf(function,"%s",password);
                CheckLogin(user,password);
                done = 0;
                break;
            }
        }
      }    
      #endif



    if(done==1 && isLogged == 1){
        switch (CMD)
        {
            case CHECKUSER:
            {
                CheckLogin(user,password);
                break;
            }

            case LED4ON:
            {
              LED4 = 1;
              putsU3("La led 4 est allumée\n\r");
              break;
            }

            case LED4OFF:
            {
              LED4 = 0;
              putsU3("La led 4 est éteinte\n\r");
              break;
            }

            case LED5ON:
            {
              LED5 = 1;
              putsU3("La led 5 est allumée\n\r");
              break;
            }

            case LED5OFF:
            {
              LED5 = 0;
              putsU3("La led 5 est éteinte\n\r");
              break;
            }

            
            case CHECKPINCODE:
            {
              int temp;
              //Command to check if the user have send the right pin code
              sscanf(function,"%d",&temp);
              OpenDoor(temp);
              break;
            }

            case SETPINCODE:
            {
              int temp;
              //Pour set le code pin de la porte
              sscanf(function,"%d",&temp);
              SetDoorCode(temp);
              break;
            }

            case ADDNOTE:
            {
                //Fonction d'ajout d'une note
                sscanf(function,"%s,%s,%s,%s",p_subject,buffer2,p_user,p_date);
                AddNote(p_subject,buffer2,p_user,p_date);
                break;
            }

            case DELETENOTE:
            {
                //Fonction de suppresion d'une note
                sscanf(function,"%s,%s",p_subject,p_user);
                DeleteNote(p_subject,p_user);
                break;
            }

            case READALLNOTE:
            {
                //Fonction de lecture d'une note
                sscanf(function,"%s,%s",p_subject,p_user);
                ReadNoteAll();
                break;
            }

            case READPERSONALNOTE:
            {
              //Fonction de lecture d'une note
              sscanf(function,"%s",p_user);
              ReadPersonnalNotes(p_user);
              break;
            }


            case READPUBLICNOTE:
            {
              ReadPublicNotes();
              break;
            }


            case CHECKDOORPASSWORD:
            {
              int temp;
              //Command to check if the user have send the right pin code
              sscanf(function,"%d",&temp);
              CheckDoorPassword(temp);
              break;
            }

            case READTEMP:
            {
              float temp = read_kty_81_220(0); //0 pour température intérieure.
              sprintf(buffer,"la température actuelle est de %f °C",temp);
              putsU3(buffer);
              break;
            }

            case WRITEBYTE:
            {
              WriteByte();
              break;
            }

            case READBYTE:
            {
              ReadByte();
              break;
            }

            case FLOATWRITE:
            {
              FloatWrite();
              break;
            }

            case FLOATREAD:
            {
              FloatRead();
              break;
            }

            case STRINGWRITE:
            {
              StringWrite();
              break;
            }

            case STRINGREAD:
            {
              StringRead();
              break;
            }

            case CHIPERASE:
            {
              CE_cmd();
              putsU3("CHIP ERASED");
              break;
            }

            case GETTIME:
            {
              GetTime();
              break;
            }

            case 50:
            {
              break;
            }

            case GETUSERS:
            {
              GetUsers();
              break;
            }
    
            case READLOGADRESS:
            {
              Read_log_adress ();
              break;
            }

            case ADDUSERS:
            {
              //test du rajout d'utilisateur
              sscanf(function,"%s,%s",buffer2,password);
              //Ecriture du compte en mémoire
              AddUser(buffer2,password);
              break;
            }

            case DELETEUSER:
            {
                //test du rajout d'utilisateur
                sscanf(function,"%s",buffer2);
                //Ecriture du compte en mémoire
                DeleteUser(buffer2);
                break;
            }

            case MODIFYPASSWORD:
            {
                //Test de la modification du mot de passe.
                sscanf(function,"%s,%s",buffer2,buffer_s);
                //Modification du mot de passe.
                ModifyPassWord(buffer2,buffer_s);
                break;
            }

            case CHECKLOGIN:
            {
                //test de la vérification d'un USER
                sscanf(function,"%s,%s",buffer2,buffer_s);
                //Ecriture du compte en mémoire
                CheckLogin(buffer2,buffer_s);
                break;
            }


            case MODIFYUSERNAME:
            {
                //Modification du username
                sscanf(function,"%s,%s",buffer2,buffer_s);
                //Modification du Username
                ModifiyUsername(buffer2,buffer_s);
                break;
            }

            case LISTINIT:
            {
                ListInit();
                break;
            }


            case INITNOTE:
            {
                //Initialisation
                InitNote();
                break;
            }
            case INITCHAUFFAGE:
            {
                Init_Gestion_Chauffage();
               
            }
            case SETCHAUFFAGETEMP:
            {
                gestion_chauffage(22); //La température demandée est une constante pour l'instant.
            }
            case GETCHAUFFAGEHISTORY:
            {
                Get_Historique();
            }

            case DISCONNECT:
            {
                SetCommandMode();
                Disconnect();
                QuitCommandMode();
                break;
            }

            default:
            {

            }

        }
        done=0;
  }







  
    }
#endif
#ifdef SPI

        SPI2Init();
        int i=0;
        union myadress test ;
        while(1)
        {

            for (i=0; i<0xfffff; i++);

            
            
            test.adress = 0x300;
            WRITE_cmd(test.nb,32);
            test.adress += 9;
            WRITE_cmd(test.nb,32);

        }

        SpiChnClose(2);
#endif
#ifdef TMP
    
#endif
            

           
 }

// UART 2 interrupt handler, set at priority level 4
void __ISR(_UART2_VECTOR, ipl4) IntUart2Handler(void)
{
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART3A)))
        {
           INTClearFlag(INT_SOURCE_UART_RX(UART3A)); // Clear the RX interrupt Flag
           INTDisableInterrupts(); //On désactive les interrupts le temps de l'acquisiton
            
            memset(buffer,0,128);   //On vide le buffer
            memset(function,0,128); //On remet la chaine des paramètres à zéro
            memset(buffer_s,0,128); //On remet le buffer à zéro.
            memset(user,0,128);     //ON remet le user à zéro.
            memset(password,0,128);     //ON remet le user à zéro.
            CMD = 8012;             //On remet la variable par défaut

            if(U2STAbits.FERR == 0 && U2STAbits.PERR == 0 )
            {
              putU3('K');
              getsU3(buffer,128);   //On récupère le message
              done=1;               //On indique qu'une transmission c'est faite
            }
            INTEnableInterrupts();  //On réactive les interrupts
            INTClearFlag(INT_SOURCE_UART_RX(UART3A)); // Clear the RX interrupt Flag
  
            sscanf(buffer,"%s,%d,%s",user,&CMD,function);
            //printf("user : %s \n cmd : %d \n function : %s \n",user,CMD,function);
	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART_MODULE_ID)) )
	{
            INTClearFlag(INT_SOURCE_UART_TX(UART_MODULE_ID));
	}
}


//TIMER 1 Interrupt Handler,set at priotity level 2
void __ISR (_TIMER_1_VECTOR,ipl2) IntTimer1Handler(void)
{
    if(count>643)// test effectué 643 fois -> 643 fois sec = 300 sec
        //2.145 fois permet d'avoir une seconde.
    {

        count=1;    // reset du compteur
        mT1ClearIntFlag();
    }
    else
    {
        count ++;
        mT1ClearIntFlag();
    }

}


// Interruption appelée par le capeur de mouvement. (Pin 3)
//_EXTERNAL_0_VECTOR est le vecteur pour le capteur de mouvement
// ipl7 est le niveau de priorité le plus haut: le premier à être servi.
/*void __ISR(_TIMER_45_VECTOR,ipl2) IntTimer45Handler(void)
{
    if(count2 > 45)
    {
        mINT0IntEnable(1); //On active l'interrupt du capteur de mouvement.
        mT45IntEnable(0); //On désactive l'interrupt du timer 45.
        mT45SetIntPriority(3);
        mT45IntEnable(1);
    }
    else
    {
        count2++;
    }
    mT45ClearIntFlag();
}*/

//Timer 45 Interrupt Handler,set at priority level 3
void __ISR(_TIMER_45_VECTOR,ipl3) IntTimer45Handler(void)
{
    if(count3>10)
    {
        Add_temperature();
        count3 = 1;
    }
    else
    {
        count3++;
    }
    mT45ClearIntFlag();
}
