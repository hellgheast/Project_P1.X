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

    union myadress var;
    char buffer [64];

    //Ecriture
    var.adress = 0;
    WRITE_cmd_n(var.nb,begin_log.nb,4);
    sprintf(buffer,"%x\n",begin_log.adress);
    putsU3(buffer);
    
    //Lecture
    READ_cmd_n(var.nb,begin_log.nb,4);
    sprintf (buffer,"Valeur lue : %d\n",begin_log.adress);
    putsU3(buffer);

    putsU3("Begin writed\n");

    //Ecriture
    var.adress = 32;
    WRITE_cmd_n(var.nb,actual_log.nb,4);
    sprintf(buffer,"%x\n",actual_log.adress);
    putsU3(buffer);

    //Lecture
    READ_cmd_n(var.nb,actual_log.nb,4);
    sprintf (buffer,"Valeur lue : %d\n",actual_log.adress);
    putsU3(buffer);
    putsU3("Actual writed\n");

    //Ecrtiture
    var.adress = 64;
    WRITE_cmd_n(var.nb,end_log.nb,4);
    sprintf(buffer,"%x\n",end_log.adress);
    putsU3(buffer);

    //Lecture
    READ_cmd_n(var.nb,end_log.nb,4);
    sprintf (buffer,"Valeur lue : %d\n",end_log.adress);
    putsU3(buffer);
    putsU3("End writed\n");

  //  AddUser("ADMIN***","TEST****");
  
}

void Read_log_adress (void)
{
    union myadress var;
    char buffer [64];

    var.adress = 0x000;
    READ_cmd_n(var.nb,begin_log.nb,4);

    sprintf(buffer,"ADRESSE : %x BEGIN : %x\n",var.adress,begin_log.adress);
    putsU3(buffer);

    var.adress = 0x020;
    READ_cmd_n(var.nb,actual_log.nb,4);

    sprintf(buffer,"ADRESSE : %x ACTUAL : %x\n",var.adress,actual_log.adress);
    putsU3(buffer);


    var.adress = 0x040;
    READ_cmd_n(var.nb,end_log.nb,4);

    sprintf(buffer,"END : %x BEGIN : %x\n",var.adress,begin_log.adress);
    putsU3(buffer);

}

void Write_log_addres (void)
{
    union myadress var;
    char buffer [64];

    var.adress = 0x000;
    WRITE_cmd_n(var.nb,begin_log.nb,4);

    var.adress = 0x020;
    WRITE_cmd_n(var.nb,actual_log.nb,4);

    var.adress = 0x040;
    WRITE_cmd_n(var.nb,end_log.nb,4);
}

//Prototypes de fonctions
int  AddUser    (char* user,char* password)
{
    char buffer [32];

    Read_log_adress();

    //Vérification de la longeur des chaînes de caractères transmises.
    if(strlen(user)+1>9 || strlen(password)+1 >9)
    {
        return -1;
    }


    //Lecture dans la liste
    for(actual_log.adress = begin_log.adress;actual_log.adress!=0x360;actual_log.adress += 18 )
    {
        READ_string(actual_log.nb,buffer,32);
        if(strcmp(buffer,"********")==0)
        {
            WRITE_cmd_n(actual_log.nb,user,strlen(user)+1);
            actual_log.adress +=strlen(user)+1;
            WRITE_cmd_n(actual_log.nb,password,strlen(password)+1);
            actual_log.adress+=strlen(password)+1;
            Write_log_addres();
            break;
        }
    }

    //On indique que le nombre d'utilisateur maximum à été atteint.
    if (actual_log.adress == 0x360)
    {
        putsU3("Nombre maximum d'utilisateurs atteints\n");
        return -2;
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
       }
       else
       {
        actual_log.adress += 18;
       }
    }while(state==0);
}
int  CheckUser  (char* user,char* password)
{
    char buffer [64];
    char get_buffer[64];


    Read_log_adress();

    for(actual_log.adress = begin_log.adress;actual_log.adress!=0x360;actual_log.adress += 9 )
    {
        READ_string(actual_log.nb,get_buffer,64);
        sprintf(buffer,"USER : %s\n",get_buffer);
        putsU3(buffer);
        actual_log.adress += 9;

        if(strcmp(user,get_buffer)==0) //Si on trouve que le pseudo existe on retourne 1
        {
            return 1;
        }
    }

    return 0;                       //Si il n'existe pas on retourne 0


}
void GetUsers   (void)
{
  char buffer[64];
  char get_buffer[64];

  Read_log_adress();

  for(actual_log.adress = begin_log.adress;actual_log.adress!=0x360;actual_log.adress += 9 )
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

void ModifiyPassWord (char* user,char* new_password)
{
  char buffer [64];
    char get_buffer[64];
    
    
    Read_log_adress();

    for(actual_log.adress = begin_log.adress;actual_log.adress!=0x360;actual_log.adress += 9 )
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
        }
    }
    
}

void ListInit (void)
{
    union myadress temp_adress;
    int address_value = 0x300;
    unsigned char debug_buffer [64];
    unsigned char buffer [9] = "********";

    //Fonction pour initialiser la liste a vide
    temp_adress.adress = ConvertLittletoBig(address_value);
    sprintf(debug_buffer,"%d %d %d %d\n",temp_adress.nb[0],temp_adress.nb[1],temp_adress.nb[2],temp_adress.nb[3]);
    putsU3(debug_buffer);
    WRITE_cmd_n(&temp_adress.nb[1],buffer,9);

    address_value += 9;
    temp_adress.adress = ConvertLittletoBig(address_value);
    sprintf(debug_buffer,"%d %d %d %d\n",temp_adress.nb[0],temp_adress.nb[1],temp_adress.nb[2],temp_adress.nb[3]);
    putsU3(debug_buffer);
    WRITE_cmd_n(&temp_adress.nb[1],buffer,9);

    address_value += 9;
    temp_adress.adress = ConvertLittletoBig(address_value);
    sprintf(debug_buffer,"%d %d %d %d\n",temp_adress.nb[0],temp_adress.nb[1],temp_adress.nb[2],temp_adress.nb[3]);
    putsU3(debug_buffer);
    WRITE_cmd_n(&temp_adress.nb[1],buffer,9);

    address_value += 9;
    temp_adress.adress = ConvertLittletoBig(address_value);
    sprintf(debug_buffer,"%d %d %d %d\n",temp_adress.nb[0],temp_adress.nb[1],temp_adress.nb[2],temp_adress.nb[3]);
    putsU3(debug_buffer);
    WRITE_cmd_n(&temp_adress.nb[1],buffer,9);

    address_value += 9;
    temp_adress.adress = ConvertLittletoBig(address_value);
    sprintf(debug_buffer,"%d %d %d %d\n",temp_adress.nb[0],temp_adress.nb[1],temp_adress.nb[2],temp_adress.nb[3]);
    putsU3(debug_buffer);
    WRITE_cmd_n(&temp_adress.nb[1],buffer,9);

    address_value += 9;
    temp_adress.adress = ConvertLittletoBig(address_value);
    sprintf(debug_buffer,"%d %d %d %d\n",temp_adress.nb[0],temp_adress.nb[1],temp_adress.nb[2],temp_adress.nb[3]);
    putsU3(debug_buffer);
    WRITE_cmd_n(&temp_adress.nb[1],buffer,9);

    sprintf(debug_buffer,"%d %d %d %d\n",temp_adress.nb[0],temp_adress.nb[1],temp_adress.nb[2],temp_adress.nb[3]);
    putsU3(debug_buffer);


}
