#include <p32xxxx.h>
#include "SD_Card.h"


void SD_Card (void); //prototype
#define B_SIZE 512 // taille par défaut d'un bloc


char data1[ B_SIZE];
char buffer1[ B_SIZE];

#define START_ADDRESS 10000 // adresse de départ

void SD_Card (void)
{
    // code du livre
    LBA addr;
    int i,j,r;
    InitSD();
    for( i=0; i<B_SIZE; i++)//remplissage du buffer
    {
        data1[i]= i;
    }
    initMedia();
    addr=START_ADDRESS;
    for(j=0;j<16;j++) //écriture
    {
          writeSECTOR(addr+j, data1);
    }
    addr=START_ADDRESS;
    for(j=0;j<16;j++)
    {
        readSECTOR(addr+j, buffer1);//lecture
        if ( memcmp(data1, buffer1, B_SIZE))//comparaison du contenu des 2 buffers
        {
            //envoie d'un code d'erreur
        }
    }
    
}