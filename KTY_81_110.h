#include "General_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <math.h>
#include <p32xxxx.h>
#include <conio.h>
#include <proc/p32mx795f512l.h>


/*Veuillez configurer les param�tres suivant*/

#define ROPT      3900          //R�sistance de Lin�arisation en OHM
#define R25       2020          //Resistance de KTY-81-220 � 25 degr�s en OHM
#define ALPHA     0.00768       //Constante Alpha
#define BETA      0.0000188     //Constante Beta
#define ATTENTE   200           //Temps d'attente entre chaque affichage [ms]
#define VREF      3.3           //Tension de r�f�rence [V]
#define QUANTUM   0.00322265625 //QUANTUM = VREF/2^Nbbit[AD]
/*------------------------------------------*/

/*Fonctions*/
/*-------------------------------------------*/
#define K_PARAM1 ADC_MODULE_ON | ADC_FORMAT_INTG16 | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON // On indique que l
#define K_PARAM2 ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SAMPLES_PER_INT_9| ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF
#define K_PARAM3 ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15
#define K_PARAM4 SKIP_SCAN_ALL
#define K_PARAM5 ENABLE_AN0_ANA
/*------------------------------------------*/
//Fonction de configuration de l'ADC

void InitADC (void)
{
  CloseADC10();
  SetChanADC10(ADC_CH0_POS_SAMPLEA_AN0| ADC_CH0_NEG_SAMPLEA_NVREF );
  OpenADC10(K_PARAM1,K_PARAM2,K_PARAM3,K_PARAM4,K_PARAM5);/*lE 0xFFFF d�sactive le scan d'aDC*/
  EnableADC10();
}



void InitADC_R()
{
    AD1CON1bits.ON=0;        //On d�sactive l'ADC
    AD1PCFGbits.PCFG0 = 0;   //On indique que AN0 est analogique
    AD1PCFGbits.PCFG1 = 0;   //On iniduqe que AN1 est analogique

    AD1CON1bits.SIDL=1;      //On indique que on arr�te la conversion en IDLE mode
    AD1CON1bits.FORM=0b000;  //On recoit un integer de 16 bits
    AD1CON1bits.SSRC=0b111;  //Auto Sampling actif
    AD1CON1bits.CLRASAM=0;   //�criture par dessus
    AD1CON1bits.ASAM=1;      //Sampling effectu� lorsque on demande un r�sultat

    AD1CON2bits.VCFG=0b000;  //R�f�rence de tension 3.3V
    AD1CON2bits.OFFCAL = 0;  //Calibration mode d�sactiv�
    AD1CON2bits.SMPI=0b1111; //Interrupt lorsque la 16�me s�quence � �t� faite
    AD1CON2bits.BUFM=0;      //Le buffer se comporte comme un buffer 16 bits
    AD1CON2bits.ALTS=0;      //On utilise toujours le multiplexeur A
    AD1CON2bits.CSCNA=0;     //Pas de scan des entr�es

    AD1CON3bits.ADRC=1;      //Clock interne de l'ADC
    AD1CON3bits.SAMC=0b11111;//31 Ticks d'auto-sample
    AD1CON3bits.ADCS=0xFF;   //Tad = 512 * P�riode du clock p�riph�rique

    AD1CHSbits.CH0NA=0;      //On s�lectionne -VR pour l'entr�e n�gative du multiplexeur
    AD1CHSbits.CH0NB=0;      //On s�lectionne -VR pour l'entr�e n�gative du multiplexeur
    AD1CHSbits.CH0SB=0b000;  //On s�lectionne le canal AN0 pour le d�but
    AD1CHSbits.CH0SA=0b000;  //On s�lectionne le canal AN0 pour le d�but

    AD1CSSLbits.CSSL=0x03;
    AD1CON1bits.ON=1;        //On active l'ADC

}


/*Protypes des fonctions*/
float delta_sqrt(float);
float U_to_R(long);
float temperature (float);
float read_kty_81_220(int); //canal s�l�ctionn�: 0 temp�rature int�rieure, 2 temp�rature ext�rieure.

/*Op�ration de lecture,convertion,retour de valeur*/
float read_kty_81_220(int channel)
{
  int i;
  long value = 0;//Variable pour  la valeur d�cimale lue sur le convertisseur AD
  float resistor;//Variable pour la valeur d�cimale de la r�sistance
  float value_d=0;//Variable pour la sauvegarde de la valeur de la racine de delta
  
  AD1CHSbits.CH0SA=0b0000; //On s�lectionne le canal AN0 pour le d�but


  DelayMs(1);
  while ( AD1CON1bits.DONE==0 ) { }
  value=ReadADC10(channel);
  //DelayMs(ATTENTE);
  
  resistor=U_to_R(value);
  value_d=delta_sqrt(resistor);
  
  return temperature(value_d);
}

/*Fonction de conversion de la tension en r�sistance*/
float U_to_R (long UR)
{
  float ures;
  float i;
  float r;
  
  ures=(UR*QUANTUM);
  i=(VREF-ures)/ROPT;
  r=(ures/i);
  return r ; 
}

/*Fonction de calcul de la racine delta*/
float delta_sqrt (float res)
{
  float delta;
  delta=(ALPHA*ALPHA)-(4*BETA*(1-(res/R25)));
  delta=sqrtf(delta);
  return delta;
}

/*Fonction de calcul de la temp�rature*/
float temperature (float delta)
{
 float temper=0;
 temper=25+((ALPHA*(-1))+delta)/(2*BETA);
 return temper; 
}



