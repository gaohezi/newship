#ifndef __SHT31_H
#define __SHT31_H
#include "sys.h"
#include "myiic.h"
#include "delay.h"
#include "usart.h"

 
 #define SHT31_ADDR 0x88  //ADDR (pin 2)connected to VDD
void Cmd_Write_sht31(uint8_t msb,uint8_t lsb);
void Read_sht31(void);
void Convert_sht31(void);
 
#endif






