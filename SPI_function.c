#include "SPI_function.h"
#include "Bluetooth_Module.h"

//Fichier source des commandes SPI pour l'EEPROM 25LC1024

#define READ  0b00000011  //3
#define WRITE 0b00000010  //2
#define WREN  0b00000110  //6
#define WRDI  0b00000100  //4
#define RDSR  0b00000101  //5
#define WRSR  0b00000001  //1
#define PE    0b01000010  //
#define SE    0b11011000  //
#define CE    0b11000111  //
#define RDID  0b10101011  //
#define DPD   0b10111001  //


/*Code Source*/
/*Fonction de configuration du bus SPI sur le PIC*/
void SetSPICONFIG(int chn,int isMaster,int taille_byte)
{
   TRISE = 0x00;
   SpiOpenFlags config = SPI_OPEN_SMP_END|SPI_CONFIG_MODE8|SPI_CON_ON;

   switch (taille_byte)
   {
       case 1:
       {
         config=config|SPI_CONFIG_MODE8;
         break;
       }

       case 2:
       {
           config=config|SPI_CONFIG_MODE16;
           break;
       }
       case 4:
       {
           config=config|SPI_CONFIG_MODE32;
           break;
       }

       if (isMaster==1)
       {
         config=config|SPI_CONFIG_MSTEN;
       }

       SpiChnOpen(chn,config,16);//Division de la fréquence par 16

   }
}

void SPI2Init(void)
{
    TRISCbits.TRISC3 = 0;

    // On indique que les bits 0 à 3 du port E sont des sorties
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;

    //config SPI2
    SPI2CONbits.ON		= 0;	// disable SPI port
    SPI2CONbits.SIDL 	        = 0; 	// Continue module operation in Idle mode
    SPI2BUF                     = 0;   	// clear SPI buffer
    SPI2CONbits.MODE16          = 0;    //Mode 16 bits inactif
    SPI2CONbits.MODE32          = 0;    //Mode 32 bits inactif

    SPI2CONbits.DISSDO		= 0;	// SDOx pin is controlled by the module
    SPI2CONbits.MODE16          = 0;	// set in 8-bit mode, clear in 16-bit mode
    SPI2CONbits.SMP		= 1;	// Input data sampled at middle of data output time,dans notre cas end of time
    SPI2CONbits.CKP 		= 0;	// On indique que IDLE est 0 et ACTIVE est 1
    SPI2CONbits.CKE 		= 1;	// Le changement de la donnée se fait au flanc descdant
    SPI2CONbits.MSTEN 		= 1; 	// 1 =  Master mode; 0 =  Slave mode

    SPI2CONbits.FRMEN		= 0;	// non-framed mode

        SPI2BRG = 16;                        //Il s'agit du diviseur de fréquence Fsck = Fpb / 2* (SPIxBRG+1)

    SPI2CONbits.ON 		= 1; 	// enable SPI port, clear status

}

void SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster)
{
    TRISE = 0x00;
    // 16 bits/char, input data sampled at end of data output time
    SpiOpenFlags oFlags=SPI_OPEN_MODE8|SPI_OPEN_SMP_END|SPI_OPEN_ON;

    if(isMaster)
    {
	oFlags|=SPI_OPEN_MSTEN; // Set as Master mode in this example
    }

    if(frmEn)
    {
	oFlags|=SPI_OPEN_FRMEN;

        if(!frmMaster)
	{
            oFlags|=SPI_OPEN_FSP_IN;
	}
    }
    // Open SPI module, use SPI channel 1, use flags set above, Divide Fpb by 4
    SpiChnOpen(chn, oFlags, 8);
}



//Fonctions de commande de l'EEPROM

unsigned char writeSPI2( unsigned char data )
{
    SPI2BUF = data;                 // write to buffer for TX
    while(!SPI2STATbits.SPIRBF);    // wait for transfer to complete
    return SPI2BUF;                 // read the received value
}//writeSPI2

void WREN_cmd ()
{
    WP = 1;
    HOLD = 1;

    CS = 0;
    writeSPI2(WREN);
    CS = 1;
}

void WRDI_cmd(void)
{
    CS = 0;
    writeSPI2(WRDI);
    CS = 1;
}

unsigned char RDSR_cmd(void)
{
    unsigned char reg;
    CS = 0;
    writeSPI2(RDSR);
    reg = writeSPI2(0);
   // printf("REGISTER : %d\n",reg);
    CS=1;
    return reg;
}

void WRSR_cmd (int BP1,int BP0)
{
    char config = 0;
    CS = 0;

    if(BP0==1) config|0x01;
    if(BP1==1) config|0x02;

    writeSPI2(WRSR);
    writeSPI2(config);
    CS = 1;
}

void PE_cmd (unsigned char addr_tab[])
{
    WREN_cmd();
    CS = 0;
    writeSPI2(PE);
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);
    CS = 1;
}

void SE_cmd (unsigned char addr_tab[])
{
    WREN_cmd();
    CS = 0;
    writeSPI2(SE);
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);
    CS = 1;
}

void CE_cmd ()
{
    WREN_cmd();
    CS = 0;
    writeSPI2(CE);
    CS = 1;
}

void DPD_cmd()
{
    CS = 0;
    writeSPI2(DPD);
    CS = 1;
}


unsigned char RDID_cmd (unsigned char addr_tab[])
{
    unsigned char ID;
    CS = 0;
    writeSPI2(RDID);
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);
    ID = writeSPI2(0);
    CS = 1;
    return ID;
}


void WRITE_cmd(unsigned char addr_tab[],unsigned char byte)
{
    while(RDSR_cmd() & 0x1) ; // On vérifie si l'EEPROM est en cours de fonctionnement

    /*Autorisation d'écriture*/
    WREN_cmd();

    CS = 0;
    writeSPI2(WRITE);
    /*Envoi de l'adresse*/
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);
    /*Envoi du byte*/
    writeSPI2(byte);
    CS = 1;
}

unsigned char READ_cmd (unsigned char addr_tab[])
{
    int value;
    while(RDSR_cmd() & 0x1);  // On vérifie si l'EEPROM est en cours de fonctionnement

    CS = 0;
    /*Envoi de la commande*/
    writeSPI2(READ);
    /*Envoi de l'adresse*/
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);
    value = writeSPI2(0);
    CS = 1;
    return value;
}

void WRITE_cmd_32(unsigned char addr_tab[], int data) { // write a 32-bit value starting at an even address
    // wait until any work in progress is completed
    //while (RDSR_cmd() & 0x1); // check the WIP flag

     WREN_cmd();
    // perform a 32-bit write sequence (4 byte page write)
    CS = 0; // select the Serial EEPROM
    writeSPI2(WRITE); // write command


    //Envoi des valeurs en little-endian
    writeSPI2(addr_tab[2]); // address LSB (word aligned)
    writeSPI2(addr_tab[1]); // address LSB (word aligned)
    writeSPI2(addr_tab[0]); // address MSB first

    writeSPI2(data >> 24);  // Envoi du MSB
    writeSPI2(data >> 16);  // Envoi du 2ème byte
    writeSPI2(data >> 8);   // Envoi du 3ème byte
    writeSPI2(data);        // Envoi du LSB
    CS = 1;
}// writeSEE

unsigned int READ_cmd_32(unsigned char addr_tab[])
{
    // read a 32-bit value starting at an even address
    int i;
    // wait until any work in progress is completed
    //while (RDSR_cmd() & 0x1); // check WIP
    // perform a 16-bit read sequence (two byte sequential read)
    CS = 0; // select the Serial EEPROM
    writeSPI2(READ); // read command

    //Envoi des valeurs en little-endian
    writeSPI2(addr_tab[2]); // address LSB (word aligned)
    writeSPI2(addr_tab[1]); // address LSB (word aligned)
    writeSPI2(addr_tab[0]); // address MSB first

    i = writeSPI2( 0);   // send dummy, read msb
    i = (i << 8) + writeSPI2(0); // send dummy, read lsb
    i = (i << 8) + writeSPI2(0); // send dummy, read lsb
    i = (i << 8) + writeSPI2(0); // send dummy, read lsb
    CS = 1;
    return ( i);
}// readSEE

void WRITE_cmd_n (unsigned char addr_tab[],unsigned char data[],unsigned int n)
{
    char i;
    while (RDSR_cmd() & 0x1){} // check the WIP flag

     WREN_cmd();

    // perform a 32-bit write sequence (4 byte page write)
    CS = 0; // select the Serial EEPROM
    writeSPI2(WRITE); // write command
    
    //Envoi des valeurs en little-endian
    writeSPI2(addr_tab[2]); // address LSB (word aligned)
    writeSPI2(addr_tab[1]); // address LSB (word aligned)
    writeSPI2(addr_tab[0]); // address MSB first

    for(i=0;i<n;i++)
    {
        writeSPI2(data[i]);
    }
    CS = 1;
}

void READ_cmd_n(unsigned char addr_tab[],unsigned char data[],unsigned int n)
{
    // read a 32-bit value starting at an even address
    char i;
    // wait until any work in progress is completed
    while (RDSR_cmd() & 0x1){} // check the WIP flag

    // perform a 16-bit read sequence (two byte sequential read)
    CS = 0; // select the Serial EEPROM
    writeSPI2(READ); // read command

    //Envoi des valeurs en little-endian
    writeSPI2(addr_tab[2]); // address LSB (word aligned)
    writeSPI2(addr_tab[1]); // address LSB (word aligned)
    writeSPI2(addr_tab[0]); // address MSB first

   for(i=0;i<n;i++)
   {
       data[i]=writeSPI2(0);
   }

   CS = 1;
}// readSEE


int READ_string(unsigned char addr_tab[],unsigned char* data,unsigned int n)
{
    // read a 32-bit value starting at an even address
    int i;

    // wait until any work in progress is completed
    // while (RDSR_cmd() & 0x1); // check WIP

    // perform a 16-bit read sequence (two byte sequential read)
    CS = 0; // select the Serial EEPROM
    writeSPI2(READ); // read command

    //Envoi des valeurs en little-endian
    writeSPI2(addr_tab[2]); // address LSB (word aligned)
    writeSPI2(addr_tab[1]); // address LSB (word aligned)
    writeSPI2(addr_tab[0]); // address MSB first

   for(i=0;i<n;i++)
   {
       data[i]=writeSPI2(0);
       //Cette condition détecte la fin d'une chaîne de caractère
       if(data[i]=='\0')
       {
           break;
       }
   }

   CS = 1;

   return i;
}// readSEE

void WRITE_float (char addr_tab[],float value)
{
   //Variables
   union myfloat test;
   WRITE_cmd_n(addr_tab,test.nb,sizeof(test.f));
}

float READ_float (char addr_tab[])
{
  //Variables
  union myfloat test;
  READ_cmd_n(addr_tab,test.nb,sizeof(test.f));
  return test.f;
}