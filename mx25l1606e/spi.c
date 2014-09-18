/* ---------------------------------- File Description --------------------------------------------
【文件功能】：    
【创建时间】：2010年6月3日星期五14:22:18
【文件版本】：V1.0
【作者声明】：本文件作者为温业中,可通过email与作者联系: eric540@163.com
------------------------------------------------------------------------------------------ */
#include "spi.h"

/*---- S   P   I 1 _   C O N F I G U R A T I O N ----
【功能】：
【参数】：****
【返回】：****
【说明】：pclk2=72M  spi_clk=pclk2/SPI_InitStructure.SPI_BaudRatePrescaler 
--------------作者:温业中   2010年5月7日星期五09:27:31--------------------------------*/
void SPI1_Configuration(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PA.4 as Output push-pull, used as Chip select */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Deselect the FLASH: Chip Select high */
    SPI_CS_High();

    /* SPI1 configuration */ 
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* Enable SPI1 NSS output for master mode */
    SPI_SSOutputCmd(SPI1, DISABLE);    
    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE); 
}

/*------------------- S P I _ W R I T E --------------------
【功能】：向SPI口写一个字节
【参数】：要写的1字节的数据
【返回】：回环字节 
【说明】：****
--------------作者:温业中   2009年3月20日14:48:22-------------------------*/
u8 SPI_Write(u8 byte)
{
    //等待发送缓冲空
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_SendData(SPI1, byte);  
    //等待接收缓冲接收到数据
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
}

/*------------------- S P I _ R E A D --------------------
【功能】：从SPI读一字节数据
【参数】：无
【返回】：读回的数据
【说明】：****
--------------作者:温业中   2009年3月20日14:49:25-------------------------*/
u8 SPI_Read(void)
{
    return (SPI_Write(DUMMY_BYTE));
}


