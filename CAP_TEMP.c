#include <CAP_TEMP.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "KTY_81_110.h"


/********************************************************************
Fichier      : CAP_TEMP.C
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : CSS COMPILER
Date         : 20.11.2012
Version      : 4.134
---------------------------------------------------------------------
Description  : Affichage de la distance.
--------------------------Modifications------------------------------
Version      : 1.1 - 21.11.2012
Modification : Ajout de fonctions de traitement
*********************************************************************/

void main()
{
   port_B_pullups(0x11);
   setup_adc_ports(sAN1);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_comparator(NC_NC_NC_NC);// This device COMP currently not supported by the PICWizard

  
  
  float value_temp=0;
  
  
  while(TRUE)
  {
    value_temp=read_kty_81_110();
    printf("\nLa température est de %.2f ºC\r",value_temp);
    
  }

}


