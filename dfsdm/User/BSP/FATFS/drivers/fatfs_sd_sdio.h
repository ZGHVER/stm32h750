#ifndef __STM32H743_FatFs_SD_H
#define __STM32H743_FatFs_SD_H

#include "stm32h7xx.h"
#include <stdio.h>

#include "BSP/FATFS/diskio.h"
#include "BSP/FATFS/integer.h"
#include "BSP/FATFS/ff_gen_drv.h"

DSTATUS SD_initialize(BYTE lu);
DSTATUS SD_status(BYTE lu);
DRESULT SD_read(BYTE lun,//��������������豸ʱ�õ�(0...)
                                     BYTE *buff,//���ݻ����� 
                                     DWORD sector, //�����׵�ַ
                                     UINT count);//��������(1..128)

  
DRESULT SD_write(BYTE lun,//��������������豸ʱ�õ�(0...)
                                      const BYTE *buff,//���ݻ����� 
                                      DWORD sector, //�����׵�ַ
                                      UINT count);//��������(1..128)

DRESULT SD_ioctl(BYTE lun,BYTE cmd, void *buff);


#endif /* __STM32H743_FatFs_SD_H */