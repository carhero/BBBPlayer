/**************** (C) Anam Electronics. *********************************************
PROJECT						: VSR-50/70/130
Microcontroller				: STM32F401RB ( 32bit, 64pin, 128K(ROM), 64K(RAM) )
COMPILER					: IAR-EWARM 7.7, J-Link-ARM
MODULE						: AD820xx.c
AUTHOR						: ANAM ELECTRONICS SOFT TEAM
CREATION DATE	    		: 2017. 07. 26
DESCRIPTION			    	: AD820xx driver
*******************************************************************************/
#include <stdio.h>
#include "AD820xx.h"
#include "../drivers/I2cPort.h"
//#include "McuI2c.h"
//#include "DBG.h"
//#include "McuPortCtrl.h"

using namespace cacaosd_i2cport;

#define DBG printf
I2cPort *i2c;

#if 1

#define TRUE		1
#define FAIL		0

#define I2C_CLOCK				255
#define DEV_ADDR				0x34 //8bit I2C device address(Pin2 ERROR=High(0x68),ERROR=Low(0x60))

#define EEPROM_WR				0
#define EEPROM_RD				1
#define PAGE_SIZE				32
#define ERROR_CODE			0x78

const unsigned char aVolumeTbl[100] = {  
43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,
63,64,65,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,
100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,
132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,
164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,
196,198,200,203,206,209,212,215,218,221,224,227,};

void AD820xx_SetMasterVolume(unsigned char volume)
{
  unsigned char reg = 100 - volume;

  /* MUTE */
  if(volume == 0)
  {
    reg = 0xFF;
  }
  else
  {
    reg = aVolumeTbl[100 - volume];
    //reg = 0x27 + (100 - volume);
    //reg = 0x27;
  }

  i2c->writeByte(AD820_REG_MSTVOL, reg);

  //DBG("MasterVolume:%d, 0x:%X,  Reg:0x%X\n\r", volume, volume, reg);
}


void AD820xx_SetMasterMute(unsigned char mute)
{
  AD820_STAT_CTRL3 REG;
  REG.Byte = i2c->readByte(AD820_REG_STAT_CTRL3);

  REG.bMUTE = mute ? 1 : 0;
  i2c->writeByte(AD820_REG_STAT_CTRL3, REG.Byte);

  DBG("MasterMute:%d, 0x:%X", REG.Byte, REG.Byte);
}

void AD820xx_SetCh1Volume(unsigned char volume)
{
  unsigned char reg = volume;
  i2c->writeByte(AD820_REG_CH1VOL, reg);

  DBG("Ch1Volume:%d, 0x:%X", reg, reg);
}


void AD820xx_SetCh1Mute(unsigned char mute)
{
  AD820_STAT_CTRL3 REG;
  REG.Byte = i2c->readByte(AD820_REG_STAT_CTRL3);

  REG.bCM1 = mute ? 1 : 0;
  i2c->writeByte(AD820_REG_STAT_CTRL3, REG.Byte);

  DBG("Ch1Mute:%d, 0x:%X", REG.Byte, REG.Byte);
}


void AD820xx_SetCh2Volume(unsigned char volume)
{
  unsigned char reg = volume;
  i2c->writeByte(AD820_REG_CH2VOL, reg);

  DBG("Ch1Volume:%d, 0x:%X", reg, reg);
}


void AD820xx_SetCh2Mute(unsigned char mute)
{
  AD820_STAT_CTRL3 REG;
  REG.Byte = i2c->readByte(AD820_REG_STAT_CTRL3);

  REG.bCM2 = mute ? 1 : 0;
  i2c->writeByte(AD820_REG_STAT_CTRL3, REG.Byte);

  DBG("Ch2Mute:%d, 0x:%X", REG.Byte, REG.Byte);
}


void AD820xx_CH1MixingEnable(unsigned char enable)
{
  AD820_STAT_CTRL4 REG;
  REG.Byte = i2c->readByte(AD820_REG_STAT_CTRL4);

  REG.bC1MX_EN = enable ? 1 : 0;
  i2c->writeByte(AD820_REG_STAT_CTRL4, REG.Byte);
  DBG("CH1MixingEnable:%d \n\r", REG.bC1MX_EN);
}


void AD820xx_CH2MixingEnable(unsigned char enable)
{
  AD820_STAT_CTRL4 REG;
  REG.Byte = i2c->readByte(AD820_REG_STAT_CTRL4);

  REG.bC2MX_EN = enable ? 1 : 0;
  i2c->writeByte(AD820_REG_STAT_CTRL4, REG.Byte);
  DBG("CH2MixingEnable:%d \n\r", REG.bC2MX_EN);
}


void AD820xx_SoftwareReset(void)
{
    AD820_STAT_CTRL3 REG;
    REG.Byte = i2c->readByte(AD820_REG_STAT_CTRL3);

    REG.bSW_RSTB = 1;
    i2c->writeByte(AD820_REG_STAT_CTRL3, REG.Byte);
    DBG("AD820xx_SoftwareReset!!\n\r");
}

void AD820xx_SetMonoEnable(unsigned char enable)
{
  AD820_STAT_CTRL4 REG;
  //i2c_reg_read(DEV_ADDR, AD820_REG_STAT_CTRL4, 1, &REG.Byte);
  REG.Byte = i2c->readByte(AD820_REG_STAT_CTRL4);

  REG.bMONO_EN = enable ? 1 : 0;
  i2c->writeByte(AD820_REG_STAT_CTRL4, REG.Byte);
  DBG("SetMonoEnable:%d \n\r", REG.bMONO_EN);
}


void AD820xx_FaultPinOut(void)
{
  //LL_GPIO_SetPinMode(AMP_FAULT_GPIO_Port, AMP_FAULT_Pin, LL_GPIO_MODE_OUTPUT);
}


void AD820xx_FaultPinIn(void)
{
  //LL_GPIO_SetPinMode(AMP_FAULT_GPIO_Port, AMP_FAULT_Pin, LL_GPIO_MODE_INPUT);
}


unsigned char TestRead;
unsigned char TestWrite = 50;
AD820_STAT_CTRL2 REG2;
AD820_STAT_CTRL2 REG7;

void AD820xx_Init()
{
    i2c = new I2cPort(DEV_ADDR, 2);
    i2c->openConnection();

    if(!i2c->isConnectionOpen())
    {
        DBG("NG---------i2c connection error----------\n");
    }
}


#endif
/******************* (C) COPYRIGHT Anam Electronics. *****END OF FILE*******************/
