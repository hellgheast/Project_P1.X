/* 
 * File:   Gestion_porte.h
 * Author: mohammed.bensalah
 *
 * Created on 30. août 2014, 18:12
 */

#include "General_func.h"
#include "SPI_function.h"

//Variables globales
extern char user [128];

#ifndef GESTION_PORTE_H
#define	GESTION_PORTE_H


void SetDoorCode (int code);
void ChangeDoorPassword (int code);
int  CheckDoorPassword  (int code);
int  OpenDoor(int code);

void SetDoorCode (int code)
{
    //Variables locales
    union myadress door_adress;
    door_adress.adress = 0x36D;
    WRITE_cmd_32(door_adress.nb,code);
}

void ChangeDoorPassword (int code)
{
    //Variables locales
    union myadress door_adress;
    door_adress.adress = 0x36D;
    WRITE_cmd_32(door_adress.nb,code);
}

int CheckDoorPassword(int code)
{
    //Variables locales
    union myadress door_adress;
    int temp;

    door_adress.adress = 0x36D;
    temp = READ_cmd_32(door_adress.nb);
    if (code == temp)
    {
     return 1;
    }
    else
    {
      return 0;
    }

}

int  OpenDoor(int code)
{
    //Variables locales
    union myadress door_adress;
    int DoorCheck;
    door_adress.adress = 0x36D;

    DoorCheck = CheckDoorPassword(code);
    if (DoorCheck == 1)
    {
        printf("%s DOOR OPEN",user);
        LED4 = 1;
    }
    else
    {
        printf("%s DOOR NOT OPEN",user);
        LED4 = 0;
    }
}


#endif	/* GESTION_PORTE_H */

