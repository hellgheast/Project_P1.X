/* 
 * File:   TL2350R.h
 * Author: mohammed.bensalah
 *
 * Created on 6. juin 2014, 10:42
 */

#ifndef TL2350R_H
#define	TL2350R_H

#include "General_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <math.h>
#include <p32xxxx.h>
#include <conio.h>
#include <proc/p32mx795f512l.h>



/*Veuillez configurer les paramètres suivant*/
#define VREF      3.3           //Tension de référence [V]
#define QUANTUM   0.00322265625//QUANTUM = VREF/2^Nbbit[AD]
#define T_CHANNEL   1             //Canal sélectionné
/*------------------------------------------*/


/*Fonctions*/



/*Prototype des fonctions*/
float read_TL250(void);
float convert_U_to_L(int number);
int   ADCTL (void);



/*Opération de lecture,convertion,retour de valeur*/
float read_TL250(void)
{
  int i;
  long value = 0;//Variable pour  la valeur décimale lue sur le convertisseur AD
  float intensity;//Variable pour la valeur décimale de la résistance


  DelayMs(1);
  AD1CHSbits.CH0SA=0b001; //On sélectionne le canal AN1
  while ( AD1CON1bits.DONE==0 ) { }
  value=ReadADC10(T_CHANNEL);
  DelayMs(ATTENTE);

  intensity=convert_U_to_L(value);

  return intensity;
}

int   ADCTL (void)
{
    int val;
    AD1CHSbits.CH0SA=0b001; //On sélectionne le canal AN1
    while ( AD1CON1bits.DONE==0 ) { }
    val=ReadADC10(1);
    return val;
}


float convert_U_to_L(int number)
{
    float voltage;
    float intensity;

    voltage=number*QUANTUM;
    intensity=0.1398*voltage + 0.0051;
    return intensity;
}



#endif	/* TL2350R_H */

