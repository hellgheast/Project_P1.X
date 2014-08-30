#include "General_func.h"
void DelayMs(unsigned int msec)
{
	unsigned int tWait, tStart;

    tWait=(SYS_FREQ/2000)*msec;
    tStart=ReadCoreTimer();
    while((ReadCoreTimer()-tStart)<tWait);		// wait for the time to pass

}

int  ConvertLittletoBig(int var)
{
  union myadress temp;
  char temp_char;
  temp.adress = var;

  temp_char = temp.nb[0];
  temp.nb[0] = temp.nb[3];
  temp.nb[3] = temp_char;

  temp_char  = temp.nb[2];
  temp.nb[2] = temp.nb[1];
  temp.nb[1] = temp_char;

  return temp.adress;
}


void WriteByte (void)
{

  //Variables locales
  union myadress mon_adresse;
  int data;
  unsigned char datac;


  putsU3("WRITE BYTE MODE\n\r");
  done = 0;


  sscanf(function,"%d,%d",&mon_adresse.adress,&data);
  sprintf(function,"mon adresse est %d, et la donnée est %d\n",mon_adresse.adress,data);
  putsU3(function);

  datac = data;
  WRITE_cmd(mon_adresse.nb,datac);

  putsU3("WRITE END\n");
}


void ReadByte(void)
{
     //Variables locales
     union myadress mon_adresse;
     unsigned char datac;

    putsU3("READ MODE");
    done = 0;

    sscanf(function,"%d",&mon_adresse.adress);
    sprintf(function,"mon adresse est %d\n",mon_adresse.adress);
    putsU3(function);


    datac = READ_cmd(mon_adresse.nb);
    sprintf(function,"la donnée vaut %d\n",datac);
    putsU3(function);

    putsU3("READ END");

}

//Cas de l'écriture d'un float sur l'EEPROM
void FloatWrite(void)
{
    //Variables locales
    union myadress mon_adresse;
    union myfloat value;


    putsU3("WRITE FLOAT MODE");

    //Récupération de l'adresse ou on va écrire et du flottant
    sscanf(function,"%d,%f",&mon_adresse.adress,&value.f);
    sprintf(function,"mon adresse est %d est la valeur est %f\n",mon_adresse.adress,value.f);
    putsU3(function);

    //Ecriture dans l'EEPROM du flottant
    WRITE_cmd_n(mon_adresse.nb,value.nb,sizeof(value.f));
    putsU3("WRITE FLOAT END\n\r");
}

//Cas de la lecture d'un float sur l'EEPROM
void FloatRead (void)
{
  //Variables locales
  union myadress mon_adresse;
  union myfloat value;

  putsU3("READ FLOAT MODE");

  //Récupération de l'adresse ou on va lire
  sscanf(function,"%d",&mon_adresse.adress);
  sprintf(function,"mon adresse est %d\n",mon_adresse.adress);
  putsU3(function);

  //Lecture dans l'EEPROM
  READ_cmd_n(mon_adresse.nb,value.nb,sizeof(value.f));

  sprintf(function,"la donnée vaut %f\n",value.f);
  putsU3(function);

  putsU3("READ FLOAT END\n\r");

}

//Cas de l'écriture d'une chaîne de caractère
void StringWrite(void)
{
  //Variables locales
  union myadress mon_adresse;

  putsU3("WRITE STRING MODE");
  sscanf(function,"%d,%s",&mon_adresse.adress,buffer_s);
  sprintf(function,"mon adresse est %d et la chaine est : %s\n",mon_adresse.adress,buffer_s);
  putsU3(function);

  WRITE_cmd_n(mon_adresse.nb,buffer_s,strlen(buffer_s)+1);
  putsU3("WRITE STRING END\n\r");
}

//Cas de la lecture d'une chaîne de caractère
void StringRead(void)
{
  //Variables locales
    union myadress mon_adresse;

  putsU3("READ STRING MODE");
  sscanf(function,"%d",&mon_adresse.adress);
  sprintf(function,"mon adresse est %d\n",mon_adresse.adress);
  putsU3(function);

  READ_string(mon_adresse.nb,buffer_s,BUFFER_SIZE);
  sprintf(buffer,"La chaîne est %s \n",buffer_s);
  putsU3(buffer);
  putsU3("READ STRING END\n\r");

}

void GetTime (void)
{
  //Variables locales
  //Variables temporelles
  rtccTime    TEMPS;             // time structure
  rtccDate    DATE;            // date structure

  RtccGetTimeDate(&TEMPS,&DATE);
  sprintf(buffer,"La date et l'heure actuelle sont :\n\r %x %x %x -- %x:%x:%x\n",DATE.mday,DATE.mon,DATE.year,TEMPS.hour,TEMPS.min,TEMPS.sec);
  putsU3(buffer);
}

void GetLight (void)
{
    float lum = read_TL250();
    sprintf(buffer,"la lumière actuelle est de %d uW/cm2",ADCTL());
    putsU3(buffer);
}