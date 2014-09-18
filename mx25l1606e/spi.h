/* ---------------------------------- File Description --------------------------------------------
【文件功能】：    
【创建时间】：2010年6月3日星期五14:22:30
【文件版本】：V1.0
【作者声明】：本文件作者为温业中,可通过email与作者联系: eric540@163.com
------------------------------------------------------------------------------------------ */
#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"	 
#include "serial_debug.h"

#define SPI_CS_Low()     GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_CS_High()    GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define DUMMY_BYTE  0XFF

void SPI1_Configuration(void);
u8 SPI_Write(u8 byte);
u8 SPI_Read(void);

#endif
