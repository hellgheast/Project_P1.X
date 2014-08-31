/* 
 * File:   SPI_function.h
 * Author: mohammed.bensalah
 *
 * Created on 25. avril 2014, 09:35
 * Dernière modification 23 Juillet 2014 16:12
 */

#include "General_func.h"
#include <peripheral/SPI_Port_Mappings.h>
#include <peripheral/spi.h>
#include <p32xxxx.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SPI_FUNCTION_H

#define	SPI_FUNCTION_H

/*Définitions*/

/*Taille des données transferèes*/
#define TAILLE_MAX 65 //TaileMaximum
#define TAILLE_MIN 1

/*Commande EEPROM*/

#define CHANNEL_SPI 3

/*Prototype*/

void  WRDI_cmd    (void);           //Write Disable
void  WREN_cmd    (void);           //Write Enable
void  WRSR_cmd    (int BP1,int BP0);//Write Status Register
unsigned char  RDSR_cmd    (void);           //Read Status Register
void  PE_cmd      (unsigned char addr_tab[]);//Page Erase
void  SE_cmd      (unsigned char addr_tab[]);//Sector Erase
void  CE_cmd      (void);           //Chip Erase
void  DPD_cmd     ();               //Deep Power Down Mode
unsigned char  RDID_cmd    (unsigned char addr_tab[]);//Release from Deep Power Down


void  WRITE_cmd   (unsigned char addr_tab[],unsigned char byte);
void  WRITE_cmd_32(unsigned char addr_tab[],int data);
void  WRITE_cmd_n (unsigned char addr_tab[],unsigned char data[], unsigned int n);
unsigned char  READ_cmd       (unsigned char addr_tab[]);
unsigned int   READ_cmd_32    (unsigned char addr_tab[]);
void READ_cmd_n(unsigned char addr_tab[],unsigned char *data,unsigned int n);
int READ_string(unsigned char addr_tab[],unsigned char *data,unsigned int n);

unsigned char writeSPI2      (unsigned char data );
unsigned char readSPI2       (void);

void SetSPICONFIG (int chn,int isMaster,int taille_transmission);
void SPI2Init (void);


//Fonctions d'intercompatibilité
void WRITE_float (char addr_tab[],float value);
float READ_float (char addr_tab[]);

/************************************
 Les pins pour CS,WP et HOLD sont
 * WP barre   : Pin 37 RE0
 * CS barre   : Pin 36 RE1
 * HOLD barre : Pin 35 RE2
 * MISO         Pin 50 RG7
 * MOSI         Pin 51 RG8
 * SCK          Pin 52 RG6
 * SS           Pin 53 RG9
************************************/

#define WP    PORTEbits.RE0
#define CS    PORTEbits.RE1
#define HOLD  PORTEbits.RE2





#endif	/* SPI_FUNCTION_H */

