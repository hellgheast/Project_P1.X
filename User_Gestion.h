/********************************************************************
Fichier      : Bluetooth_module.h
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 7. Août 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Librairie de fonctionnement de gestion des utilisateurs
--------------------------Modifications------------------------------
Version      : 1.2
Modification : Version_initiale
*********************************************************************/

#include "General_func.h"
#include "Bluetooth_Module.h"
#include "SPI_function.h"

#ifndef USER_GESTION_H
#define	USER_GESTION_H

//Prototypes de fonctions
int  AddUser    (char* user,char* password);
void DeleteUser (char* user);
int  CheckUser  (char* user,char* password);
void GetUsers   (void);
void ModifiyPassWord (char* user,char* new_password);

//
void Read_log_adress (void);
void Init_user_gestion (void);
void Write_log_addres (void);

//Fonction d'initialisation de la liste
void ListInit (void);



#endif	/* USER_GESTION_H */

