/********************************************************************
Fichier      : S25FL512S
---------------------------------------------------------------------
Auteur       : ISMAIL BEN SALAH
Compilateur  : MPLAB X
Date         : 1 septembre 2014
Version      : 2.15
---------------------------------------------------------------------
Description  : Librairie de fonctionnement de la flash SPANSION S25FL512S
--------------------------Modifications------------------------------
Version      : 1.2
Modification : Implémentation des fonctions d'effacement
*********************************************************************/

#ifndef S25FL512S_H
#define	S25FL512S_H

#include "SPI_function.h"



//Hardware definitions
#define CS_FLASH PORTEbits.RE3
#define CS_FLASH_IO TRISEbits.TRISE3

//List of the available command
#define REMS        0x90    //Read Electronic Manufacturer Signature
#define RDID        0x9F    //Read ID (JEDEC Manufacturer ID and JEDEC CFI)
#define RES         0xAB    //Read Electronic Signature
#define RSFDP       0x5A    //Read Serial Flash Discoverable Parameters

//Register Access
#define RDSR1       0x05    //Read Status Register-1
#define RDSR2       0x07    //Read Status Register-2
#define RDCR        0x35    //Read Configuration Register-1
#define WRR         0x01    //Write Register (Status-1, Configuration-1)
#define WRDI        0x04    //Write Disable
#define WREN        0x06    //Write Enable
#define CLSR        0x30    //Clear Status Register-1 - Erase/Prog. Fail Reset
#define ABRD        0x14    //AutoBoot Register Read
#define ABWR        0x15    //AutoBoot Register Write
#define BRRD        0x16    //Bank Register Read
#define BRWR        0x17    //Bank Register Write
#define BRAC        0xB9    //Bank Register Access
#define DLPRD       0x41    //Data Learning Pattern Read
#define PNVDLR      0x43    //Program NV Data Learning Register
#define WVLDR       0x4A    //Write Volatile Data Learning Register

//Read Flash Array
#define READ        0x03    //Read (3- or 4-byte address)
#define READ_4      0x13    //Read (4-byte address)
#define FAST_READ   0x0B    //Fast Read (3- or 4-byte address)
#define FAST_READ_4 0x0C    //Fast Read (4-byte address)
#define DDRFR       0x0D    //DDR Fast Read (3- or 4-byte address)
#define DDRFR_4     0x0E    //DDR Fast Read (4-byte address)
#define DOR         0x3B    //Read Dual Out (3- or 4-byte address)
#define DOR_4       0x3C    //Read Dual Out (4-byte address)
#define QOR         0x6B    //Read Quad Out (3- or 4-byte address)
#define QOR_4       0x6C    //Read Quad Out (4-byte address)
#define DIOR        0xBB    //Dual I/O Read (3- or 4-byte address)
#define DIOR_4      0xBC    //Dual I/O Read (4-byte address)
#define DDRDIOR     0xBD    //Dual I/O Read (3- or 4-byte address)
#define DDRDIOR_4   0xBE    //DDR Dual I/O Read (4-byte address)
#define QIOR        0xEB    //Quad I/O Read (3- or 4-byte address)
#define QIOR_4      0xEC    //Quad I/O Read (4-byte address)
#define DDRQIOR     0xED    //DDR Quad I/O Read (3- or 4-byte address)
#define DDRQIOR_4   0xEE    //DDR Quad I/O Read (4-byte address)

//Program Flash Array
#define PP          0x02    //Page Program (3- or 4-byte address)
#define PP_4        0x12    //Page Program (4-byte address)
#define QPP         0x32    //Quad Page Program (3- or 4-byte address)
#define QPP_A       0x38    //Quad Page Program - Alternate instruction (3- or 4-byte address)
#define QPP_4       0x34    //Quad Page Program (4-byte address)
#define PGSP        0x85    //Program Suspend
#define PGRS        0x8A    //Program Resume

//Erase Flash Array
#define BE          0x60    //Bulk Erase
#define BE_A        0xC7    //Bulk Erase (alternate command)
#define SE          0xD8    //Erase 256 kB(3- or -4byte adress)
#define SE_4        0xDC    //Erase 256 kB(4byte adress)
#define ERSP        0x75    //Erase Suspend
#define ERRS        0x7A    //Erase Resume

//One Time Program Array
#define OTPP        0x42    //OTP Program
#define OTPR        0x4B    //OTP Read

//Advanced sector management
#define DYBRD       0xE0    //DYB Read
#define DYBWE       0xE1    //DYB Write
#define PPBRD       0XE2    //PPB Read
#define PPBP        0xE3    //PPB Program
#define PPBE        0xE4    //PPB Erase
#define ASPR        0x2B    //ASO Read
#define ASPP        0x2F    //ASP Program
#define PLBRD       0xA7    //PPB Lock Bit Read
#define PLBWR       0xA6    //PPB Lock Bit Write
#define PASSRD      0xE7    //Password Read
#define PASSP       0xE8    //Password Program
#define PASSU       0xE9    //Password Unlock

//Reset
#define RESET       0xF0    //Software Reset
#define MBR         0xFF    //Mode Bit Reset

//Functions
void InitFlash (void);
void Write_Enable(void);
unsigned char RDSR1_cmd (void);
unsigned char RDSR2_cmd (void);

void READ_Flash_cmd_n(char addr_tab[],char get_tab[],int size_data);
void WRITE_Flash_cmd_n(char addr_tab[],char data_tab[],int size_data);

void Flash_Erase (void);
void Sector_Erase (char addr_tab[]);


void Write_Enable (void)
{
    CS_FLASH = 0;
    writeSPI2(WREN);
    CS_FLASH = 1;
}

void InitFlash (void)
{
    CS_FLASH_IO = 0;
}

unsigned char RDSR1_cmd (void)
{
    unsigned char reg;
    CS_FLASH = 0;
    reg = writeSPI2(RDSR1);
    CS_FLASH = 1;
    return reg;
}


unsigned char RDSR2_cmd (void)
{
  unsigned char reg;

  CS_FLASH = 0;
  reg = writeSPI2(RDSR2);
  CS_FLASH = 1;
  return reg;
}



void READ_Flash_cmd_n(char addr_tab[],char get_tab[],int size_data)
{
    int i;

    //Check if the flash if not working
    while(RDSR1_cmd() & 0x01){}

    //Lecture Phase
    CS_FLASH = 0;

    writeSPI2(READ_4);

    //Send the adress
    writeSPI2(addr_tab[3]);
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);

    for(i = 0;i<size_data;i++)
    {
        get_tab[i] = writeSPI2(0xFF);
    }

    CS_FLASH = 1;
}

void WRITE_Flash_cmd_n(char addr_tab[],char data_tab[],int size_data)
{

       int i;

    //Check if the flash if not working
    while(RDSR1_cmd() & 0x01){}


    //Set the Write Enable
    Write_Enable();

    //Lecture Phase
    CS_FLASH = 0;


    writeSPI2(PP_4);

    //Send the adress
    writeSPI2(addr_tab[3]);
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);

    for(i = 0;i<size_data;i++)
    {
      writeSPI2(data_tab[i]);
    }

    CS_FLASH = 1;

}

//Command to erase all the flash
void Flash_Erase (void)
{

    //Set the Write Enable
    Write_Enable();

    CS_FLASH = 0;

    //Bulk Erase to erase all the flash
    writeSPI2(BE);

    CS_FLASH = 1;

    //Check if the flash if not working
    while(RDSR1_cmd() & 0x01){}

}

void Sector_Erase (char addr_tab[])
{

     //Set the Write Enable
    Write_Enable();

    CS_FLASH = 0;

    //Send the command to erase à sector of 256 byte
    writeSPI2(SE_4);

    //Send the adress of the sector
    writeSPI2(addr_tab[3]);
    writeSPI2(addr_tab[2]);
    writeSPI2(addr_tab[1]);
    writeSPI2(addr_tab[0]);

    CS_FLASH = 1;
    
    //Check if the flash if not working
    while(RDSR1_cmd() & 0x01){}

}



#endif	/* S25FL512S_H */

