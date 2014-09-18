#ifndef MX25L1606E_H
#define MX25L1606E_H
#include "spi.h"

#define FLASH_CS_High  SPI_CS_High
#define FLASH_CS_Low   SPI_CS_Low

#define FLASH_WP_Low()     GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define FLASH_WP_High()    GPIO_SetBits(GPIOC, GPIO_Pin_4)

#define FLASH_BUSY_TIMEOUT  200

#define FLASH_PAGE_SIZE     256

#define RDID_CMD  0X9F
#define RDSD_CMD  0X05
#define WRSR_CMD  0x01
#define REMS_CMD  0X90
#define DP_CMD    0XB9
#define RDP_CMD   0XAB
#define RES_CMD   0XAB
#define PP_CMD    0X02
#define WREN_CMD  0X06
#define WRDI_CMD  0X04
#define CE_CMD    0X60
#define BE_CMD    0X52
#define SE_CMD    0x20
#define READ_CMD  0X03
#define FREAD_CMD 0X0B
#define DREAD_CMD 0X3B
typedef struct
{
    u8 ManufacturerID;
    u8 DeviceID[2];
}flashInfoTypedef;
void SpiFlash_Init(void);
void FlashGet_Info(flashInfoTypedef * pFlashInfo);
void FlashWait_Busy(void);
void FlashGet_ElectronicInfo(flashInfoTypedef * pFlashInfo);
void FlashEnter_DeepPowerDown(void);
void FlashReleaseFrom_DeepPowerDown(void);
u8 FlashReleaseFrom_DeepPowerDownE(void);
#endif
