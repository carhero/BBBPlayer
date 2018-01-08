/*
 * GPIOCtrl.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: Administrator
 */

#include<iostream>
#include<unistd.h> //for usleep
#include"../drivers/gpio/GPIO.h"
#include "../GPIO/GPIOCtrl.h"

using namespace exploringBB;
using namespace std;


//enum DIRECTION{ INPUT, OUTPUT };
//enum VALUE{ LOW=0, HIGH=1 };
//enum EDGE{ NONE, RISING, FALLING, BOTH };

GPIO outGPIO(49), inGPIO(115), out23GPIO(23);

int GPIOCtrl_Init()
{
//   GPIO outGPIO(49), inGPIO(115);

   // Basic Output - Flash the LED 10 times, once per second
   outGPIO.setDirection(OUTPUT);
   out23GPIO.setDirection(OUTPUT);

/*   for (int i=0; i<10; i++){
      outGPIO.setValue(HIGH);
      usleep(500000); //micro-second sleep 0.5 seconds
      outGPIO.setValue(LOW);
      usleep(500000);
   }*/
   // Basic Input example
   inGPIO.setDirection(INPUT);
   cout << "The value of the input is: "<< inGPIO.getValue() << endl;

   // Fast write to GPIO 1 million times
   outGPIO.streamOpen();
   out23GPIO.streamOpen();
/*   for (int i=0; i<1000000; i++){
      outGPIO.streamWrite(HIGH);
      outGPIO.streamWrite(LOW);
   }*/
//   outGPIO.streamClose();
//   out38GPIO.streamClose();

   return 0;
}

void GPIOCtrl_SetOut(GPIO_VALUE value)
{
    outGPIO.setValue(value);
    out23GPIO.setValue(value);
}



void GPIOCtrl_swingPort(void)
{
    int i = 1000;
    while(i)
    {
        i--;
        out23GPIO.streamWrite(HIGH);
        out23GPIO.streamWrite(LOW);
    }
}

