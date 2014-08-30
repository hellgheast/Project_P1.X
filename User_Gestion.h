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
int  AddUser    (char* p_user,char* p_password);
void DeleteUser (char* p_user);
int  CheckUser  (char* p_user,char* p_password);
void GetUsers   (void);
int ModifyPassWord (char* p_user,char* new_password);
int ModifyUsername (char* p_user,char* new_user);
//
void Read_log_adress (void);
void Init_user_gestion (void);
void Write_log_addres (void);

//Fonction d'initialisation de la liste
void ListInit (void);
int CheckLogin(char* p_user, char* p_password);



#endif	/* USER_GESTION_H */

