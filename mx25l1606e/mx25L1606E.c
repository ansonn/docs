/* ---------------------------------- File Description --------------------------------------------
【文件功能】：本文件为 MAX25L1606E  SpiFLASH芯片驱动文件
              MAX25L1606E 存储量为16M bit  分为512个扇区，每个扇区有4K byte  16个扇区为一个block 总共有32个block
              每page 为256个字节
              最快访问速度高达86MHz
【创建时间】：2012年10月22日星期一13:20:40
【文件版本】：V1.0
【作者声明】：本文件作者为温业中,可通过email与作者联系: eric540@163.com
------------------------------------------------------------------------------------------ */
#include "stm32f10x.h"
#include "mx25L1606E.h"
#include "serial_debug.h"


void SpiFlash_Init(void)
{
    flashInfoTypedef flashInfo;   
    //WP
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    FLASH_WP_High();
    SPI1_Configuration();
    
    FlashGet_ElectronicInfo(&flashInfo);
//	    printf("ManufacturerID=%02X\r\n",flashInfo.ManufacturerID);
//	    printf("DeviceID=%02X\r\n",flashInfo.DeviceID[0]);
    if((flashInfo.ManufacturerID==0xc2)&&(flashInfo.DeviceID[0]==0x14))
    {
        printf("\tFlash Info: MXIC(Macronix International Co.,Ltd\r\n");
    }
    FlashGet_Info(&flashInfo);
//	    printf("ManufacturerID=%02X\r\n",flashInfo.ManufacturerID);
//	    printf("DeviceID=%02X%02X\r\n",flashInfo.DeviceID[0],flashInfo.DeviceID[1]);
    if((flashInfo.DeviceID[0]==0x20)&&(flashInfo.DeviceID[1]==0x15))
    {
        printf("\tFlashType:MX25L1606E  FlashSize:16Mbit\r\n\tTotal 32 Blocks  16 Sectors/Block  4KB/Sector\r\n");
    }

#if 0    
    FlashEnter_DeepPowerDown();
    tmp=FlashReleaseFrom_DeepPowerDownE();
    printf("ElectronicSignature=%02x\r\n",tmp);  //打印结果为: 0x14
#endif    
}

void FlashWait_Busy(void)
{
    u8 flashSta;
    u32 retry=0;
    FLASH_CS_Low();
    SPI_Write(RDSD_CMD);
    do{
        
        flashSta=SPI_Read();
        if(retry++>FLASH_BUSY_TIMEOUT)
            break;
    }while(flashSta&0x80);
    //printf("%d\r\n",retry);
    SPI_CS_High();    
}
void FlashGet_Info(flashInfoTypedef * pFlashInfo)
{
//	    FlashWait_Busy();
    FLASH_CS_Low();
    SPI_Write(RDID_CMD);
    pFlashInfo->ManufacturerID=SPI_Read();
    pFlashInfo->DeviceID[0]=SPI_Read();
    pFlashInfo->DeviceID[1]=SPI_Read();
    SPI_CS_High();    
}
void FlashGet_ElectronicInfo(flashInfoTypedef * pFlashInfo)
{
//	    FlashWait_Busy();
    FLASH_CS_Low();
    SPI_Write(REMS_CMD);
    SPI_Write(DUMMY_BYTE);
    SPI_Write(DUMMY_BYTE);
    SPI_Write(0x00);  //manufacturer's ID first
    pFlashInfo->ManufacturerID=SPI_Read();
    pFlashInfo->DeviceID[0]=SPI_Read();   
    SPI_CS_High();    
}
void FlashEnter_DeepPowerDown(void)
{
    FLASH_CS_Low();
    SPI_Write(DP_CMD);   
    SPI_CS_High();    
}
void FlashReleaseFrom_DeepPowerDown(void)
{
    FLASH_CS_Low();
    SPI_Write(RDP_CMD);   
    SPI_CS_High();    
}
u8 FlashReleaseFrom_DeepPowerDownE(void)
{   u8 ElectronicSignature;
    FLASH_CS_Low();
    SPI_Write(RES_CMD);
    SPI_Write(DUMMY_BYTE);
    SPI_Write(DUMMY_BYTE);
    SPI_Write(DUMMY_BYTE);
    ElectronicSignature=SPI_Read();
    SPI_CS_High();    
    return ElectronicSignature;
}
void FlashWriteEnable(void)
{
    FLASH_CS_Low();
    SPI_Write(WREN_CMD);   
    SPI_CS_High();    
}
void FlashWriteDisable(void)
{
    FLASH_CS_Low();
    SPI_Write(WRDI_CMD);   
    SPI_CS_High();    
}
void FlashWrite_StaRegister(u8 flashSta)
{
    FLASH_CS_Low();
    SPI_Write(WRSR_CMD); 
    SPI_Write(flashSta); 
    SPI_CS_High();    
}
/*---- F L A S H   P A G E _   W R I T E ----
【功能】：****
【参数】：pageAddr:页地址，在函数内再乘以页大小，即为要传入的24bit地址。  页范围(0~8191) 
          *ptr:     指向要写的内容的首址。
          pageSize: 页的大小，一般填 FLASH_PAGE_SIZE 
【返回】：****
【说明】：由于此flash 接受引命令的地址位为24bit 如果低8位不为全0时，超过出页地址的范围内容会覆盖页起始范围内容
          如果小于页范围，及页后面原来存储的内容不会改变。
--------------作者:温业中   2012年10月23日星期二13:52:53--------------------------------*/
void FlashPage_Write(u16 pageAddr,u8 *ptr,u32 pageSize)
{
    u32 addr,i;
//	    addr=pageAddr*FLASH_PAGE_SIZE;
    addr=((u32)pageSize)<<8;
    FlashWait_Busy();
    FlashWriteEnable();
    FLASH_CS_Low();
    SPI_Write(PP_CMD); 
    SPI_Write(addr>>16);
    SPI_Write(addr>>8);
    SPI_Write(addr); 
    for(i=0;i<pageSize;i++)
        SPI_Write(*ptr++); 
    SPI_CS_High();    
}
void FlashChip_Erase(void)
{
    FlashWait_Busy();
    FlashWriteEnable();
    FlashWriteEnable();
    FLASH_CS_Low();
    SPI_Write(CE_CMD);     
    SPI_CS_High();    
}
/*---- F L A S H   B L O C K _   E R A S E ----
【功能】：****
【参数】：block: 0~31
【返回】：****
【说明】：****
--------------作者:温业中   2012年10月23日星期二14:43:05--------------------------------*/
void FlashBlock_Erase(u8 blockAddr)
{
    u32 addr;//,i;
//	    addr=blockAddr*16*4096;  //每个block有16个扇区，每个扇区有4KByte;
    addr=((u32)blockAddr)<<16;
    FlashWait_Busy();
    FlashWriteEnable();
    FLASH_CS_Low();
    SPI_Write(BE_CMD); 
    SPI_Write(addr>>16);
    SPI_Write(addr>>8);
    SPI_Write(addr); 
    SPI_CS_High();    
}
/*---- F L A S H   S E C T O R _   E R A S E ----
【功能】：****
【参数】：sectorAddr: 0~511
【返回】：****
【说明】：****
--------------作者:温业中   2012年10月23日星期二14:45:28--------------------------------*/
void FlashSector_Erase(u8 sectorAddr)
{
    u32 addr;//,i;
    addr=((u32)sectorAddr)<<12;
    FlashWait_Busy();
    FlashWriteEnable();
    FLASH_CS_Low();
    SPI_Write(SE_CMD); 
    SPI_Write(addr>>16);
    SPI_Write(addr>>8);
    SPI_Write(addr); 
    SPI_CS_High();    
}
void FlashRead(u32 addr,u8 *ptr,u32 len)
{
    u32 addr_tmp,i;
    addr_tmp=addr&0x001fffff;
    FlashWait_Busy();    
    FLASH_CS_Low();
    SPI_Write(READ_CMD); 
    SPI_Write(addr_tmp>>16);
    SPI_Write(addr_tmp>>8);
    SPI_Write(addr_tmp); 
    for(i=0;i<len;i++)
        *ptr++=SPI_Read(); 
    SPI_CS_High();    
}
/*---- F L A S H   P A G E _   R E A D ----
【功能】：****
【参数】：pageAddr:页地址，在函数内再乘以页大小，即为要传入的24bit地址。  页范围(0~8191) 
【返回】：****
【说明】：****
--------------作者:温业中   2012年10月23日星期二15:11:50--------------------------------*/
void FlashPage_Read(u16 pageAddr,u8 *ptr,u32 pageSize)
{
    u32 addr,i;
//	    addr=pageAddr*FLASH_PAGE_SIZE;
    addr=((u32)pageSize)<<8;
    FlashWait_Busy();    
    FLASH_CS_Low();
    SPI_Write(READ_CMD); 
    SPI_Write(addr>>16);
    SPI_Write(addr>>8);
    SPI_Write(addr); 
    for(i=0;i<pageSize;i++)
        *ptr++=SPI_Read(); 
    SPI_CS_High();    
}
void FlashFastRead(u32 addr,u8 *ptr,u32 len)
{
    u32 addr_tmp,i;
    addr_tmp=addr&0x001fffff;
    FlashWait_Busy();    
    FLASH_CS_Low();
    SPI_Write(FREAD_CMD); 
    SPI_Write(addr_tmp>>16);
    SPI_Write(addr_tmp>>8);
    SPI_Write(addr_tmp); 
    SPI_Write(DUMMY_BYTE); 
    for(i=0;i<len;i++)
        *ptr++=SPI_Read(); 
    SPI_CS_High();    
}
#if 0
void FlashDoubleRead(u32 addr,u8 *ptr,u32 len)
{
    u32 addr_tmp,i;
    addr_tmp=addr&0x001fffff;
    FlashWait_Busy();    
    FLASH_CS_Low();
    SPI_Write(DREAD_CMD); 
    SPI_Write(addr_tmp>>16);
    SPI_Write(addr_tmp>>8);
    SPI_Write(addr_tmp); 
    SPI_Write(DUMMY_BYTE); 
    for(i=0;i<len;i++)
    {
        //读双线数据,并要组合
        //*ptr++=SPI_Read(); 
    }
    SPI_CS_High();    
}
#endif
