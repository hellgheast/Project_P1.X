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

void Regulation (void);
void InitPWM (void);
void ChangePWM (int param);
void gestion_chauffage(int);

void InitPWM()
{
    OpenOC1( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 2048, 2048);
    OpenTimer2( T2_ON | T2_PS_1_8 | T2_SOURCE_INT, 4096);
    /*
    T2CONCLR = T2_ON;        // Turn the timer off
    T2CON = T2_PS_1_8;        // Set prescaler
    TMR2 = 0;                 // Clear the counter
    PR2 = 4096;               // Set the period
    T2CONSET = T2_ON;         // Turn the timer on
     */
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
      SetDCOC1PWM(2048);
    }
    else
    {
      SetDCOC1PWM(param);
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

    int erreur = consigne_temp - int_temp;

    if(erreur > 4)
    {
        chauffage = 4096;
    }
    else
    {
        
    }

    ChangePWM (chauffage);
}

#endif	/* PWM_H */

