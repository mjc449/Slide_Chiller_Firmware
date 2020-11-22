// Thermocouple_Library.h

#ifndef _THERMOCOUPLE_LIBRARY_h
#define _THERMOCOUPLE_LIBRARY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Thermocouple
{
public:
   Thermocouple() = delete;
   Thermocouple& operator=(const Thermocouple&) = delete;
   Thermocouple& operator=(Thermocouple&&) = delete;
   Thermocouple(const Thermocouple&) = delete;
   Thermocouple(Thermocouple&&) = delete;

   friend Thermocouple* InitializeThermocouple(int);
   friend Thermocouple* RemoveThermocouple(int devNumber);

   int setZero();
   int setOneHundred();
   int currentValue();
   float currentTemperature();

   ~Thermocouple();

private:
   Thermocouple(int ssPin);
   int m_slaveSelect;
   float m_calibration;
};

Thermocouple* InitializeThermocouple(int devNumber);

#endif //_THERMOCOUPLE_LIBRARY_h

