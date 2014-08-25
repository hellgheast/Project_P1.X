/********************************************************************
Fichier      : Bluetooth_module.h
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 7. Août 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Librairie de fonctionnement du RTCC
--------------------------Modifications------------------------------
Version      : 1.2
Modification : Ajout du lock
*********************************************************************/

#ifndef RTCC_H
#define	RTCC_H

#include <peripheral/rtcc.h>
/*Prototypes*/
void RTCCInit(void);
void RTCCLock(void);

/*Fonctions*/

//Fonction d'initialisation
void RTCCInit(void)
{

    RTCCLock();
    RTCCONbits.CAL =0;
    RTCCONbits.SIDL = 0;
    RTCALRMbits.ALRMEN = 0;

    RtccInit();
    rtccTime    tm;             // time structure
    rtccDate    dt;            // date structure

        tm.l    = 0;
        tm.sec  = 0x00;
        tm.min  = 0x36;
        tm.hour = 0x10;

        dt.wday = 1;
        dt.mday = 0x18;
        dt.mon  = 0x08;
        dt.year = 0x14;
        RtccSetTimeDate(tm.l, dt.l);
        RtccEnable();
        RtccWrEnable(1);
        RtccEnable();
        RtccOpen(tm.l, dt.l, 0);

        RTCCONSET=0x8000; // turn on the RTCC
 
}

void RTCCLock (void)
{
    INTDisableInterrupts(); //On désactive les interrupts le temps de la configuration

    SYSKEY = 0xaa996655; // write first unlock key to SYSKEY
    SYSKEY = 0x556699aa; // write second unlock key to SYSKEY
    RTCCONSET = 0x8; // set RTCWREN in RTCCONSET

    INTEnableInterrupts();
}

#endif	/* RTCC_H */

