/* 
 * File:   Bluetooth_module.h
 * Author: mohammed.bensalah
 *
 * Created on 
 */
/********************************************************************
Fichier      : Bluetooth_module.h
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 23. juillet 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Librairie de fonctionnement du module RN 42 Bluetooth
--------------------------Modifications------------------------------
Version      : 1.2
Modification : Ajout des fonctions pour la gestion de puissance
*********************************************************************/


#ifndef BLUETOOTH_MODULE_H
#define	BLUETOOTH_MODULE_H
/* Configuration UART pour communication avec le module Bluetooth
 * Utilisation du port UART 3
 * RX   Pin 17
 * TX   Pin 16
 * CTS  Pin 15
 * RTS  Pin 14
 * --------------------------------------------------------------
 * Commande Bluetooth RN42
 *
 *
 */



/*Includes*/
#include "General_func.h"
#include <plib.h>
#include <stdlib.h>
#include <peripheral/uart.h>
#include <p32xxxx.h>

/*
** Asynchronous Serial Communication 
** UART2 RS232 asynchronous communication demonstration code 
*/

//Definitons diverses
#define SIZE_BUFFER 32
#define GET_BUFFER  256

//Definitions I/O 
#define TX      PORTFbits.RF5   // Transit ,sortie
#define RX      PORTFbits.RF4   // Receive ,entrée
#define CTS     PORTFbits.RF12  // Clear To Send,   entrée 
#define RTS     PORTFbits.RF13  // Request To Send, sortie

#define TRTS    TRISFbits.TRISF13   // Tris control for  RTS pin
#define TCTS    TRISFbits.TRISF12   // Tris control pour CTS pin
#define TTX     TRISFbits.TRISF5    // Tris control pour TX  pin
#define TRX     TRISFbits.TRISF4    // Tris control pour RX  pin
#define BRATE   78
//Prototypes

//Fonctions UART
void  initUART3A(void);             //Initialisation de l'UART
char  putU3     (char carac);       //Envoi d'un caractère via l'UART
char  getU3     (void);             //Reception d'un caractère via l'UART
char  putsU3    (char* string);     //Envoi d'une chaîne de caractère via l'UART
char  putsU3NR  (char* string);     //Envoi d'une chaîne de caractère via l'UART sans retour à la ligne
char* getsU3    (char* buffer,int len);//Réception d'une chaîne de caractère de l'UART


//Fonctions Bluetooth

//Fonctions de configuration
int SetCommandMode (void);          //Mise en place du Set de commande
int QuitCommandMode(void);          //Quitter le mode de commande
int SizeData (int choice);          //Permet de donner la taille des données transmises
int AuthMode (int choice);          //Permet de choisir le mode d'authenfication
int SetBreak (int choice);          //Permet un break dans l'UART

int SetClass    (char* string);
int SetDClass   (char* string);

int SetEncryption(char* string);
int FactoryReset (void);

int SetHID      (char* string);
int SetInquiry  (char* string);
int setPageScan (char* string);
int SetParity   (char carac);
int SetMode     (int choice);
int SetName     (char* name);
int SetOutput   (char* string);
int SetPin      (char* string);
int SetQconf    (int choice);
int SetRAdress  (char* address);
int SetServName (char* string);
int SetRConfiguration   (char* string);
int SetBaudRate         (char* string);
int SetSniff            (char* hex_time);
int SetBonding  (int value);
int setPower    (int value);
int SetNBaudrate(char* string);
int SetProfile(int value);
int SetSerialName(char* name);
int ModeSwitch(int value);
int SetSleepMode(char* value);

//GetCommands
int GetModuleAdress(char* buffer);
int GetConnectedAdress(char* buffer);
int GetStatus(void);
int GetRAdress(char* buffer);
int GetIOvalue(char* buffer);
int GetValue (char* carac,char* buffer);


//Command & Mode
void ActiveEcho(void);
void connectRemote(void);
void connectTo(char* adress);
void connectTF(char* adress);//Connexion en Fast Mode à l'adresse indiquée
void connectTFInq(void);     //Connexion en Fast Mode à la dernière addr du mode inquiry
void connectTFRem(void);     //Connexion en Fast MOde à la dernière adresse l'adresse stocké en remote
void connectTimed(char* adress,char* time); //Connexion à durée determinée

void SetFastData(void);
void InquiryScan(char* time,char* COD,char* bluetooth_adress []);
void InquiryScanSimple(char* time,char* COD,char* bluetooth_adress []);
void IntensityPairing(char* bluetooth_adress []);

//void InquiryScanRN
//void InquiryScanCR

int LinkQuality(char* quality);
void GetModemStatus(char* status);
void PutCharC(char* carac);
int SetQuietMode(char param,int* value);
void ForceReboot (void);
void PassDataC(int value);
void ChangeTempUART(char* Baudrate,char Parity);
void WakeUp(void);
void GetFirmware(char* firmware);
void SleepDevice(void);
void  HidePin(void);
int Disconnect(void);


//Fonction de commande des GPIO


//Fonctions



#endif	/* BLUETOOTH_MODULE_H */

