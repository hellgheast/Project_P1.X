/********************************************************************
Fichier      : Notes_Gestion.h
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 7. Août 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Librairie de fonctionnement de gestion des utilisateurs
--------------------------Modifications------------------------------
Version      : 1.2
Modification : Mise en place du compteur d'utilisateur
*********************************************************************/

#include "General_func.h"
#include "SPI_function.h"

#ifndef NOTES_GESTION_H
#define	NOTES_GESTION_H

extern union myadress begin_notes;
extern union myadress end_notes;


//Prototypes de fonctions
void AddNote (char* subject,char*detail,char* p_user,char* date,char* text);
void InitNote (void)
{
    union myadress temp;

    begin_notes.adress = 0x10000;
    end_notes.adress = 0x10000;

    temp.adress = 0x0000B;
    WRITE_cmd_n(temp.nb,begin_notes.nb,3);

    temp.adress += 3;
    WRITE_cmd_n(temp.nb,end_notes.nb,3);
   
}
#endif	/* NOTES_GESTION_H */

