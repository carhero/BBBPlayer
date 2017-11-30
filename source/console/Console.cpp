/*
 * Console.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: Administrator
 */

#include <iostream>
#include<string>
#include "amp/AD820xx.h"

using namespace std;

unsigned char tmpVolume = 50;

void* Console_TaskMain(void* pArg)
{
    string cmd = "off";

    while(1)
    {
        getline (cin,cmd);
        cout << "input string is : "<< cmd << endl;

       if(cmd=="on")
       {
           cout << "Turning the LED" << cmd << endl;
       }
       else if(cmd == "up")
       {
           AD820xx_SetMasterVolume(++tmpVolume);
       }
       else if(cmd == "down")
       {
           AD820xx_SetMasterVolume(--tmpVolume);
       }
       else if (cmd=="reset")
       {
           cout << "AD820xx_SoftwareReset " << cmd << endl;
           AD820xx_SoftwareReset();
       }
       else if (cmd=="flash")
       {
           cout << "Turning the LED" << cmd << endl;
       }
       else if (cmd=="status")
       {
           cout << "Turning the LED" << cmd << endl;
       }
       else
       {
           cout << "Invalid command!" << endl;
       }
       //cmd = "";
    }
}
