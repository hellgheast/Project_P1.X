#include <p32xxxx.h>
#include <plib.h>

int count=1;
void timer(void); //prototype
#pragma interrupt InterruptHandler ipl1 vector 0
void InterruptHandler( void)
{
    if(count>29)// test effectué 30 fois -> 30 fois sec = ~5mn
    {
        PORTA=0x00; // led4 off
        count=1;    // reset du compteur
        mT1ClearIntFlag();
    }
    else
    {
        count ++;
        mT1ClearIntFlag();
    }

// interrupt service routine code here. . .
} // interrupt handler

void timer(void)
{
    TRISA=0;
    PORTA=0xff; // led4 on
    while (1)
    {
    PR1 = 19500; //19500 = ~ 10sec
    T1CON = 0x8030;
    mT1SetIntPriority(1);
    INTEnableSystemSingleVectoredInt();
    mT1IntEnable(1);
    };
}
