#include "Timer1.h"
#include "General_func.h"

void TimerInit (void)
{
    T1CON = 0x8030;
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256,0xFFFF); //PR1 = (SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)
    OpenTimer45(T45_ON|T45_SOURCE_INT|T45_PS_1_256,140625);//PR45 = (SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)
}
