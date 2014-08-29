#include "User_Gestion.h"
#include "General_func.h"


//Variables locales
extern union myadress begin_log;
extern union myadress actual_log;
extern union myadress end_log;
union myadress var;


#define DEBUG
#define begin_tab    &begin_log.nb[1]
#define actual_tab   &actual_log.nb[1]
#define end_tab      &end_log.nb[1]
#define var_tab      &var.nb[1]


//Fichier source
void Init_user_gestion (void)
{
    begin_log.adress    = 768;
    actual_log.adress   = 768;
    end_log.adress      = 768;

    union myadress temp;
    union myadress var;
    char i;

    temp.adress = 0x300;

    for(var.adress=0,i=0;i<3;i++)
    {
      //Ecriture
      WRITE_cmd_n(var.nb,temp.nb,3);
      var.adress = var.adress +3;
    }
}

void Read_log_adress (void)
{
    union myadress var;
    char buffer [64];

    var.adress = 0x0;
    READ_cmd_n(var.nb,begin_log.nb,3);
    
    var.adress = 0x3;
    READ_cmd_n(var.nb,actual_log.nb,3);
    
    var.adress = 0x6;
    READ_cmd_n(var.nb,end_log.nb,3);
    
}

void Write_log_addres (void)
{
    union myadress var;
    char buffer [64];

    var.adress = 0x0;
    WRITE_cmd_n(var.nb,begin_log.nb,3);

    var.adress = 0x3;
    WRITE_cmd_n(var.nb,actual_log.nb,3);

    var.adress = 0x6;
    WRITE_cmd_n(var.nb,end_log.nb,3);
}

//Prototypes de fonctions
int  AddUser    (char* user,char* password)
{
    char buffer [32];
    char printf_buffer[64];
    unsigned char cnt;

    Read_log_adress();

    //On indique que le nombre d'utilisateur maximum à été atteint.
    if (actual_log.adress > 0x36C)
    {
        putsU3("Nombre maximum d'utilisateurs atteints\n");
        return -2;
    }

    //Vérification de la longeur des chaînes de caractères transmises.
    if(strlen(user)+1 != 9 || strlen(password)+1 != 9)
    {
        return -1;
    }


    //Lecture dans la liste
    for(actual_log.adress = begin_log.adress,cnt=0;cnt<12;actual_log.adress += 18,cnt++ )
    {
        READ_string(actual_log.nb,buffer,32);
        sprintf(printf_buffer,"STRING : %s\n USER : %s PASSWORD : %s \n",buffer,user,password);
        putsU3(printf_buffer);

        if(strcmp(buffer,"********")==0)
        {
    
            WRITE_cmd_n(actual_log.nb,user,strlen(user)+1);
            actual_log.adress +=9;

            WRITE_cmd_n(actual_log.nb,password,strlen(password)+1);
            actual_log.adress+=strlen(password)+1;
            cnt = 13;
            Write_log_addres();
            return 1;

        }
    }

    
    
   
    return 0;
}
void DeleteUser (char* user)
{
    char buffer [128];
    char state = 0;
    char ERASER[9]= "********";
    short size;
   
    Read_log_adress();
    
    actual_log.adress = begin_log.adress;
    do{

       size = READ_string(actual_log.nb,buffer,128);
       if (strcmp(user,buffer)==0)
       {
         WRITE_cmd_n(actual_log.nb,ERASER,9);
         actual_log.adress += 9;
         WRITE_cmd_n(actual_log.nb,ERASER,9);
         actual_log.adress += 9;
         state = 1;
         putsU3("User deleted\n");
         Write_log_addres();
       }
       else
       {
        actual_log.adress += 18;
       }
    }while(state==0 && actual_log.adress < 0x36C);
}
int  CheckUser  (char* user,char* password)
{
    char buffer [64];
    char get_buffer[64];
    int cnt;

    Read_log_adress();

    for(actual_log.adress = begin_log.adress,cnt=0;cnt<6;actual_log.adress += 18,cnt++ )
    {
        READ_string(actual_log.nb,get_buffer,64);
        sprintf(buffer,"USER : %s\n",get_buffer);
        putsU3(buffer);

        if(strcmp(user,get_buffer)==0) //Si on trouve que le pseudo existe on retourne 1
        {
            putsU3("User found\n");

            cnt = 255;
            return 1;
        }
    }

    putsU3("User not found\n");
    return 0;                       //Si il n'existe pas on retourne 0

}

int CheckLogin (char* user,char* password)
{
    char buffer [64];
    char get_buffer[64];
    int cnt;
    Read_log_adress();

    for(actual_log.adress = begin_log.adress,cnt=0;cnt<6;actual_log.adress += 18,cnt++ )
    {
        READ_string(actual_log.nb,get_buffer,64);
        sprintf(buffer,"USER : %s\n",get_buffer);
        putsU3(buffer);

        if(strcmp(user,get_buffer)==0) //Si on trouve que le pseudo existe on retourne 1
        {
            actual_log.adress +=9;
            READ_string(actual_log.nb,get_buffer,64);
            if(strcmp(password,get_buffer)==0)
            {
                putsU3("LOGIN OK");
              return 1;
            }


        }
    }

    putsU3("User not found\n");
    return 0;                       //Si il n'existe pas on retourne 0



}

void GetUsers   (void)
{
  char buffer[64];
  char get_buffer[64];
  int cnt;

  Read_log_adress();

  for(actual_log.adress = begin_log.adress,cnt=0;cnt<6;actual_log.adress += 9,cnt++ )
    {

        READ_string(actual_log.nb,get_buffer,64);
        sprintf(buffer,"USER : %s\n",get_buffer);
        putsU3(buffer);
        actual_log.adress += 9;
        READ_string(actual_log.nb,get_buffer,64);
        sprintf(buffer,"PASSWORD : %s\n",get_buffer);
        putsU3(buffer);
    }
}

int ModifiyPassWord (char* user,char* new_password)
{
  char buffer [64];
    char get_buffer[64];
    
    
    Read_log_adress();

    for(actual_log.adress = begin_log.adress;actual_log.adress!=0x36C;actual_log.adress += 9 )
    {
        READ_string(actual_log.nb,get_buffer,64);
        sprintf(buffer,"USER : %s\n",get_buffer);
        putsU3(buffer);
        actual_log.adress += 9;
        
        if(strcmp(user,get_buffer)==0) //Si on trouve que le pseudo existe on retourne 1
        {
            //On modifie le mot de passer
            WRITE_cmd_n(actual_log.nb,new_password,strlen(new_password)+1);
            putsU3("Password changed\n");
            return 1;
        }
    }

    return 0;
    
}

void ListInit (void)
{
    union myadress temp_adress;
    int i;
    unsigned char debug_buffer [64];
    unsigned char buffer [9] = "********";
    
    //Fonction pour initialiser la liste a vide
    for(temp_adress.adress = 0x300,i=0;i<12;i++,temp_adress.adress+=9)
    {
       sprintf(debug_buffer,"ADDR_INIT : %x\n",temp_adress.adress);
       putsU3(debug_buffer);
       WRITE_cmd_n(temp_adress.nb,buffer,9);
    }   
}
