/********************************************************************
Fichier      : Notes_Gestion.h
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 7. Ao�t 2014
Version      : 2.05
---------------------------------------------------------------------
Description  : Librairie de fonctionnement de gestion des notes
--------------------------Modifications------------------------------
Version      : 1.2
Modification : Mise en place du compteur d'utilisateur
*********************************************************************/

#include "General_func.h"
#include "SPI_function.h"
#include "User_Gestion.h"

#ifndef NOTES_GESTION_H
#define	NOTES_GESTION_H



typedef struct
{
 char Subject [9];
 char User [9];
 char Date [11];
 char Text [129];
}mynote;


//Extern variable
extern union myadress begin_notes;
extern union myadress actual_notes;
extern union myadress end_notes;

//Global variable
unsigned char notes_count;

////Prototypes
void AddNote (char* subject,char* text,char* p_user,char* date);
void DeleteNote (char* subject,char* p_user);
void ReadNoteAll (void);

void ReadPublicNotes ();
void ReadPersonnalNotes (char* user);

void Read_notes_pointer(void);
void Write_notes_pointer(void);
void InitNote (void);

//Initialize the note
void InitNote (void)
{
    union myadress temp;
    char buffer[9]="********";

    begin_notes.adress  = 0x10000;
    actual_notes.adress = 0x10000;
    end_notes.adress    = 0x10000;
    notes_count =  0;

    temp.adress = 0x0000B;
    WRITE_cmd_n(temp.nb,begin_notes.nb,3);

    temp.adress += 3;
    WRITE_cmd_n(temp.nb,actual_notes.nb,3);

    temp.adress += 3;
    WRITE_cmd_n(temp.nb,end_notes.nb,3);

    temp.adress += 1;
    WRITE_cmd(temp.nb,notes_count);

    temp.adress = 0x10000;
    WRITE_cmd_n(temp.nb,buffer,9);

}

//Read the pointer's adress
void Read_notes_pointer(void)
{
  union myadress temp;
  
  temp.adress = 0x0000B;
  READ_cmd_n(temp.nb,begin_notes.nb,3);
  printf("BEGIN : %d\n",begin_notes.adress);


  temp.adress += 3;
  READ_cmd_n(temp.nb,actual_notes.nb,3);
  printf("ACTUAL : %d\n",actual_notes.adress);

  temp.adress += 3;
  READ_cmd_n(temp.nb,end_notes.nb,3);   
  printf("END : %d\n",end_notes.adress);

  temp.adress += 1;
  notes_count = READ_cmd(temp.nb);

  printf("NOTES COUNT: %u \n",notes_count);
}

//Write the pointer's adress in memory
void Write_notes_pointer(void)
{
    union myadress temp;

    temp.adress = 0x0000B;
    WRITE_cmd_n(temp.nb,begin_notes.nb,3);

    temp.adress += 3;
    WRITE_cmd_n(temp.nb,actual_notes.nb,3);

    temp.adress += 3;
    WRITE_cmd_n(temp.nb,end_notes.nb,3);

    temp.adress += 1;
    WRITE_cmd(temp.nb,notes_count);    
}


//Add note
void AddNote (char* subject,char* text,char* p_user,char* date)
{
    mynote NotetoSend;
    char get_buffer [32];
    char ERASER [9]="********";
    int get_free=0;

    //Copy the data in the structure
    strcpy(NotetoSend.Subject,subject);
    strcpy(NotetoSend.User,p_user);
    strcpy(NotetoSend.Date,date);
    strcpy(NotetoSend.Text,text);



    //Read pointers and informations about the notes
    Read_notes_pointer();

    //Start at the beginning of the chain
    actual_notes.adress = begin_notes.adress;

    //Loop to write in a free memory space
    do
    {
      READ_cmd_n(actual_notes.nb,get_buffer,9);
      if (strcmp(get_buffer,ERASER)==0)
      {
          get_free = 1;
      }
      else
      {
          actual_notes.adress += 9+9+11+129;
      }
    }
    while (get_free==0);


    //Write the note in the memory
    WRITE_cmd_n(actual_notes.nb,NotetoSend.Subject,9);
    actual_notes.adress += 9;

    WRITE_cmd_n(actual_notes.nb,NotetoSend.User,9);
    actual_notes.adress += 9;

    WRITE_cmd_n(actual_notes.nb,NotetoSend.Date,11);
    actual_notes.adress += 11;

    WRITE_cmd_n(actual_notes.nb,NotetoSend.Text,129);

    actual_notes.adress += 129;
    end_notes.adress = actual_notes.adress;

    WRITE_cmd_n(actual_notes.nb,"********",9);

    notes_count++;

    Write_notes_pointer();
}

void DeleteNote (char* subject,char* p_user)
{
    //Variables
    char get_buffer[64];
    unsigned char cnt=0;

    Read_notes_pointer();

    actual_notes.adress = begin_notes.adress;

    do
    {
      READ_cmd_n(actual_notes.nb,get_buffer,9); // Check if the subject is right.
      if(strcmp(subject,get_buffer)== 0)
      {
        actual_notes.adress += 9;
        READ_cmd_n(actual_notes.nb,get_buffer,9); // Check if the user is right.
        if(strcmp(p_user,get_buffer)==0)
        {
          notes_count--;
          actual_notes.adress-=9;
          WRITE_cmd_n(actual_notes.nb,"********",9);
          Write_notes_pointer();
          cnt = notes_count+1;
        }
        
        else
        {
             cnt++;
             actual_notes.adress += 9 + 11 + 129;
        }
      }

      else
      {
        cnt++;
        actual_notes.adress += 9 + 9 + 11 + 129;
      }
    }
    while(cnt<notes_count);
}

void ReadNoteAll(void)
{
    unsigned char cnt;
    unsigned char get_buffer [256];
    //Read pointers and informations about the notes
    Read_notes_pointer();

    //Start at the beginning of the chain
    actual_notes.adress = begin_notes.adress;

    for (cnt=0;cnt<notes_count;)
    {
      READ_cmd_n(actual_notes.nb,get_buffer,9);
      if (strcmp(get_buffer,"********")!=0)
      {

              printf("SUBJECT : %s\n",get_buffer);

              actual_notes.adress += 9;
              READ_cmd_n(actual_notes.nb,get_buffer,9);
              printf("DESTINATAIRE : %s\n",get_buffer);

              actual_notes.adress += 9;
              READ_cmd_n(actual_notes.nb,get_buffer,11);
              printf("DATE : %s\n",get_buffer);

              actual_notes.adress += 11;
              READ_string(actual_notes.nb,get_buffer,256);
              printf("TEXTE : %s\n",get_buffer);

              actual_notes.adress += 129;
              cnt++;
      }
      else
      {
          actual_notes.adress += 9+9+11+129;
      }
    }

}

//Read the personnal note
void ReadPersonnalNotes(char* user)
{
    unsigned char cnt;
    unsigned char get_subject [9];
    unsigned char get_buffer [256];

    //Read pointers and informations about the notes
    Read_notes_pointer();

    //Start at the beginning of the chain
    actual_notes.adress = begin_notes.adress;

    for (cnt=0;cnt<notes_count;cnt++)
    {
      READ_cmd_n(actual_notes.nb,get_subject,9);
      if (strcmp(get_subject,"********")!=0)
      {
          actual_notes.adress += 9;
          READ_cmd_n(actual_notes.nb,get_buffer,9);

          if(strcmp(get_buffer,user)==0)
          {
              printf("SUBJECT : %s\n",get_subject);
              memset(get_subject,0,9);
              
              printf("DESTINATAIRE : %s\n",get_buffer);

              actual_notes.adress += 9;
              READ_cmd_n(actual_notes.nb,get_buffer,11);
              printf("DATE : %s\n",get_buffer);

              actual_notes.adress += 11;
              READ_string(actual_notes.nb,get_buffer,256);
              printf("TEXTE : %s\n",get_buffer);

              actual_notes.adress += 129;
          }
          else
          {
            actual_notes.adress += 9+11+129;
            
          }
      }
      else
      {
          actual_notes.adress += 9+9+11+129;
      }
    }

}

//Read the public note
void ReadPublicNotes(void)
{
    unsigned char cnt;
    unsigned char get_subject [9];
    unsigned char get_buffer [256];

    //Read pointers and informations about the notes
    Read_notes_pointer();

        //Start at the beginning of the chain
    actual_notes.adress = begin_notes.adress;

    for (cnt=0;cnt<notes_count;cnt++)
    {
      READ_cmd_n(actual_notes.nb,get_subject,9);
      if (strcmp(get_subject,"********")!=0)
      {
          actual_notes.adress += 9;
          READ_cmd_n(actual_notes.nb,get_buffer,9);

          if(strcmp(get_buffer,"PUBLIC**")==0)
          {
              printf("SUBJECT : %s\n",get_subject);
              memset(get_subject,0,9);

              printf("DESTINATAIRE : %s\n",get_buffer);

              actual_notes.adress += 9;
              READ_cmd_n(actual_notes.nb,get_buffer,11);
              printf("DATE : %s\n",get_buffer);

              actual_notes.adress += 11;
              READ_string(actual_notes.nb,get_buffer,256);
              printf("TEXTE : %s\n",get_buffer);

              actual_notes.adress += 129;
          }
          else
          {
            actual_notes.adress += 9+11+129;

          }
      }
      else
      {
          actual_notes.adress += 9+9+11+129;
      }
    }

}




#endif	/* NOTES_GESTION_H */

