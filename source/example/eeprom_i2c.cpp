/*
 * eeprom_i2c.cpp
 *
 *  Created on: May 13, 2019
 *      Author: Administrator
 */

#include <string.h>
#include "../drivers/i2c/I2cPort.h"
#include <iostream>
#include <string.h>

using namespace cacaosd_i2cport;

#define DEV_ADDR    (0xA0>>1)

#define DBG printf
I2cPort *i2c2;

int EEPROM_I2CInit(void)
{
    i2c2 = new I2cPort(DEV_ADDR, 2); // i2c2 is usded as i2c-2 bus line
    i2c2->openConnection();

    if(!i2c2->isConnectionOpen())
    {
        DBG("NG---------i2c connection error----------\n");
        return -1;
    }

    return 0;
}


void EEPROM_I2CExit(void)
{
    i2c2->closeConnection();
}


void EEPROM_WriteData(uint8_t dev_addr, uint16_t reg_addr, uint8_t *data, uint8_t length)
{
    //uint8_t command = 0;    // command
    //uint8_t end_irq = 0;       // end command

    i2c2->writeByteWithReg16(dev_addr, reg_addr, data, length);

    DBG("IA9Q5_SetPairing\n");
}

unsigned char EEPROM_GetData(unsigned int RegAddr)
{
    unsigned char retValue;
    EEPROM_I2CInit();

    DBG("Checking Module...\n");
    //for(int tmpcnt = 0; tmpcnt < 10; tmpcnt += 1)
    {
        retValue = i2c2->readByteWithReg16(DEV_ADDR, RegAddr, 2);
        DBG("REG %X value:%X\n", RegAddr, retValue);
        usleep(10*500);
    }

    EEPROM_I2CExit();

    return retValue;
}

unsigned char EEPROM_WriteBufData(unsigned int RegAddr, unsigned char *buf, unsigned int length)
{
    unsigned char retValue;

    DBG("EEPROM_WriteBufData...\n");
//    i2c2->writeByteWithReg16(DEV_ADDR, RegAddr, buf, length);
    i2c2->writeByteBuffer(RegAddr, buf, length);

    return retValue;
}

unsigned char EEPROM_ReadBufData(unsigned int RegAddr, unsigned char *buf, unsigned int length)
{
    unsigned char retValue;
    //EEPROM_I2CInit();

    DBG("EEPROM_ReadBufData...\n");
    //i2c2->readBufferWithReg16(DEV_ADDR, RegAddr, buf, length);
    i2c2->readByteBuffer(RegAddr, buf, length);

    return retValue;
}

