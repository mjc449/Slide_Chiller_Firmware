// 
// 
// 

#include "Thermocouple_Library.h"
#include <SPI.h>

static byte thermocoupleFlags{ 0 };
Thermocouple* tc1 = nullptr;
Thermocouple* tc2 = nullptr;

Thermocouple* InitializeThermocouple(int devNumber)
{
   if ((devNumber == 1) && ~(thermocoupleFlags & (1UL << 0)))
   {
      thermocoupleFlags |= 1UL << 0;
      tc1 = new Thermocouple(8);
      return tc1;
   }
   else if ((devNumber == 2) && ~(thermocoupleFlags & (1UL << 1)))
   {
      thermocoupleFlags |= 1UL << 1;
      tc2 = new Thermocouple(7);
      return tc2;
   }
   else
      return nullptr;
}

Thermocouple* RemoveThermocouple(Thermocouple* tcDevice)
{
   if (tcDevice == tc1)
   {
      thermocoupleFlags &= ~(1UL << 0);
      delete tc1;
      return nullptr;
   }
   else if (tcDevice == tc2)
   {
      thermocoupleFlags &= ~(1UL << 1);
      delete tc2;
      return nullptr;
   }
   else
      return tcDevice;
}

Thermocouple::Thermocouple(int ssPin)
{
   m_slaveSelect = ssPin;
   pinMode(m_slaveSelect, OUTPUT);
   digitalWrite(m_slaveSelect, HIGH);
   return;
}

Thermocouple::~Thermocouple()
{
   digitalWrite(m_slaveSelect, HIGH);
}

int Thermocouple::setZero()
{
   return 0;
}

int Thermocouple::setOneHundred()
{
   return 0;
}

int Thermocouple::currentValue()
{
   int val;
   digitalWrite(m_slaveSelect, LOW);
   SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
   val = SPI.transfer16(0x0000);
   digitalWrite(m_slaveSelect, HIGH);
   SPI.endTransaction();
   return val;
}

float Thermocouple::currentTemperature()
{
   float temperature;
   return temperature;
}