/********************************************************************
Fichier      : SD_Card
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 28 mars 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Fichier General
--------------------------Modifications------------------------------
Version      : 1.5
Modification : Ajout
*********************************************************************/

#ifndef GENERAL_FUNC_H
#define	GENERAL_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <math.h>
#include <p32xxxx.h>
#include <conio.h>
#include <proc/p32mx795f512l.h>

#define SYS_FREQ		(72000000)
#define UART_MODULE_ID UART3A
#define BUFFER_SIZE 128

#include <xc.h>

//Nos functions propres à nous
void DelayMs(unsigned int msec);
void InitADC (void);
void InitMemory (void);





/*********************************************************************
 * Type:        myfloat
 * Fonction:    Permet l'écriture d'un float sur l'EEPROM
 ********************************************************************/

//Union de données
union myfloat
{
    float f;
    char nb[4];
};

/*********************************************************************
 * Type:  		myadress
 * Fonction             Permet la gestion plus simple de l'incrémentation d'une adresse
 ********************************************************************/

union myadress
{
    int adress;
    char nb[4];
};


 
//Variables globales inter-fichier
//Pointeur
union myadress begin_temp;
union myadress actual_temp;
union myadress end_temp;

//Pointeur sur les adresses des logins
union myadress begin_log;
union myadress actual_log;
union myadress end_log;

//Variables gloabes
char buffer     [BUFFER_SIZE];
char user       [BUFFER_SIZE];
char password   [BUFFER_SIZE];
char function   [BUFFER_SIZE];
char done;
char cnt_debug;
int  CMD;
char buffer_s   [BUFFER_SIZE];



#endif	/* GENERAL_FUNC_H */

