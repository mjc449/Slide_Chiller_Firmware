// Metro_M0_Timer.h

#ifndef _METRO_M0_TIMER_h
#define _METRO_M0_TIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif //ARDUINO

class MetroTimer
{
public:
   MetroTimer();
   MetroTimer(int period, int matchVal);

   ~MetroTimer();

   void SetPeriod(int period);
   void SetMatchVal(int matchVal);
};

#endif //_METRO_M0_TIMER_h

