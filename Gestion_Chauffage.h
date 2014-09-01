/********************************************************************
Fichier      : Gestion_Chauffage.h
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 25. août 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Librairie de fonctionnement pour la gestion du chauffage.
--------------------------Modifications------------------------------
Version      : 1.0
Modification : Version Initiale
*********************************************************************/

#include <peripheral/outcompare.h>

#ifndef PWM_H
#define	PWM_H
#define Kp 256 //Lorsque erreur est inférieure à 4 degrés, on chauffe à 100%, Kp doit donner 1024 lorsque l'on est à 4 degrés.

void Regulation (void);
void InitPWM (void);
void ChangePWM (int param);
void gestion_chauffage(int);

void InitPWM()
{

    CloseOC3();
    OpenTimer2( T2_ON | T2_PS_1_2 | T2_SOURCE_INT, 1024);
    OpenOC3( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    SetDCOC3PWM(512);
}
void function_ (void)
{
    OC1R = 2048;              // Load initial value into duty cycle register
    OC1RS = 2048;             // When a period finishes the contents of OC1RS is loaded to OC1R
    OC1CON = OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE; // Set Timer2 as source | enable pwm mode without fault protection
    OC1CONSET = OC_ON;
}


void ChangePWM (int param)
{
    if(param > PR2)
    {
      SetDCOC3PWM(512);
    }
    else
    {
      SetDCOC3PWM(param);
    }

}

void gestion_chauffage(int consigne_temp)
{
    float int_temp;
    float ext_temp;
    float lum;
    int chauffage;

    int_temp = read_kty_81_220(0);
    ext_temp = read_kty_81_220(2);
    lum = read_TL250();

    float erreur = (consigne_temp-lum/200.0) - int_temp; //calcul de l'erreur et diminution de la consigne selon la luminosité

    if(erreur > 4.0)
    {
        chauffage = 4096;
    }
    else
    {
        chauffage = Kp * erreur + (int_temp-ext_temp)*10; //régulation p et prise en compte de la température extérieure.
    }

    if(chauffage <= 1024 && chauffage >=0) //test de sécurité pour éviter les problèmes.
    {
        ChangePWM (chauffage);
    }
    else if(chauffage < 0)
    {
         ChangePWM (0);
    }
    else
    {
         ChangePWM (1024);
    }
}

#endif	/* PWM_H */

