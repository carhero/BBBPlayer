/**************** (C) Anam Electronics. *********************************************
PROJECT					: VSR-50/70/130
Microcontroller			: STM32F401RB ( 32bit, 64pin, 128K(ROM), 64K(RAM) )
COMPILER				: IAR-EWARM 7.7, J-Link-ARM
MODULE					: AD820xx.h
AUTHOR					: ANAM ELECTRONICS SOFT TEAM
CREATION DATE           : 2017. 07. 26
DESCRIPTION				: AD820xx Ditigal AMP
*******************************************************************************/
#ifndef __AD820xx_H__
#define __AD820xx_H__

/* -------------------- Register Define -------------------- */
#define AD820_REG_STAT_CTRL1			0x00
#define AD820_REG_STAT_CTRL2			0x01
#define AD820_REG_STAT_CTRL3			0x02
#define AD820_REG_MSTVOL					0x03
#define AD820_REG_CH1VOL					0x04
#define AD820_REG_CH2VOL					0x05
#define AD820_REG_VOL_SEL					0x06
#define AD820_REG_STAT_CTRL4			0x07
#define AD820_REG_AR_DRC					0x08  // 0X08 : Attack rate and Release rate for Dynamic Range Control (DRC)
#define AD820_REG_T5BIT_DRC				0x10  // 0X10 : Top 5 bits of attack threshold for Dynamic Range Control (DRC)
#define AD820_REG_M8BIT_AT				0x11  // 0X11 : Middle 8 bits of attack threshold
#define AD820_REG_B8BIT_AT				0x12  // 0X12 : Bottom 8 bits of attack threshold
#define AD820_REG_TPWR_CLIP			0x13  // 0X13 : Top 8 bits of power clipping
#define AD820_REG_MPWR_CLIP			0x14  // 0X14 : Middle 8 bits of power clipping
#define AD820_REG_BPWR_CLIP			0x15  // 0X15 : Bottom 8 bits of power clipping level
#define AD820_REG_STAT_CTRL5			0x16  // 0X16 : State control 5
#define AD820_REG_VOL_TUNE				0x17  // 0X17 : Volume fine tune
#define AD820_REG_DTC							0x18  // 0X18 : Dynamic Temperature Control (DTC)
#define AD820_REG_T8BIT_NGAL			0x1A  // 0X1A : Top 8 bits of noise gate attack level
#define AD820_REG_M8BIT_NGAL			0x1B  // 0X1B : Middle 8 bits of noise gate attack level
#define AD820_REG_B8BIT_NGAL			0x0C  // 0X1C : Bottom 8 bits of noise gate attack level
#define AD820_REG_T8BIT_NGRL			0x1D  // 0X1D : Top 8 bits of noise gate release level
#define AD820_REG_M8BIT_NGRL			0x1E  // 0X1E : Middle 8 bits of noise gate release level
#define AD820_REG_B8BIT_NGRL			0x1F  // 0X1F : Bottom 8 bits of noise gate release level
#define AD820_REG_T8BIT_DRC				0x20  // 0X20 : Top 8 bits of DRC energy coefficient
#define AD820_REG_B8BIT_DRC				0x21  // 0X21 : Bottom 8 bits of DRC energy coefficient
#define AD820_REG_T8BIT_RTDRC			0x22  // 0X22 : Top 8 bits of release threshold for Dynamic Range Control (DRC)
#define AD820_REG_M8BIT_RT				0x23  // 0X23 : Middle 8 bits of release threshold
#define AD820_REG_B8BIT_RT				0x24  // 0X24 : Bottom 8 bits of release threshold


#pragma pack(push, 1)
/* -------------------- Register Bit Structure -------------------- */
typedef union _AD820_STAT_CTRL1
{
    unsigned char Byte;
    struct
    {
        unsigned char bNGE:1;						// Noise Gate Enable
        unsigned char bReserved:3;				// Reserved[3:1]
        unsigned char bLREXC:1;					// Left/Right (L/R) Channel Exchanged
        unsigned char bIF:3;							// Input Format [7:5]
    };
} AD820_STAT_CTRL1, *pAD820_STAT_CTRL1;


typedef union _AD820_STAT_CTRL2
{
    unsigned char Byte;
    struct
    {
        unsigned char PMF:4;							// MCLK/Fs setup when PLL is not bypassed
        unsigned char bFS:2;							// Sampling Frequency
        unsigned char bRev6:1;						// Reserved [6]
        unsigned char bBCLK_SEL:1;				// BCLK System enable
    };
} AD820_STAT_CTRL2, *pAD820_STAT_CTRL2;


typedef union _AD820_STAT_CTRL3
{
    unsigned char Byte;
    struct
    {
        unsigned char bCOMP_EN:1;				// Frequency Compensate filter
        unsigned char bCM2:1;						// Channel 2 Mute
        unsigned char bCM1:1;						// Channel 1 Mute
        unsigned char bMUTE:1;						// Master Mute
        unsigned char bSW_RSTB:1;				// Software reset
        unsigned char bLV_UVSEL:1;				// LV Under Voltage Selection
        unsigned char bHPB:1;						// DC Blocking HPF Bypass
        unsigned char bA_SEL_FAULT:1;		// I2C address selection or FAULT output
    };
} AD820_STAT_CTRL3, *pAD820_STAT_CTRL3;


typedef union _AD820_STAT_CTRL4
{
    unsigned char Byte;
    struct
    {
        unsigned char bRev0:1;						// Reserved
        unsigned char bPL2_EN:1;					// CH2 Power limit enable
        unsigned char bPC2_EN:1;					// CH2 Power Clipping enable
        unsigned char bMONO_EN:1;				// MONO or Stereo configure
        unsigned char bPL1_EN:1;					// CH1 Power limit enable
        unsigned char bPC1_EN:1;					// CH1 Power Clipping enable
        unsigned char bC2MX_EN:1;				// Channel2 Mixing Enable
        unsigned char bC1MX_EN:1;				// Channel1 Mixing Enable
    };
} AD820_STAT_CTRL4, *pAD820_STAT_CTRL4;
#pragma pack(pop)


/* -------------------- Function Define  -------------------- */
void AD820xx_SetMasterVolume(unsigned char volume);
void AD820xx_SetMasterMute(unsigned char mute);
void AD820xx_SetCh1Volume(unsigned char volume);
void AD820xx_SetCh1Mute(unsigned char mute);
void AD820xx_SetCh2Volume(unsigned char volume);
void AD820xx_SetCh2Mute(unsigned char mute);
void AD820xx_CH1MixingEnable(unsigned char enable);
void AD820xx_CH2MixingEnable(unsigned char enable);
void AD820xx_SetMonoEnable(unsigned char enable);
void AD820xx_Init();
void AD820xx_SoftwareReset(void);

#endif
/******************* (C) COPYRIGHT Anam Electronics. *****END OF FILE*******************/
