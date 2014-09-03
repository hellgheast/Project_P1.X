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
#include "User_Gestion.h"
#include "SPI_function.h"
#include "General_func.h"
#include <peripheral/rtcc.h>

#ifndef PWM_H
#define	PWM_H
#define Kp 256 //Lorsque erreur est inférieure à 4 degrés, on chauffe à 100%, Kp doit donner 1024 lorsque l'on est à 4 degrés.

void Regulation (void);
void InitPWM (void);
void ChangePWM (int param);
void gestion_chauffage(float);

void Init_Gestion_Chauffage(void);
void Read_temperature_pointer(void);
void Add_temperature(void);
void Get_Historique(void);

extern union myadress begin_temperature;
extern union myadress actual_temperature;
extern union myadress end_temperature;

void InitPWM()
{

    CloseOC3();
    OpenTimer2( T2_ON | T2_PS_1_2 | T2_SOURCE_INT, 1024);
    OpenOC3( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    SetDCOC3PWM(512);
}

void Init_Gestion_Chauffage(void)
{
    union myadress temp;

    begin_temperature.adress=0x400;
    actual_temperature.adress=0x400;
    end_temperature.adress=0x406;

    temp.adress = 0x0015;
    WRITE_cmd_n(temp.nb,begin_temperature.nb,3);

    temp.adress +=3;
    WRITE_cmd_n(temp.nb,actual_temperature.nb,3);

    temp.adress +=3;
    WRITE_cmd_n(temp.nb,end_temperature.nb,3);

}

void Read_temperature_pointer(void)
{
    union myadress temp;

    temp.adress=0x0015;
    READ_cmd_n(temp.nb,begin_temperature.nb,3);
    printf("BEGIN : %d \n",begin_temperature.adress);

    temp.adress += 3;
    READ_cmd_n(temp.nb,actual_temperature.nb,3);
    printf("ACTUAL : %d \n",actual_temperature.adress);

    temp.adress += 3;
    READ_cmd_n(temp.nb,end_temperature.nb,3);
    printf("END : %d \n",end_temperature.adress);
}

void Add_temperature(void)
{
    union myfloat int_temp;
    union myfloat ext_temp;
    char buffer[8];
    rtccTime mon_temps;
    rtccDate ma_date;
    unsigned char Time[3];
    unsigned char Date[3];
    union myadress i;
    union myadress a;

     RtccGetTimeDate(&mon_temps,&ma_date);

    Time[0]=mon_temps.b[1];
    Time[1]=mon_temps.b[2];
    Time[2]=mon_temps.b[3];

    Date[0]=ma_date.b[1];
    Date[1]=ma_date.b[2];
    Date[2]=ma_date.b[3];

    actual_temperature.adress=begin_temperature.adress;

    WRITE_cmd_n(actual_temperature.nb,Date,3);
    actual_temperature.adress+=3;

    WRITE_cmd_n(actual_temperature.nb,Time,3);
    actual_temperature.adress+=3;

    int_temp.f = read_kty_81_220(0);
    ext_temp.f = read_kty_81_220(2);

    for(i.adress=end_temperature.adress;i.adress>actual_temperature.adress;i.adress=i.adress-8) // Décalage des valeurs sur la droite
    {
        a.adress=i.adress-8;
        READ_cmd_n(a.nb,buffer,8);
        WRITE_cmd_n(i.nb,buffer,8);
    }
    WRITE_cmd_n(actual_temperature.nb,int_temp.nb,4);
    actual_temperature.adress+=4;

    WRITE_cmd_n(actual_temperature.nb,ext_temp.nb,4);

    if(end_temperature.adress<0x4e6) //0x4e6 = derniere adresse de la plage
    {
    end_temperature.adress+=8;
    }
    else
    {
        // Fin du plage mémoire
    }
}
void Get_Historique(void)
{
    //Pour l'instant affichage avec des printf
    char date[3];
    char heure[3];
    char temp[4];
    union myadress i;
    actual_temperature.adress=begin_temperature.adress;

    READ_cmd_n(actual_temperature.nb,date,3);
    actual_temperature.adress+=3;
    printf("Date : %s \n",date);

    READ_cmd_n(actual_temperature.nb,heure,3);
    actual_temperature.adress+=3;
    printf("Heure : %s \n",heure);

    for (i.adress=actual_temperature.adress;i.adress<=end_temperature.adress;i.adress+=4)
    {
        READ_cmd_n(i.nb,temp,4);
        printf("%s \n",temp);
    }
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

void gestion_chauffage(float consigne_temp)
{
    float int_temp;
    float ext_temp;
    float lum;
    int chauffage;

    int_temp = read_kty_81_220(0);
    //ext_temp = read_kty_81_220(2);
    //lum = read_TL250();

    //float erreur = (consigne_temp-lum/200.0) - int_temp; //calcul de l'erreur et diminution de la consigne selon la luminosité
    float erreur=consigne_temp-int_temp;
    if(erreur > 4.0)
    {
        chauffage = 1024;
    }
    else
    {
        chauffage= Kp*erreur;
        //chauffage = Kp * erreur + (int_temp-ext_temp)*10; //régulation p et prise en compte de la température extérieure.
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

