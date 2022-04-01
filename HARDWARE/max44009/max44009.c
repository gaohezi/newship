/*


传感器移植说明:  代码以IO模拟I2C实现传感器采集，需要移植文件 传感器型号对应的.h文件和.c两个文件、
DataType.h(可选) 
			.h文件(如：B_LUX_V20.h等)：包含IO脚的宏定义 和 函数声明, 需要修改IO的宏定义，对应用户使用的IO
			.c文件(如: B_LUX_V20.c等)：包含I2C模拟代码和传感器采集函数, 可能需要修改地方,两个延时函数的修过
			DataType.h							 ：类型定义 (可选)
*/
//***************************************
// B_LUX_V32D_V30采集程序
//****************************************
#include "max44009.h"
//#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "SysTick.h"

//初始化IIC
//void IIC_Init(void)
//{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
//	   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
//	IIC_SCL=1;//PB10
//	IIC_SDA=1;//PB11

//}
 
/*---------------------------------------------------------------------
 功能描述: 起始信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
//void B_LUX_V32D_Start()
//{
//  B_LUX_V32D_SDA0_H;                         //拉高数据线
//  B_LUX_V32D_SCL0_H;                         //拉高时钟线
//  B_LUX_V32D_Delay5us();                     //延时
//  B_LUX_V32D_SDA0_L;                         //产生下降沿
//  B_LUX_V32D_Delay5us();                     //延时
//  B_LUX_V32D_SCL0_L;                         //拉低时钟线
//}
//产生IIC起始信号
void IIC_Start(void)
{
	//SDA_OUT();     //sda线输出
//	IIC_SDA=1;	  	  
//	IIC_SCL=1;
//	delay_us(5);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	delay_us(5);
//	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	 B_LUX_V32D_SDA0_H;                         //拉高数据线
  B_LUX_V32D_SCL0_H;                         //拉高时钟线
  delay_us(5);                      //延时
  B_LUX_V32D_SDA0_L;                         //产生下降沿
  delay_us(5);                     //延时
  B_LUX_V32D_SCL0_L;                         //拉低时钟线
}	 

/*---------------------------------------------------------------------
 功能描述: 停止信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
//void B_LUX_V32D_Stop()
//{
//  B_LUX_V32D_SDA0_L;                         //拉低数据线
//  B_LUX_V32D_SCL0_H;                         //拉高时钟线
//  B_LUX_V32D_Delay5us();                     //延时
//  B_LUX_V32D_SDA0_H;                         //产生上升沿
//  B_LUX_V32D_Delay5us();                     //延时
//  B_LUX_V32D_SCL0_L;
//  B_LUX_V32D_Delay5us();
//}
//产生IIC停止信号
void IIC_Stop(void)
{
//	SDA_OUT();//sda线输出
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//发送I2C总线结束信号
//	delay_us(4);		
  B_LUX_V32D_SDA0_L;                         //拉低数据线
  B_LUX_V32D_SCL0_H;                         //拉高时钟线
 delay_us(5);                      //延时
  B_LUX_V32D_SDA0_H;                         //产生上升沿
  delay_us(5);                     //延时
  B_LUX_V32D_SCL0_L;
  delay_us(5); 	
}
/*---------------------------------------------------------------------
 功能描述: 发送应答信号
 参数说明: ack - 应答信号(0:ACK 1:NAK)
 函数返回: 无
 ---------------------------------------------------------------------*/
void B_LUX_V32D_SendACK(u8 ack)
{
//  if (ack&0x01)	
//			IIC_SDA=1;	//B_LUX_V32D_SDA0_H;		//写应答信号
//  else	
//		  IIC_SDA=0;  //B_LUX_V32D_SDA0_L;
//  
// IIC_SCL=1;				// B_LUX_V32D_SCL0_H;                         //拉高时钟线
//  delay_us(5);                    //延时
// IIC_SCL=0;				// B_LUX_V32D_SCL0_L;                         //拉低时钟线
//  delay_us(5);
// IIC_SDA=1;				// B_LUX_V32D_SDA0_H;
//  delay_us(5);                    //延时
  if (ack&0x01)	B_LUX_V32D_SDA0_H;		//写应答信号
  else	B_LUX_V32D_SDA0_L;
  
  B_LUX_V32D_SCL0_H;                         //拉高时钟线
 delay_us(5);                     //延时
  B_LUX_V32D_SCL0_L;                         //拉低时钟线
 delay_us(5); 
  B_LUX_V32D_SDA0_H;
  delay_us(5);                     //延时
}
  	  
/*---------------------------------------------------------------------
 功能描述: 接收应答信号
 参数说明: 无
 函数返回: 返回应答信号
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_RecvACK()
{
//  u8 CY = 0x00;
//	u16 vConter = 1000;
//  IIC_SDA=1;		//B_LUX_V32D_SDA0_H;
//		
//  SDA_IN();			//B_LUX_V32D_SDA0_I;
//   IIC_SCL=1;		//B_LUX_V32D_SCL0_H;                         //拉高时钟线
//  delay_us(5);                     //延时
//  
//	while (vConter)
//	{
//		vConter--;
//		CY |= B_LUX_V32D_SDA0_DAT;                 //读应答信号
//		
//		if(!CY)	break;
//	}
//  
//  delay_us(5);                     //延时
//  
//  IIC_SCL=0;		//B_LUX_V32D_SCL0_L;                         //拉低时钟线
//  SDA_OUT();		//B_LUX_V32D_SDA0_O;
//  
//  return CY;
  u8 CY = 0x00;
	u16 vConter = 1000;
  B_LUX_V32D_SDA0_H;
  
  B_LUX_V32D_SDA0_I;
  
  B_LUX_V32D_SCL0_H;                         //拉高时钟线
 delay_us(5);                    //延时
  
	while (vConter)
	{
		vConter--;
		CY |= B_LUX_V32D_SDA0_DAT;                 //读应答信号
		
		if(!CY)	break;
	}
  
  delay_us(5);                    //延时
  
  B_LUX_V32D_SCL0_L;                         //拉低时钟线
  B_LUX_V32D_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
 功能描述: 向IIC总线发送一个字节数据
 参数说明: dat - 写字节
 函数返回: 成功返回0  失败返回1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_SendByte(u8 dat)
{
//	u8 vRval = 0;
//  u8 i;
//  
//  for (i=0; i<8; i++)         			//8位计数器
//  {
//    if (dat&0x80)	
//			IIC_SDA=1;//B_LUX_V32D_SDA0_H;
//    else	
//			IIC_SDA=0;//B_LUX_V32D_SDA0_L;                   //送数据口/////////////////////////////////////////////////////////////?SDA与SCL错误
//    
//    delay_us(5);              		//延时
//    IIC_SCL=1;//B_LUX_V32D_SCL0_H;                		//拉高时钟线
//    delay_us(5);             		//延时
//    IIC_SCL=0;//B_LUX_V32D_SCL0_L;                		//拉低时钟线
//   delay_us(5);              		//延时
//    dat <<= 1;              			//移出数据的最高位
//  }
//  
//  vRval = B_LUX_V32D_RecvACK();
//	
//	return vRval;
u8 vRval = 0;
  u8 i;
  
  for (i=0; i<8; i++)         			//8位计数器
  {
    if (dat&0x80)	B_LUX_V32D_SDA0_H;
    else	B_LUX_V32D_SDA0_L;                   //送数据口
    
    delay_us(5);            		//延时
    B_LUX_V32D_SCL0_H;                		//拉高时钟线
   delay_us(5);            		//延时
    B_LUX_V32D_SCL0_L;                		//拉低时钟线
    delay_us(5);              		//延时
    dat <<= 1;              			//移出数据的最高位
  }
  
  vRval = B_LUX_V32D_RecvACK();
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 从IIC总线接收一个字节数据
 参数说明: 无
 函数返回: 接收字节
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_RecvByte()
{
//  u8 i;
//  u8 dat = 0;
//  SDA_IN();//B_LUX_V32D_SDA0_I;
//  
//  IIC_SDA=1;//B_LUX_V32D_SDA0_H;                         //使能内部上拉,准备读取数据,
//  for (i=0; i<8; i++)         	        //8位计数器
//  {
//    IIC_SCL=1;//B_LUX_V32D_SCL0_H;                       //拉高时钟线
//    delay_us(5);             	//延时
//    dat |= B_LUX_V32D_SDA0_DAT;              //读数据               
//    IIC_SCL=0;//B_LUX_V32D_SCL0_L;                       //拉低时钟线
//    delay_us(5);            	//延时
//    
//    if (i<7)
//      dat <<= 1;
//    	
//  }
//  SDA_OUT();//B_LUX_V32D_SDA0_O;
//  
//  return dat;
	u8 i;
  u8 dat = 0;
  B_LUX_V32D_SDA0_I;
  
  B_LUX_V32D_SDA0_H;                         //使能内部上拉,准备读取数据,
  for (i=0; i<8; i++)         	        //8位计数器
  {
    B_LUX_V32D_SCL0_H;                       //拉高时钟线
    delay_us(5);              	//延时
    dat |= B_LUX_V32D_SDA0_DAT;              //读数据               
    B_LUX_V32D_SCL0_L;                       //拉低时钟线
    delay_us(5);             	//延时
    
    if (i<7)
      dat <<= 1;
    	
  }
  B_LUX_V32D_SDA0_O;
  
  return dat;
}

/*---------------------------------------------------------------------
 功能描述: 写MAX44009
 参数说明: REG_Address - 寄存器地址
 函数返回: 成功返回0  失败返回1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_Single_Write(u8 REG_Address, u8 REG_data)
{
	u8 vRval = 0;
	
  IIC_Start();                                        //起始信号
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress);     //发送设备地址+写信号
  vRval += B_LUX_V32D_SendByte(REG_Address);                 //内部寄存器地址，
  vRval += B_LUX_V32D_SendByte(REG_data);                    //内部寄存器数据，
  IIC_Stop();                                         //发送停止信号
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 读MAX44009内部数据
 参数说明: 无
 函数返回: 成功返回0  失败返回1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_Single_Read(u8 REG_Address, u8 *vp_Dat)
{  
  u8 vRval = 0;
	
  IIC_Start();                                        //起始信号
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+0);   //发送设备地址+读信号
  vRval += B_LUX_V32D_SendByte(REG_Address);                 //内部寄存器地址，
  IIC_Start();                                        //起始信号
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+1);   //发送设备地址+读信号
  *vp_Dat = B_LUX_V32D_RecvByte();                           
  B_LUX_V32D_SendACK(1);                                     //回应ACK
  IIC_Stop();                                         //停止信号
  
  return vRval;
  
}

/*---------------------------------------------------------------------
 功能描述: 读MAX44009内部数据
 参数说明: 无
 函数返回: 成功返回0  失败返回1
 ---------------------------------------------------------------------*/
extern u8 B_LUX_V32D_Multi_read(u8 REG_Address1, u8 REG_Address2, u8 *vBuf)
{   	
	u8 vRval = 0;
	
  //寄存器1
  IIC_Start();                                        //起始信号
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+0);   //发送设备地址+读信号
  vRval += B_LUX_V32D_SendByte(REG_Address1);                //内部寄存器地址，
  IIC_Start();                                        //起始信号
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+1);   //发送设备地址+读信号
  vBuf[0] = B_LUX_V32D_RecvByte();                           
  B_LUX_V32D_SendACK(1);

  //连续寄存器2
  IIC_Start();                                        //起始信号
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+0);   //发送设备地址+读信号
  vRval += B_LUX_V32D_SendByte(REG_Address2);                //内部寄存器地址，
  IIC_Start();                                        //起始信号
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+1);   //发送设备地址+读信号
  vBuf[1] = B_LUX_V32D_RecvByte();                           
  B_LUX_V32D_SendACK(1);                                     //回应ACK
  
  IIC_Stop();                                         //停止信号
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 传感器寄存器配置函数
 参数说明: val - 配置参数
 函数返回: 成功返回0  失败返回1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_RegCfg(u8 val)
{
	u8 vRval = 0;
	
  u8 valCfg = 0;
  
  valCfg = (0x00<<7)|(0x00<<6)|(0x00<<3)|(0x00<<0);               //默认800ms采集一次   //配置为自动动模式  //配置积分时间800ms (光照精度0.045Lux  最大光照度 188006.4Lux)                      
  
  if (val&0x80)                //采集方式
  {
    valCfg |= 0x80;
  }
  if (val&0x40)                //采集模式 0-自动模式  1-手动模式
  {
    valCfg |= 0x40;
  }
  if (val&0x08)                //分流比
  {
    valCfg |= 0x08;
  }
  if (val&0x04)                //积分时间
  {
    valCfg |= 0x04;
  }
  if (val&0x02)
  {
    valCfg |= 0x02;
  }
  if (val&0x01)
  {
    valCfg |= 0x01;
  }
  
  vRval += B_LUX_V32D_Single_Write(0x02, valCfg);                               //配置参数
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 初始化光照传感器
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_Init()
{
	u8 vRval = 0;
	
	RCC->APB2ENR |= (0x01<<3);    //使能PORTB 
	RCC->APB2ENR |= (0x01<<4);    //使能PORTC 
	
	
  B_LUX_V32D_SCL0_O;
  SDA_OUT();//B_LUX_V32D_SDA0_O;
	B_LUX_V32D_ADDR_O;
	B_LUX_V32D_ADDR_L;
  IIC_SCL=1;//B_LUX_V32D_SCL0_H;
  IIC_SDA=1;//B_LUX_V32D_SDA0_H;

  delay_ms(100);                                      //延时100ms
  
  vRval = B_LUX_V32D_RegCfg(0x00);
  
	return vRval;
}
/*---------------------------------------------------------------------
 功能描述: 光照读取函数
 参数说明: *vLux - 返回光照值
 函数返回: 成功返回0  失败返回1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_GetLux(u32 *vLux)
{
	u8 	vRval = 0;
	
  u8   vIndex = 0;                     //指数
  u8   vMantissa = 0;                  //尾数
  u8   vLuxBuf[3];
  u16  val16 = 0;
  fp32    vflux = 0.0;
  
  vRval += B_LUX_V32D_Multi_read(0x03, 0x04, vLuxBuf);
  
  //指数
  vIndex = vLuxBuf[0]&0xF0;
  vIndex >>= 4;
  
  //尾数
  vMantissa   = (vLuxBuf[0]&0x0F);
  vMantissa <<= 4;
  vMantissa  |= (vLuxBuf[1]&0x0F);
  
  if (vIndex == 0x01)
  {
    vIndex = 0x00;
  }
  else if (vIndex > 0x0E)
  {
    vIndex = 0x0E;
  }
  
  val16 = (0x01<<vIndex);
  
  vflux = ( (fp32)val16 ) * ( (fp32)vMantissa) * 0.045;
  
  vflux = (vflux*1000.0);                      		 //*1000 小数位
  (*vLux) = (u32)(vflux*1.4);                   //小球透明外壳矫正
  delay_ms(1);                          //延时1s
  
	return vRval;
} 

