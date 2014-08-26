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