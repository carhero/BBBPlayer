#ifndef __WM8960_H__
#define __WM8960_H__

//#include "stm32f4xx_hal.h"


#define DMA_MAX_SZE     0xFFFF
#define DMA_MAX(x)      (((x) <= 0xFFFF)? (x):0xFFFF)
#define AUDIODATA_SIZE  2   /* 16-bits audio data size */

#define AUIDO_START_ADDRESS       58 /* Offset relative to audio file header size */


//extern uint32_t AudioRemSize;
//extern uint16_t *CurrentPos;

unsigned char WM8960_Write_Reg(unsigned char reg, unsigned short dat);
unsigned short WM8960_Read_Reg(unsigned char reg);
unsigned char WM89060_Init(void);

//unsigned int AudioFlashPlay(unsigned short* pBuffer, unsigned int FullSize, unsigned int StartAdd);
//unsigned int AUDIO_Play(unsigned short* pBuffer, unsigned int Size);



#endif
