/********************************************************************
Fichier      : Gestion_Chauffage.h
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 25. ao�t 2014
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
#define Kp 256 

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

    CloseOC4();
    OpenOC4( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    SetDCOC4PWM(0);

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

    printf("Date : %x%x-%x%x-%x%x \n",(0xF0&Time[0]),(0x0F&Time[0]),(0xF0&Time[1]),(0x0F&Time[1]),(0xF0&Time[2]),(0x0F&Time[2]));

    Date[0]=ma_date.b[1];
    Date[1]=ma_date.b[2];
    Date[2]=ma_date.b[3];

     printf("Heure : %x%x-%x%x-%x%x \n",(0xF0&Date[0]),(0x0F&Date[0]),(0xF0&Date[1]),(0x0F&Date[1]),(0xF0&Date[2]),(0x0F&Date[2]));

    actual_temperature.adress=begin_temperature.adress;

    WRITE_cmd_n(actual_temperature.nb,Date,3);
    actual_temperature.adress+=3;

    WRITE_cmd_n(actual_temperature.nb,Time,3);
    actual_temperature.adress+=3;

    int_temp.f = read_kty_81_220(0);
    ext_temp.f = read_kty_81_220(2);

    for(i.adress=end_temperature.adress;i.adress>actual_temperature.adress;i.adress=i.adress-8) // We add the temperature at the begin of the list and we shift to the right the other
    {
        a.adress=i.adress-8;
        READ_cmd_n(a.nb,buffer,8);
        WRITE_cmd_n(i.nb,buffer,8);
    }
    WRITE_cmd_n(actual_temperature.nb,int_temp.nb,4);
    actual_temperature.adress+=4;

    WRITE_cmd_n(actual_temperature.nb,ext_temp.nb,4);

    if(end_temperature.adress<0x4e6) //0x4e6 the last memory position for the temperature
    {
    end_temperature.adress+=8;
    }
    else
    {
        // We are arrived at the end of the memmory we allowed for the heating
    }
}
void Get_Historique(void)
{
    //Printing the history 
    char date[3];
    char heure[3];
    char temp[4];
    union myadress i;
    union myfloat value;
    actual_temperature.adress=begin_temperature.adress;

    READ_cmd_n(actual_temperature.nb,date,3);
    actual_temperature.adress+=3;
    printf("Date : %x%x-%x%x-%x%x \n",(0xF0&date[0]),(0x0F&date[0]),(0xF0&date[1]),(0x0F&date[1]),(0xF0&date[2]),(0x0F&date[2]));


    READ_cmd_n(actual_temperature.nb,heure,3);
    actual_temperature.adress+=3;
    printf("Heure : %x%x-%x%x-%x%x \n",(0xF0&heure[0]),(0x0F&heure[0]),(0xF0&heure[1]),(0x0F&heure[1]),(0xF0&heure[2]),(0x0F&heure[2]));

    for (i.adress=actual_temperature.adress;i.adress<=end_temperature.adress;i.adress+=4)
    {
        READ_cmd_n(i.nb,value.nb,4);
        printf("%f\n",value.f);
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

    //ancien code 
    //float erreur = (consigne_temp-lum/200.0) - int_temp; //calcul de l'erreur et diminution de la consigne selon la luminosit�
    float erreur=consigne_temp+0.5-int_temp;
    if(erreur > 1.0)
    {
        chauffage = 1024;
    }
    else
    {
        chauffage= Kp*erreur;

        // Ancien code
        //chauffage = Kp * erreur + (int_temp-ext_temp)*10; //r�gulation p et prise en compte de la temp�rature ext�rieure.
    }
    
    if(chauffage <= 1024 && chauffage >=0) //Security test to avoid problems
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

