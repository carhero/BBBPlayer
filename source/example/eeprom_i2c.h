/*
 * eeprom_i2c.h
 *
 *  Created on: May 13, 2019
 *      Author: Administrator
 */

#ifndef SOURCE_EXAMPLE_EEPROM_I2C_H_
#define SOURCE_EXAMPLE_EEPROM_I2C_H_

int EEPROM_I2CInit(void);
void EEPROM_I2CExit(void);
void EEPROM_WriteData(unsigned char dev_addr, unsigned short reg_addr, unsigned char *data, unsigned char length);
unsigned char EEPROM_GetData(unsigned int RegAddr);
unsigned char EEPROM_WriteBufData(unsigned int RegAddr, unsigned char *buf, unsigned int length);
unsigned char EEPROM_ReadBufData(unsigned int RegAddr, unsigned char *buf, unsigned int length);

#endif /* SOURCE_EXAMPLE_EEPROM_I2C_H_ */
