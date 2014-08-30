#include "Bluetooth_module.h"




//Fichier source pour les fonctions.

//Initialisation de l'UART en utilisant le port UART3A
/*
  UART1A = UART1
  UART2A = UART3
  UART3A = UART2
  UART1B = UART4
  UART2B = UART6
  UART3B = UART5
*/

//Fonction d'initialisation
void initUART3A(void)
{
    //Registre UxMode
    U2MODEbits.ON       = 1;    //UART actif
    U2MODEbits.SIDL     = 0;    //Stop in Idle Mod : 1 stop opérations en mode IDLE 0 : continue opérations en mode IDLE
    U2MODEbits.IREN     = 0;    //IrDA encoder decoder disable
    U2MODEbits.RTSMD    = 1;    //RTS est en mode simplex
    U2MODEbits.UEN      = 0b00; //Ici RX et TX sont actives,le contrôle de flux se fait via les pin définies plus haut dans le programme
    U2MODEbits.WAKE     = 1;    //Wake-up actif si on reçoit le start-bit
    U2MODEbits.LPBACK   = 0;    //Mode loopback désactivé
    U2MODEbits.ABAUD    = 0;    //Auto-Baud désactive
    U2MODEbits.RXINV    = 0;    //Inversion désactivé
    U2MODEbits.PDSEL    = 0b00; //8 bits de données,sans parité
    U2MODEbits.STSEL    = 0;    //1 bits de stop
    U2MODEbits.BRGH     = 1;    //Mode High-Speed actif

    //Registe UxSTAT
    U2STAbits.ADM_EN    = 0;    //Mode de détection automatique d'adresse désactivé
    U2STAbits.UTXISEL   = 0b00; //Interrupt activé quand il y a au moins un espace
    U2STAbits.UTXINV    = 0;    //
    U2STAbits.URXEN     = 1;    // On active la pin RX de l'uart
    U2STAbits.UTXBRK    = 0;    //
    U2STAbits.UTXEN     = 1;    // On active la pin TX de l'uart
    U2STAbits.URXISEL   = 0b00; // Interrupt lorsque l'on reçoit un caractère
    U2STAbits.OERR      = 0;    //On indique que le receive n'a pas fait d'overflow

    //Registre UxBRG
    UARTSetDataRate(UART3A,36000000,115200);

    //On reset les registres
    U2TXREG = 0;
    U2RXREG = 0;

    //Configuration de CTS,RTS
    TRTS = 0;
    TCTS = 1;
}

//Fonction pour envoyer un caractètre sur l'UART
char putU3(char carac)
{
   //while (CTS);       //Flow control inactif
    while(U2STAbits.UTXBF);
    U2TXREG = carac;
    return carac;
}

//Fonction pour recevori un caractètre sur l'UART
char getU3(void)
{
    //RTS = 0;          //Flow control inactif
    while (!U2STAbits.URXDA);
    //RTS = 1;          //Flow control inactif
    return U2RXREG;
}

//Fonction pour envoyer une chaîne de caractère
char putsU3 (char* string)
{
    while(*string) //Tant que l'on atteint pas la fin de la chaîne de caractère
    {
        putU3(*string++);
    }
    putU3('\r');
}

//Fonction pour envoyer une chaîne de caractère sans carridge return
char putsU3NR (char* string)
{
    while(*string) //Tant que l'on atteint pas la fin de la chaîne de caractère
    {
        putU3(*string++);
    }
}

//Fonction pour recevoir une chaîne de caractère
char* getsU3(char* buffer,int len)
{
    char *p = buffer; //On transmet l'adresse du buffer
    do
    {
     *buffer = getU3(); //On récupère un caractère
      if(*buffer == '\r') {break;} //On vérifie si le caractère récupère n'est pas celui de retour chariot
      buffer++;         //On pointe vers le prochain emplacement mémmoire
      len--;            //On indique que la longeur diminue
    }while(len > 1);
    *buffer='\0';
    return p;           //On retourne l'adresse du buffer;
}

//Fonctions Bluetooth
int SetCommandMode (void)
{
    //Fonction qui met le RN 42 en mode Commande
    int state;
    char* string = "$$$";

    char buf [SIZE_BUFFER];
    putsU3NR("$$$");
    getsU3(buf,SIZE_BUFFER);

    //Permet de vérifier si le RN 42 s'est mis en mode commande
    state = strcmp(buf,"CMD");
    if (state == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int QuitCommandMode(void)
{
    //Fonction qui permet de quitter le mode Commande du RN42
    int state;
    char* string = "---";
    char buf [SIZE_BUFFER];
    putsU3("---");
    getsU3(buf,SIZE_BUFFER);

    //Permet de vérifier si le RN 42 à bien quitté le mode
    state = strcmp(buf,"END");
    if (state == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
int SizeData(int choice)
{
    //Fonction qui permet de sélectionner le mode data 7 bit ou pas
    char buf [SIZE_BUFFER];

    if (choice == 1)
    {
        putsU3("S7,1");
    }
    if (choice == 0)
    {
        putsU3("S7,0");
    }

    getsU3(buf,SIZE_BUFFER);
    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int AuthMode (int choice)
{
   //Fonction qui permet de sélectionner le mode d'authetification
   char buf [SIZE_BUFFER];
   switch (choice)
   {
       //Open Mode,Bluetooth 2.0 sans cryptage sans SSP
       case 0:
       {
           putsU3("SA,0");
           break;
       }
       //SSP Keyboard mode,sur Androïd on demande un code pour se connecter
       case 1:
       {
           putsU3("SA,1");
           break;
       }
       //Mode de fonctionnnement standard,fonctionne avec android avec des packets non sécurisées
       case 2:
       {
           putsU3("SA,2");
           break;
       }
       //Mode code pin sans SSP
       case 4:
       {
           putsU3("SA,4");
           break;
       }
   }

    getsU3(buf,SIZE_BUFFER);
    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {
        return 0;
    }
    else
    {
      return -1;
    }

}

int SetBreak(int choice)
{
   //Fonction qui envoye un break sur l'UART
   char buf [SIZE_BUFFER];
   switch (choice)
   {
       //Break de 37 ms
       case 1:
       {
           putsU3("SB,1");
           break;
       }
       //Brak de 18.5 ms
       case 2:
       {
           putsU3("SB,2");
           break;
       }
       //Break de 12 ms
       case 3:
       {
           putsU3("SB,3");
           break;
       }

       //Break de 9 ms
       case 4:
       {
           putsU3("SB,4");
           break;
       }

       //Break de 7 ms
       case 5:
       {
           putsU3("SB,5");
           break;
       }

       //Break de 6 ms
       case 6:
       {
           putsU3("SB,6");
           break;
       }
   }

    getsU3(buf,SIZE_BUFFER);
    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}

}

int SetClass (char* string)
{
    //Fonction pour donner la classe du module (Most Significant word)
    char buf [SIZE_BUFFER] = "SC,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}

}

int SetDClass (char* string)
{
    //Fonction pour donner la classe du module (Least Significant word)
    char buf [SIZE_BUFFER] = "SD,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int FactoryReset (void)
{
    char buf [SIZE_BUFFER];
    putsU3("SF,1");

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetHID (char* string)
{
    //Fonction pour configurer le registre HID du RN42
    char buf [SIZE_BUFFER] = "SH,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetInquiry (char* string)
{
    //Fonction pour configurer la durée d'un inquiry scan
    char buf [SIZE_BUFFER] = "SI,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetPageScan(char* string)
{
    //Fonction pour configurer la durée d'un enable page scan
    char buf [SIZE_BUFFER] = "SJ,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetParity (char carac)
{
    //Fonction pour configurer la parité de l'UART coté RN42
    char buf [SIZE_BUFFER];

    switch(carac)
    {
        //EVEN - Le bit de parité = 1 si la trame contient un nombre pair de bits
        case 'E':
        {
           putsU3("SL,E");
           break;
        }

        //ODD - Le bit de parité = 1 si la trame contient un nombre impair de bits
        case 'O':
        {
            putsU3("SL,O");
            break;
        }

        //NONE - Le bit de parité a une valeur indéfinie non significative
        case 'N':
        {
            putsU3("SL,N");
            break;
        }
    }

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetMode (int choice)
{
  //Fonction qui configure le mode du Module
   char buf [SIZE_BUFFER];
   switch (choice)
   {
       //Mode Esclave
       case 0:
       {
           putsU3("SM,0");
           break;
       }
       //Mode Maître
       case 1:
       {
           putsU3("SM,1");
           break;
       }
       //Mode Trigger
       case 2:
       {
           putsU3("SM,2");
           break;
       }
       //Mode Auto-Connect Maître
       case 3:
       {
           putsU3("SM,3");
           break;
       }

       //Mode Auto-connect DTR
       case 4:
       {
           putsU3("SM,4");
           break;
       }

       //Mode Auto-Connect Quelquonque
       case 5:
       {
           putsU3("SM,5");
           break;
       }

       //Mode Pairing (Appairage)
       case 6:
       {
           putsU3("SM,6");
           break;
       }
   }

    getsU3(buf,SIZE_BUFFER);
    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}

}

int SetName (char* name)
{
    //Fonction pour donner un nom au module
    char buf [SIZE_BUFFER] = "SN,";
    strcat(buf,name);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetOuput (char* string)
{
    //Fonction qui montre l'état actuel du module ()
    //sting == ESC caractère d'échapement
    //String == space désactivation de setouput

    char buf [SIZE_BUFFER] = "SO,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetPin (char* string)
{
    //Fonction qui permet de configurer le code PIN
    char buf [SIZE_BUFFER] = "SP,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetQconf(int choice)
{
    //Fonction pour effectuer une configuration spéciale du module bluetooth
    char buf [SIZE_BUFFER];

    switch(choice)
    {
        //Aucun utilisation d'une configuration spéciale
        case 0:
        {
            putsU3("SQ,0");
            break;
        }
        //Ne lit pas GPIO 3 et 6 au démarrage (utilisation autre de GPIO3,6)
        case 4:
        {
            putsU3("SQ,4");
            break;
        }
        //Désactive la possibilité d'être détecté au démarrage
        //Pour désactiver cette configuration utiliser SQ 0
        case 8:
        {
            putsU3("SQ,8");
            break;
        }
        //Configuration pour optimiser le transfert pour des transferts à basse
        //latence que l'envoi direct
        case 16:
        {
            putsU3("SQ,16");
            break;
        }
        //Configuration pour que le module reboot après la déconnexion
        case 128:
        {
            putsU3("SQ,128");
            break;
        }
        //Configuration pour avoir 2 stop bit UART
        case 256:
        {
            putsU3("SQ,256");
            break;
        }
    }

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetRAdress(char* address)
{
    //Fonction qui permet de configurer l'adresse des modules qui vont se connecter
    //au module bluetooth
    //(sur 12 chiffres)
    char buf [SIZE_BUFFER] = "SR,";
    strcat(buf,address);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}

}

int SetServName (char* string)
{
    //Fonction qui permet de donner un nom au service bluetooth
    //(sur 12 chiffres)
    char buf [SIZE_BUFFER] = "SR,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetRConfiguration(char* string)
{
    //Fonction qui permet de configurer le timer de configuration et la configuration locale
    /* 0 aucune configuration à distance ou locale
     * 1-252 temps en seconde avant de permettre la configuration
     * 254 Configuration en contnu uniquement à distance
     * 255 Configuration en continu, à distance et en local
     */
    char buf [SIZE_BUFFER] = "ST,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetBaudRate(char* string)
{
    //Fonction qui permet de configurer le BaudRate de l'UART du module
    /* On a besoin uniquement des deux premiers chiffres pour configurer le baud-rate
     * 1200, 2400,4800, 9600, 19.2, 28.8, 38.4, 57.6, 115K, 230K, 460K, or 921K
     */
    char buf [SIZE_BUFFER] = "SU,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetSniff (char* hex_time)
{
    //Fonction qui permet de configurer le mode sniff (basse-consommation) du Module
    /* le temps est multiplie par 625 us,il est envoyé en hexa
     * 0 : désactive
     * tout autre
     */
    char buf [SIZE_BUFFER] = "SW,";
    strcat(buf,hex_time);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetBonding (int value)
{
  //Fonction qui determine quelles sont les connexions acceptées par le module
  // 1 si Bonding actif : uniquement dont il connait l'adresse écrite via SR
  // 0 Tout appareil

  char buf [SIZE_BUFFER];

  switch(value)
  {
      case 0:
      {
          putsU3("SX,0");
          break;
      }
      case 1:
      {
          putsU3("SX,1");
          break;
      }
  }

  getsU3(buf,SIZE_BUFFER);
  //Message d'erreur si tout se passe bien
  if (strcmp(buf,"AOK")== 0)
  {return 0;}
  else
  {return -1;}
}

int SetPower (int value)
{
  //Fonction qui permet de configurer le puissance du Module
  /*
   *    Valeur[en Hexa] Puissance [dBM]
   *    0010            16
   *    000C            12
   *    0008            8
   *    0004            4
   *    0000            0
   *    FFFC            -4
   *    FFF8            -8
   *    FFF4            -12
   */
  char buf [SIZE_BUFFER];

  switch(value)
  {
      case 0x0010:
      {
          putsU3("SY,0010");
          break;
      }
      case 0x000C:
      {
          putsU3("SY,000C");
          break;
      }
      case 0x0008:
      {
          putsU3("SY,0008");
          break;
      }
      case 0x0004:
      {
          putsU3("SY,0004");
          break;
      }
      case 0x0000:
      {
          putsU3("SY,0000");
          break;
      }
      case 0xFFFC:
      {
          putsU3("SY,FFFC");
          break;
      }
      case 0xFFF8:
      {
          putsU3("SY,FFF8");
          break;
      }
      case 0xFFF4:
      {
          putsU3("SY,FFF4");
          break;
      }
      default:
      {
          putsU3("SY,0000");
          break;
      }
  }

  getsU3(buf,SIZE_BUFFER);
  //Message d'erreur si tout se passe bien
  if (strcmp(buf,"AOK")== 0)
  {return 0;}
  else
  {return -1;}
}

int SetNBaudrate(char* string)
{
    //Fonction qui permet de configurer un baudrate non standard
    /* 0 aucune configuration à distance ou locale
     * Baudrate = Valeur/0.004096
     */
    char buf [SIZE_BUFFER] = "SZ,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetProfile(int value)
{
    //Fonction qui permet de sélectionner un profile pour le RN42
    /*
     * 0 : SPP
     * 1 : DUN-CTE
     * 2 : DUN-DTE
     * 3 : MDM SPP
     * 4 : SPP and DUN-DCE
     * 5 : APL
     * 6 : HID
     */
    char buf[SIZE_BUFFER];

    switch(value)
    {
        case 0:
        {
            putsU3("S~,0");
            break;
        }
        case 1:
        {
            putsU3("S~,1");
            break;
        }

        case 2:
        {
            putsU3("S~,2");
            break;
        }

        case 3:
        {
            putsU3("S~,3");
            break;
        }

        case 4:
        {
            putsU3("S~,4");
            break;
        }

        case 5:
        {
            putsU3("S~,5");
            break;
        }

        case 6:
        {
            putsU3("S~,6");
            break;
        }

    }


    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}

}

int SetSerialName(char* string)
{
    //Fonction qui permet de donner un nom au module
    //en gardant les deux derniers bytes de l'adresse MAC dans le nom

    char buf [SIZE_BUFFER] = "S-,";
    strcat(buf,string);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int ModeSwitch(int value)
{
    //Fonction qui permet d'effectuer un switch maître-esclave ou esclave-maître
    // 0 : Inactif
    // 1 : Actif
    char buf[SIZE_BUFFER];

     switch(value)
    {
        case 0:
        {
            putsU3("S?,0");
            break;
        }
        case 1:
        {
            putsU3("S?,1");
            break;
        }
     }


    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int SetSleepMode(char* value)
{
    //Fonction qui permet d'effectuer un sleep-mode
    //valeur en hexa, 0xXXYY XX étant la durée en secondes [hexa] ou le module dort
    // et YY la durée ou le module est éveillé (découvrable)

    char buf [SIZE_BUFFER] = "S|,";
    strcat(buf,value);//On concatène la chaîne de caractère
    putsU3(buf);

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"AOK")== 0)
    {return 0;}
    else
    {return -1;}
}

int GetModuleAdress(char* buffer)
{
    //Fonction pour obtenir l'adresse du module

    //Message pour recevoir l'adresse
    putsU3("GB");

    getsU3(buffer,strlen(buffer));

    //Message d'erreur si tout se passe bien
    if (strcmp(buffer,"NULL")== 0)
    {return -1;}
    else
    {return 0;}
}

int GetConnectedAdress(char* buffer)
{
    //Fonction pour obtenir l'adresse de l'appareil connecté

    //Message pour recevoir l'adresse de l'appareil
    putsU3("GF");

    getsU3(buffer,strlen(buffer));

    //Message d'erreur si tout se passe bien
    if (strcmp(buffer,"NULL")== 0)
    {return -1;}
    else
    {return 0;}
}

int GetStatus(void)
{
    //Fonction pour connaître le statut (en connexion ou pas)
    char buf [SIZE_BUFFER];
    //Message pour recevoir l'adresse
    putsU3("GB");

    getsU3(buf,SIZE_BUFFER);

    //Message d'erreur si tout se passe bien
    if (strcmp(buf,"1,0,0")== 0)
    {return 1;}
    if(strcmp(buf,"0,0,0")== 0)
    {return 0;}
    else
    {return -1;}
}

int GetRAdress(char* buffer)
{
    //Fonction pour obtenir l'adresse à DISTANCE du module

    //Message pour recevoir l'adresse de l'appareil
    putsU3("GR");

    getsU3(buffer,strlen(buffer));

    //Message d'erreur si tout se passe bien
    if (strcmp(buffer,"?")== 0||strcmp(buffer,"ERR")== 0)
    {return -1;}
    else
    {return 0;}
}

int GetIOvalue(char* buffer)
{
    //Fonction pour obtenir le byte hexa qui indique la valeur des IO

    //Message pour demander à recevoir la valeur de IO
    putsU3("G&");

    getsU3(buffer,strlen(buffer));

    //Message d'erreur si tout se passe bien
    if (strcmp(buffer,"?")== 0||strcmp(buffer,"ERR")== 0)
    {return -1;}
    else
    {return 0;}
}

int GetValue (char* carac,char* buffer)
{
    //Fonction qui permet d'obtenir la valeur d'un paramètre quelquonque.
    char l_buf [SIZE_BUFFER] = "G";
    strcat(l_buf,carac);//On concatène la chaîne de caractère
    putsU3(l_buf);

    getsU3(buffer,strlen(buffer));

    //Message d'erreur si tout se passe bien
    if (strcmp(buffer,"?")== 0||strcmp(buffer,"ERR")== 0)
    {return -1;}
    else
    {return 0;}
}

void ActiveEcho(void)
{
    //Fonction pour que  l'on recoive tout ce qui est envoyé
    putsU3("+");
}

void connectRemote(void)
{
    //Fonction qui se connecte à l'adresse stocké de l'appareil distant
    putsU3("C");
}

void connectTo(char* adress)
{
    //Fonction qui se connecte à l'adresse spécifié
    char l_buf [SIZE_BUFFER] = "C,";
    strcat(l_buf,adress);//On concatène la chaîne de caractère
    putsU3(l_buf);
}


void connectTF(char* adress)
{
    //Fonction de Connexion en Fast Mode à l'adresse indiquée
    char l_buf [SIZE_BUFFER] = "CF,";
    strcat(l_buf,adress);//On concatène la chaîne de caractère
    putsU3(l_buf);
}


void connectTFInq(void)
{
    //Fonction de Connexion en Fast Mode à la dernière addr du mode inquiry
    putsU3("CFI");
}

void connectTFRem(void)
{
  //Connexion en Fast MOde à la dernière adresse l'adresse stocké en remote
    putsU3("CFR");
}

void connectTimed(char* adress,char* time)
{
    //Connexion à durée determinée
    char l_buf [GET_BUFFER] = "CT,";
    strcat(l_buf,adress);//On concatène la chaîne de caractère
    strcat(l_buf,",");
    strcat(l_buf,time);
    putsU3(l_buf);
}

void SetFastData(void)
{
    //Fonction pour activer le Fast Mode
    //set qui quitte immédiatement le mode de commande
    putsU3("F,1");
}

void InquiryScan(char* time,char* COD,char* bluetooth_adress [])
{
        char l_buf [GET_BUFFER] = "I,";
        char i=0;
        //Fonction qui retourne les noms des appareils Bluetooth à portée.
        //Le temps est en secondes,le COD permet de sélectionner quelle classe d'appareil c'est.

        //Partie ou on récupère tout les devices détectables
        if(strcmp(COD,"XX")==0)
        {
            strcat(l_buf,time);
            putsU3(l_buf);
            while(getsU3(bluetooth_adress[i],strlen(bluetooth_adress[i]))!= NULL && i<9)
            {
                i++;
            }
        }

        //Partie ou on récupère les adresses d'une classe précise
        else
        {
            strcat(l_buf,time);
            strcat(l_buf,",");
            strcat(l_buf,COD);
            putsU3(l_buf);
            while(getsU3(bluetooth_adress[i],strlen(bluetooth_adress[i]))!= NULL)
            {
                i++;
            }
        }
}

void InquiryScanSimple(char* time,char* COD,char* bluetooth_adress [])
{
        //Fonction qui s'occupe de faire un scan de tout les appareils en mode
        //Appareillée (painring)
        char l_buf [GET_BUFFER] = "IN,";
        char i=0;

        //Partie ou on récupère tout les devices détectables
        if(strcmp(COD,"XX")==0)
        {
            strcat(l_buf,time);
            putsU3(l_buf);
            while(getsU3(bluetooth_adress[i],strlen(bluetooth_adress[i]))!= NULL && i<9)
            {
                i++;
            }
        }

        //Partie ou on récupère les adresses d'une classe précise
        else
        {
            strcat(l_buf,time);
            strcat(l_buf,",");
            strcat(l_buf,COD);
            putsU3(l_buf);
            while(getsU3(bluetooth_adress[i],strlen(bluetooth_adress[i]))!= NULL)
            {
                i++;
            }
        }



}

void IntensityPairing(char* bluetooth_adress[])
{
    char i=0;

    //Fonction qui s'occupe de retourner l'intensité de chacune des connexion éffectué
    putsU3("IQ");

    //On récupère les RSSI de chacue connexion
    while(strcmp(bluetooth_adress[i],"Inquiry Done")!=0)
    {
      getsU3(bluetooth_adress[i],strlen(bluetooth_adress[i]));
      i++;
    }
}

void HidePin (void)
{
    //Fonction pour cacher le mot de passe
    putsU3("J");
}

int Disconnect(void)
{
    //Fonction pour déconnecter le Module de la connexion
    char l_buf [SIZE_BUFFER];

    putsU3("K,");

    getsU3(l_buf,SIZE_BUFFER);
    //Message d'erreur si tout se passe bien
    if (strcmp(l_buf,"K,")== 0)
    {return 0;}
    else
    {return -1;}
}


int LinkQuality(char* quality)
{

    //Fonction qui indique la qualité de la connexion
    //Réponse du genre RSSI=xx,xx et continue d'envoyer tant que pas recu
    //à nouveau la commande
    putsU3("L");
    getsU3(quality,strlen(quality));
    //Si ce n'est pas connecté on retourne moins 1
    if(strstr("NOT",quality)!=NULL)
    {
        return -1;
    }
    //Sinon on retourne 0
    else
    {
        return 0;
    }
}

void GetModemStatus(char* status)
{
    //Fonction pour avoir le statut du signal du modem à distance ?
    putsU3("M");
    getsU3(status,strlen(status));
}

void PutCharC(char* carac)
{
    char l_buf[SIZE_BUFFER]="P,";
    //Fonction pour envoyer un caractère lorsque nous sommes dans le mode
    //de commande
    strcat(l_buf,carac);
    putsU3(l_buf);

}

int SetQuietMode(char param,int* value)
{
    char buffer[SIZE_BUFFER];
    //Fonction pour mettre en mode calme le module bluetooth
    //Si le firmware du module n'est pas 6.X utilisez l'option -1

    switch(param)
    {
        case 'K':
        {
            //On met le module en mode non-découvrable
            putsU3("Q");
            break;
        }
        case '0':
        {
            //On met le module en mode découvrable et possible de se connecter
            putsU3("Q,0");
            getsU3(buffer,SIZE_BUFFER);
            if(strcmp(buffer,"AOK")== 0)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }
        case '1':
        {
            //On met le module en mode non découvrable et impossible de se connecter
            putsU3("Q,1");
            break;
        }
        case '2':
        {
            //On le met en mode non découvrable et peut se connecter
            putsU3("Q,2");
            break;
        }
        case '?':
        {
            //Requête pour connaître dans quel mode est le module
            putsU3("Q,?");
            getsU3(buffer,SIZE_BUFFER);

            if(strcmp(buffer,"0")==0)
            {
              *value=0;
              break;
            }
            if(strcmp(buffer,"1")==0)
            {
              *value=1;
              break;
            }
            if(strcmp(buffer,"2")==0)
            {
              *value=2;
               break;
            }
            else
            {
                return -1;
            }
            break;
        }
    }
    return 0;

}

void ForceReboot (void)
{
    //Fonction pour faire redémarrer le module
    putsU3("R,1");
}

void PassDataC(int value)
{
    //Fonction pour laisser passer les données lorsque on est en mode commande
    switch(value)
    {
        case 0:
        {
            putsU3("T,0");
            break;
        }
        case 1:
        {
            putsU3("T,1");
            break;
        }
    }
}

void ChangeTempUART(char* Baudrate,char Parity)
{
    //Fonction pour changer temporairement le baudrate et la parité
    /*
     * La chaîne de caractère Baudrate doit correspondre exactement à une de ces
     * valeurs(uniquement les 4 premiers caractères.)
     * 1200, 2400,4800, 9600, 19.2K, 38.4K, 57.6K, 115K, 230K, 460K, or 921K.
     *
     * Pour la parité
     * O pour Odd
     * E pour Even
     * N pour None
     */

    char buffer [SIZE_BUFFER]="U,";
    strcat(buffer,Baudrate);//On rajoute le baudrate à la commande

    switch(Parity)
    {
        case 'O':
        {
            strcat(buffer,"O");
            break;
        }

        case 'E':
        {
            strcat(buffer,"E");
            break;
        }

        case 'N':
        {
            strcat(buffer,"N");
            break;
        }
    }
    putsU3(buffer);
}


void WakeUp(void)
{
    //Fonction pour réveiller le module
    putsU3("W");
}

void SleepDevice(void)
{
   //Fonction pour passer le module en mode veille
    putsU3("Z");
}