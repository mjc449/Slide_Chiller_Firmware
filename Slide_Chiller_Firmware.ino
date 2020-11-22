/*
 Name:		Slide_Chiller_Firmware.ino
 Created:	11/15/2020 5:32:30 PM
 Author:	Marshall
*/

#include 
#include <SPI.h>

int timerFlag = 0;

void setup() {
   //Setup thermocouple SS pins
   pinMode(7, OUTPUT);
   pinMode(8, OUTPUT);
   digitalWrite(7, HIGH);
   digitalWrite(8, HIGH);

   //Setup the TEC current sensor
   pinMode(A1, OUTPUT);
   digitalWrite(A1, HIGH);
   pinMode(A2, INPUT);
   analogReadResolution(12);

   //Set TEC output to 0 (DAC mid-range)
   analogWrite(A0, 511);

   //Setup the clock for report timing
   REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |
      GCLK_CLKCTRL_GEN_GCLK0 |
      GCLK_CLKCTRL_ID_TCC0_TCC1;
   while (GCLK->STATUS.bit.SYNCBUSY);

   TCC0->CTRLA.bit.ENABLE = 0;
   while (TCC0->SYNCBUSY.bit.ENABLE);

   TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV2;

   TCC0->WAVE.reg |= TCC_WAVE_WAVEGEN_NFRQ;
   while (TCC0->SYNCBUSY.bit.WAVE);

   TCC0->PER.reg |= 0xFFFFFFFF;
   while (TCC0->SYNCBUSY.bit.PER);

   TCC0->CC[0].reg = 0xFFFFFFFF;
   while (TCC0->SYNCBUSY.bit.CC0);

   TCC0->INTENSET.reg = 0;
   TCC0->INTENSET.bit.OVF = 1;
   //TCC0->INTENSET.bit.MC0 = 1;

   NVIC_EnableIRQ(TCC0_IRQn);

   TCC0->CTRLA.bit.ENABLE = 1;
   while (TCC0->SYNCBUSY.bit.ENABLE);

   Serial.begin(9600);
   SPI.begin();
   delay(1000);
}

// the loop function runs over and over again until power down or reset
void loop() {
   //Take the incoming value and write it to the TEC output
   if (Serial.available() > 0)
   {
      Serial.println(Serial.available());
      int newVal = Serial.parseInt();
      Serial.println(newVal);
      analogWrite(A0, newVal);
   }

   if (timerFlag != 0)
   {
      ReportTemperature();
      ReportTECCurrent();
      timerFlag = 0;
   }
   return;
}

void TCC0_Handler()
{
   TCC0->INTENCLR.bit.OVF = 1;
   TCC0->INTFLAG.bit.OVF = 1;
   timerFlag = 1;
   TCC0->INTENSET.bit.OVF = 1;
   return;
}

void ReportTemperature()
{
   int tcValue{};
   float temperature{};
   
   digitalWrite(8, LOW);
   tcValue = SPI.transfer16(0x00);
   digitalWrite(8, HIGH);
   //Serial.println(tcValue, BIN);
   if ((tcValue >> 2) & 1UL)
      Serial.println("TC1 = disconnected");
   else
   {
      temperature = (float)(tcValue >> 3) * 0.25;
      Serial.println((String)"TC1 = " + temperature + " C");
   }
   digitalWrite(7, LOW);
   tcValue = SPI.transfer16(0x00);
   digitalWrite(7, HIGH);
   if ((tcValue >> 2) & 1UL)
      Serial.println("TC2 = disconnected");
   else
   {
      temperature = (float)(tcValue >> 3) * 0.25;
      Serial.println((String)"TC2 = " + temperature + " C");
   }
   return;
}

void ReportTECCurrent()
{
   int adcValue = analogRead(A2);
   //Vcc = 3.3 V
   //ADC resolution = 0.80586 mV/div (Vcc / 4095)
   //Current sensor resolution = 132 mV/A
   //I = 0 at 0.5 * Vcc
   //Current value = (adcValue - 0.5 * Vcc) * ADC res / sensor res
   double currentValue = (double)(adcValue - 2047) * 0.8058608058608059;
   currentValue /= 132.0;
   Serial.println((String)"TEC current = " + currentValue + " A");
   return;
}

void outputSin(int step)
{
   double angle = (double)step * PI / 127.0;
   double sinVal = sin(angle);
   int analogVal = (int)((sinVal * 512.0) + 512.0);
   analogWrite(A0, analogVal);
   return;
}