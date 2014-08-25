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


//Definitions
#define UART
#define BUFFER_SIZE 128
#define DEBUG


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


union myadress var;


//Définitions Hardware
#define LED5 PORTCbits.RC1
#define LED4 PORTAbits.RA3



void Init_module (void)
{
  //Fonction d'initialisation
  //Configuration des interrupts
  // Configure UART RX Interrupt
  INTEnable(INT_SOURCE_UART_RX(UART_MODULE_ID),INT_ENABLED);
  INTSetVectorPriority(INT_VECTOR_UART(UART_MODULE_ID),INT_PRIORITY_LEVEL_2);

  // Enable multi-vector interrupts
  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
  INTEnableInterrupts();

  InitADC_R();
  initUART3A();
  SPI2Init();
  RTCCInit();

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

    
    int  mon_adresse[3]={0x04,0x03,0x02};
    char mon_adresse_b[3];
    char datac;
    int  data = 0x55223311;
    
    float value;

    union myfloat test;

    
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
        //sscanf(buffer,"%s %d %s %s",user,&CMD,password,function);
        //sprintf(buffer,"USER : %s CMD: %d PWD : %s PARAM: %s",user,CMD,password,function);

       
        sprintf(buffer,"CMD: %d PARAM: %s\n",CMD,function);
        putsU3(buffer);
      
      }
        

    if(done==1){
        putsU3("IN swictch case");
        switch (CMD)
        {
            case 0:
            {
              LED4 = 1;
              putsU3("La led 4 est allumée\n\r");
              break;
            }

            case 1:
            {
              LED4 = 0;
              putsU3("La led 4 est éteinte\n\r");
              break;
            }

            case 2:
            {
              LED5 = 1;
              putsU3("La led 5 est allumée\n\r");
              break;
            }

            case 3:
            {
              LED5 = 0;
              putsU3("La led 5 est éteinte\n\r");
              break;
            }

            case 10:
            {
              float temp = read_kty_81_220();
              sprintf(buffer,"la température actuelle est de %f °C",temp);
              putsU3(buffer);
              break;
            }

            case 20:
            {
              WriteByte();
              break;
            }

            case 21:
            {
              ReadByte();
              break;
            }

            case 22:
            {
              FloatWrite();
              break;
            }

            case 23:
            {
              FloatRead();
              break;
            }

            case 24:
            {
                StringWrite();
                break;
            }

            case 25:
            {
                StringRead();
                break;
            }

            case 26:
            {
                CE_cmd();
                putsU3("CHIP ERASED");
                break;
            }

            case 40:
            {
                GetTime();
                break;
            }

            case 50:
            {
              Init_user_gestion();
              break;
            }

            case 51:
            {
              GetUsers();
              break;
            }

            case 52:
            {
              Read_log_adress ();
              break;
            }


            case 230:
            {
              SetCommandMode();
              Disconnect();
              QuitCommandMode();
              break;
            }

           

            #ifdef DEBUG
            case 240:
            {
                var.adress = 0x300;
                //test du rajout d'utilisateur
                sscanf(function,"%s %s",user,password);
                //Ecriture du compte en mémoire
                AddUser(user,password);
            }
            
            case 241:
            {
                var.adress = 0x300;
                //test du rajout d'utilisateur
                sscanf(function,"%s %s",user,password);
                //Ecriture du compte en mémoire
                DeleteUser(user,password);
            }

            case 251:
            {
                var.adress = 0;
                sscanf(function,"%d",&begin_log.adress);
                WRITE_cmd_n(var.nb,begin_log.nb,4);
                sprintf(buffer,"%x\n",begin_log.adress);
                putsU3(buffer);
                break;
            }

            case 252:
            {
                var.adress = 0;
                READ_cmd_n(var.nb,begin_log.nb,4);
                sprintf (buffer,"Valeur lue : %d\n",begin_log.adress);
                putsU3(buffer);
                break;
            }

            case 253:
            {
                sscanf(function,"%d,%d",&var.adress,&begin_log.adress);
                WRITE_cmd_n(var.nb,begin_log.nb,4);
                break;
            }

            case 254:
            {
                sscanf(function,"%d",&var.adress);
                READ_cmd_n(var.nb,begin_log.nb,4);
                sprintf (buffer,"Valeur lue : %d\n",begin_log.adress);
                putsU3(buffer);
            }

            #endif

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
        
        while(1)
        {
            for (i=0; i<0xfffff; i++);

            mon_adresse_b[0] = 0x03;
            mon_adresse_b[1] = 0x02;
            mon_adresse_b[2] = 0x01;
           
            CS=0;
            writeSPI2(0x06);
            CS=1;
            //WRITE_cmd(mon_adresse_b,0x01);

           /* d1=READ_cmd(mon_adresse);
            if(d1==0b10001000)
            {
                LED4 = 1;
            }
            else
            {
                LED4 = 0;
            }*/
           
        }

        SpiChnClose(2);
#endif
#ifdef TMP
    
#endif
            

                
           
 }

// UART 2 interrupt handler, set at priority level 2
void __ISR(_UART2_VECTOR, ipl2) IntUart2Handler(void)
{
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART3A)))
        {
           INTClearFlag(INT_SOURCE_UART_RX(UART3A)); // Clear the RX interrupt Flag
           INTDisableInterrupts(); //On désactive les interrupts le temps de l'acquisiton
            
            memset(buffer,0,128);   //On vide le buffer
            memset(function,0,128); //On remet la chaine des paramètres à zéro
            CMD = 8012;             //On remet la variable par défaut

            if(U2STAbits.FERR == 0 && U2STAbits.PERR == 0 )
            {
              putU3('K');
              getsU3(buffer,128);   //On récupère le message
              cnt_debug++;          //La variable de débug s'incrémente
              done=1;               //On indique qu'une transmission c'est faite
            }

            INTEnableInterrupts();  //On réactive les interrupts
            INTClearFlag(INT_SOURCE_UART_RX(UART3A)); // Clear the RX interrupt Flag
            sscanf(buffer,"%d %s",&CMD,function);


	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART_MODULE_ID)) )
	{
            INTClearFlag(INT_SOURCE_UART_TX(UART_MODULE_ID));
	}
}


void WriteByte (void)
{

  //Variables locales
  int mon_adresse[3];
  char mon_adresse_b[3];
  int data;
  unsigned char datac;


  putsU3("WRITE BYTE MODE\n\r");
  done = 0;


  sscanf(function,"%d,%d,%d,%d",&mon_adresse[0],&mon_adresse[1],&mon_adresse[2],&data);
  sprintf(function,"mon adresse est %d,%d,%d et la donnée est %d\n",mon_adresse[0],mon_adresse[1],mon_adresse[2],data);
  putsU3(function);

  /*Conversion d'un INT32 en char */
  mon_adresse_b[0]=mon_adresse[0];
  mon_adresse_b[1]=mon_adresse[1];
  mon_adresse_b[2]=mon_adresse[2];

  datac = data;
  WRITE_cmd(mon_adresse_b,datac);

  putsU3("WRITE END\n");
}


void ReadByte(void)
{
     //Variables locales
     int mon_adresse[3];
     char mon_adresse_b[3];
     unsigned char datac;

    putsU3("READ MODE");
    done = 0;

    sscanf(function,"%d,%d,%d",&mon_adresse[0],&mon_adresse[1],&mon_adresse[2]);
    sprintf(function,"mon adresse est %d,%d,%d\n",mon_adresse[0],mon_adresse[1],mon_adresse[2]);
    putsU3(function);

    mon_adresse_b[0]=mon_adresse[0];
    mon_adresse_b[1]=mon_adresse[1];
    mon_adresse_b[2]=mon_adresse[2];

   datac = READ_cmd(mon_adresse_b);
   sprintf(function,"la donnée vaut %d\n",datac);
   putsU3(function);

   putsU3("READ END");

}

//Cas de l'écriture d'un float sur l'EEPROM
void FloatWrite(void)
{
    //Variables locales
    int mon_adresse[3];
    char mon_adresse_b[3];
    union myfloat value;
    

    putsU3("WRITE FLOAT MODE");
    
    //Récupération de l'adresse ou on va écrire et du flottant
    sscanf(function,"%d,%d,%d,%f",&mon_adresse[0],&mon_adresse[1],&mon_adresse[2],&value.f);
    sprintf(function,"mon adresse est %d,%d,%d est la valeur est %f\n",mon_adresse[0],mon_adresse[1],mon_adresse[2],value.f);
    putsU3(function);

    //Conversion de l'adresse
    mon_adresse_b[0]=mon_adresse[0];
    mon_adresse_b[1]=mon_adresse[1];
    mon_adresse_b[2]=mon_adresse[2];

    //Ecriture dans l'EEPROM du flottant
    WRITE_cmd_n(mon_adresse_b,value.nb,sizeof(value.f));
    putsU3("WRITE FLOAT END\n\r");
}

//Cas de la lecture d'un float sur l'EEPROM
void FloatRead (void)
{
  //Variables locales
  int mon_adresse[3];
  char mon_adresse_b[3];
  union myfloat value;

  putsU3("READ FLOAT MODE");

  //Récupération de l'adresse ou on va lire
  sscanf(function,"%d,%d,%d",&mon_adresse[0],&mon_adresse[1],&mon_adresse[2]);
  sprintf(function,"mon adresse est %d,%d,%d\n",mon_adresse[0],mon_adresse[1],mon_adresse[2]);
  putsU3(function);

  //Conversion de l'adresse de int en char (de 4 octet à 1 octet)
  mon_adresse_b[0]=mon_adresse[0];
  mon_adresse_b[1]=mon_adresse[1];
  mon_adresse_b[2]=mon_adresse[2];

  //Lecture dans l'EEPROM
  READ_cmd_n(mon_adresse_b,value.nb,sizeof(value.f));

  sprintf(function,"la donnée vaut %f\n",value.f);
  putsU3(function);

  putsU3("READ FLOAT END\n\r");

}

//Cas de l'écriture d'une chaîne de caractère
void StringWrite(void)
{
  //Variables locales
  int mon_adresse[3];
  char mon_adresse_b[3];


  putsU3("WRITE STRING MODE");
  sscanf(function,"%d,%d,%d,%s",&mon_adresse[0],&mon_adresse[1],&mon_adresse[2],buffer_s);
  sprintf(function,"mon adresse est %d,%d,%d et la chaine est : %s\n",mon_adresse[0],mon_adresse[1],mon_adresse[2],buffer_s);
  putsU3(function);

  mon_adresse_b[0]=mon_adresse[0];
  mon_adresse_b[1]=mon_adresse[1];
  mon_adresse_b[2]=mon_adresse[2];

  WRITE_cmd_n(mon_adresse_b,buffer_s,strlen(buffer_s)+1);
  putsU3("WRITE STRING END\n\r");
}

//Cas de la lecture d'une chaîne de caractère
void StringRead(void)
{
  //Variables locales
  int mon_adresse[3];
  char mon_adresse_b[3];
 

  putsU3("READ STRING MODE");
  sscanf(function,"%d,%d,%d",&mon_adresse[0],&mon_adresse[1],&mon_adresse[2]);
  sprintf(function,"mon adresse est %d,%d,%d\n",mon_adresse[0],mon_adresse[1],mon_adresse[2]);
  putsU3(function);

  mon_adresse_b[0]=mon_adresse[0];
  mon_adresse_b[1]=mon_adresse[1];
  mon_adresse_b[2]=mon_adresse[2];

  READ_string(mon_adresse_b,buffer_s,BUFFER_SIZE);
  sprintf(buffer,"La chaîne est %s \n",buffer_s);
  putsU3(buffer);
  putsU3("READ STRING END\n\r");

}

void GetTime (void)
{
  //Variables locales
  //Variables temporelles
  rtccTime    TEMPS;             // time structure
  rtccDate    DATE;            // date structure

  RtccGetTimeDate(&TEMPS,&DATE);
  sprintf(buffer,"La date et l'heure actuelle sont :\n\r %x %x %x -- %x:%x:%x\n",DATE.mday,DATE.mon,DATE.year,TEMPS.hour,TEMPS.min,TEMPS.sec);
  putsU3(buffer);
}

void GetLight (void)
{
    float lum = read_TL250();
    sprintf(buffer,"la lumière actuelle est de %d uW/cm2",ADCTL());
    putsU3(buffer);
}