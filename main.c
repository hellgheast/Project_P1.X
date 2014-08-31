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
  isLogged = 0;

  //Configuration des interrupts

  // Configure UART RX Interrupt
  INTEnable(INT_SOURCE_UART_RX(UART_MODULE_ID),INT_ENABLED);
  INTSetVectorPriority(INT_VECTOR_UART(UART_MODULE_ID),INT_PRIORITY_LEVEL_4);


  //Configure Timer 1 Interrupt
  ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

  //Configure Timer 23 Interrupt
  ConfigIntTimer23(T23_INT_ON | T23_INT_PRIOR_2);

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
            case 0:
            {
                CheckLogin(user,password);
                break;
            }

            case 1:
            {
              LED4 = 1;
              putsU3("La led 4 est allumée\n\r");
              break;
            }

            case 2:
            {
              LED4 = 0;
              putsU3("La led 4 est éteinte\n\r");
              break;
            }

            case 3:
            {
              LED5 = 1;
              putsU3("La led 5 est allumée\n\r");
              break;
            }

            case 4:
            {
              LED5 = 0;
              putsU3("La led 5 est éteinte\n\r");
              break;
            }

            
            case 6:
            {
              int temp;
              //Command to check if the user have send the right pin code
              sscanf(function,"%d",&temp);
              OpenDoor(temp);
              break;
            }

            case 7:
            {
              int temp;
              //Pour set le code pin de la porte
              sscanf(function,"%d",&temp);
              SetDoorCode(temp);
              break;
            }

            case 81:
            {
              int temp;
              //Command to check if the user have send the right pin code
              sscanf(function,"%d",&temp);
              CheckDoorPassword(temp);
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

            case 53:
            {
              //test du rajout d'utilisateur
              sscanf(function,"%s,%s",buffer2,password);
              //Ecriture du compte en mémoire
              AddUser(buffer2,password);
              break;
            }

            case 54:
            {
                //test du rajout d'utilisateur
                sscanf(function,"%s",buffer2);
                //Ecriture du compte en mémoire
                DeleteUser(buffer2);
                break;
            }

            case 55:
            {
                //Test de la modification du mot de passe.
                sscanf(function,"%s,%s",buffer2,buffer_s);
                //Modification du mot de passe.
                ModifyPassWord(buffer2,buffer_s);
                break;
            }

            case 56:
            {
                //test de la vérification d'un USER
                sscanf(function,"%s,%s",buffer2,buffer_s);
                //Ecriture du compte en mémoire
                CheckLogin(buffer2,buffer_s);
                break;
            }


            case 57:
            {
                //Modification du username
                sscanf(function,"%s,%s",buffer2,buffer_s);
                //Modification du Username
                ModifiyUsername(buffer2,buffer_s);
                break;
            }

            case 70:
            {
                ListInit();
                break;
            }


            case 90:
            {
                //Initialisation
                InitNote();
                break;
            }

            case 91:
            {
                //Fonction d'ajout d'une note
                sscanf(function,"%s,%s,%s,%s",p_subject,buffer2,p_user,p_date);
                AddNote(p_subject,buffer2,p_user,p_date);
                break;
            }

            case 92:
            {
                //Fonction de suppresion d'une note
                sscanf(function,"%s,%s,%s,%s",p_subject,buffer2,p_user,p_date);
                DeleteNote(p_subject,buffer2,p_user,p_date);
                break;
            }

            case 93:
            {
                //Fonction de lecture d'une note
                sscanf(function,"%s,%s",p_subject,p_user);
                ReadNote(p_subject,p_user);
                break;
            }

            case 94:
            {
              //Fonction de lecture d'une note
              sscanf(function,"%s",p_user);
              ReadPersonnalNotes(p_user);
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
            case 251:
            {
                //
                var.adress = 0;
                sscanf(function,"%d,%d",&begin_log.adress);
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
                READ_cmd_n(&var.nb[1],&begin_log.nb[1],3);
                sprintf (buffer,"Valeur lue : %d\n",begin_log.adress);
                putsU3(buffer);
                break;
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

            #ifdef DEBUG
              sscanf(buffer,"%d,%s",&CMD,function);
            #endif

            #ifndef DEBUG
            sscanf(buffer," %s,%s,%d,%s",user,password,&CMD,function);
            #endif
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

        printf("5 minutes interrupt atteint !\n");

        count=1;    // reset du compteur
        mT1ClearIntFlag();
    }
    else
    {
        count ++;
        mT1ClearIntFlag();
    }

}

//Timer 23 Interrupt Handler,set at priority level 2
void __ISR(_TIMER_23_VECTOR,ipl2) IntTimer23Handler(void)
{
    if(count2 > 45)
    {
        mINT0IntEnable(1); //On active l'interrupt du capteur de mouvement.
        mT23IntEnable(0); //On désactive l'interrupt du timer 23.
    }
    else
    {
        count2++;
    }
    mT23ClearIntFlag();
}

// Interruption appelée par le capeur de mouvement. (Pin 3)
//_EXTERNAL_0_VECTOR est le vecteur pour le capteur de mouvement
// ipl7 est le niveau de priorité le plus haut: le premier à être servi.
void __ISR( _EXTERNAL_0_VECTOR, ipl1) INT0Handler( void)
{
    //Mettre ici ce qu'on fait lorsque le capteur de mouvement capte quelquechose.

    mINT0ClearIntFlag();// clear the flag and exit
} // Interrupt Handler

/*cette interrupt doit se désactiver et lancer un timer qui la réactive.
 * c'est nécéssaire pour ignorer les fronts montant lors de la fois
 * du signal du capteur de mouvement et aussi pour laisser le temps à
 * la caméra de transmettre son image avant de devoir en prendre une nouvelle.*/

