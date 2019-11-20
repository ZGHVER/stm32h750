#include "gt9147.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "delay.h" 
#include "string.h" 
#include "tftlcd.h" 
#include "GUI_Type.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//4.3寸电容触摸屏-GT9147 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved		 
////////////////////////////////////////////////////////////////////////////////// 
void I2C_GTP_IRQEnable(void);

//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
const u8 GT9147_CFG_TBL[]=
{ 
	0x00,0x20,0x03,0xE0,0x01,0x05,0x3C,0x00,0x01,0x08,
	0x28,0x0C,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x17,0x19,0x1E,0x14,0x8B,0x2B,0x0D,
	0x33,0x35,0x0C,0x08,0x00,0x00,0x00,0x9A,0x03,0x11,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
	0x00,0x20,0x58,0x94,0xC5,0x02,0x00,0x00,0x00,0x04,
	0xB0,0x23,0x00,0x93,0x2B,0x00,0x7B,0x35,0x00,0x69,
	0x41,0x00,0x5B,0x4F,0x00,0x5B,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
	0x12,0x14,0x16,0x18,0x1A,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,
	0x10,0x12,0x13,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,
	0x21,0x22,0x24,0x26,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x48,0x01
};  
//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
u8 GT9147_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
	buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//计算校验和
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(u8*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
	return 0;
} 
//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件    
} 
//初始化GT9147触摸屏
//返回值:0,初始化成功;1,初始化失败 
u8 GT9147_Init(void)
{
	u8 temp[5]; 
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
                
		//PD13:INT
    GPIO_Initure.Pin=GPIO_PIN_13;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
            
		//PI8:RST
    GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化
	
		CT_IIC_Init();      	//初始化电容屏的I2C总线  
		GT_RST(0);				//复位
		delay_ms(10);
		GT_RST(1);				//释放复位		    
		delay_ms(10); 
		
    GPIO_Initure.Pin=GPIO_PIN_13;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉，浮空输入
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
		/*RST和INT上电时序决定IIC地址*/
		
//		//中断
//  I2C_GTP_IRQEnable();
	
	
	delay_ms(100);  
	GT9147_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
	temp[4]=0;
	printf("CTP ID:%s\r\n",temp);	//打印ID
	if(strcmp((char*)temp,"9157")==0)//ID==9147
	{
		temp[0]=0X02;			
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT9147
		GT9147_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
		if(temp[0]<0X60)//默认版本比较低,需要更新flash配置
		{
			printf("Default Ver:%d\r\n",temp[0]);
           // if(lcddev.id==0X5510)
							GT9147_Send_Cfg(1);//更新并保存配置
		}
		delay_ms(10);
		temp[0]=0X00;	 
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//结束复位   
		return 0;
	} 
	return 1;
}
const u16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};


//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 GT9147_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
 	static u8 t=0;//控制查询间隔,从而降低CPU占用率   
	if(GT_INT==0)
	{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志 	：退出	
		return 0;
	}
//	t++;
//	if((t%5)==0||t<5)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
//	{
		GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//读取触摸点的状态  
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志 	：退出	
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tempsta=tp_dev.sta;			//保存当前的tp_dev.sta值
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//保存触点0的数据
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
//                    if(lcddev.id==0X5510)   //4.3寸800*480 MCU屏
//                    {
//                        if(tp_dev.touchtype&0X01)//横屏
//                        {
//                            tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
//                            tp_dev.x[i]=(((u16)buf[3]<<8)+buf[2]);
//                        }else
//                        {
                            tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
                            tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
//                        }  
//                    }else if(lcddev.id==0X4342) //4.3寸480*272 RGB屏
//                    {
//                        if(tp_dev.touchtype&0X01)//横屏
//                        {
//                            tp_dev.x[i]=(((u16)buf[1]<<8)+buf[0]);
//                            tp_dev.y[i]=(((u16)buf[3]<<8)+buf[2]);
//                        }else
//                        {
//                            tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
//                            tp_dev.x[i]=272-(((u16)buf[3]<<8)+buf[2]);
//                        }
//                    }
				}			
			}  
			res=1;
			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//非法数据(坐标超出了)
			{ 
				if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
				}else					//非法数据,则忽略此次数据(还原原来的)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//恢复tp_dev.sta
				}
			}else t=0;					//触发一次,则会最少连续监测10次,从而提高命中率
		}
//	}
	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=5;//重新从10开始计数
	return res;
}
 
 /**
  * @brief  配置 PB7 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
//void I2C_GTP_IRQEnable(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure; 

//    /*开启按键GPIO口的时钟*/
//    GTP_INT_GPIO_CLK_ENABLE();

//    /* 选择中断引脚 */ 
//    GPIO_InitStructure.Pin = GTP_INT_GPIO_PIN;
//    /* 设置引脚为输入模式 */ 
//    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	    		
//    /* 设置引脚不上拉也不下拉 */
//    GPIO_InitStructure.Pull = GPIO_NOPULL;
//    /* 使用上面的结构体初始化按键 */
//    HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure); 
//    /* 配置中断优先级 */
//    HAL_NVIC_SetPriority(GTP_INT_EXTI_IRQ, 1, 1);
//    /* 使能中断 */
//    HAL_NVIC_EnableIRQ(GTP_INT_EXTI_IRQ);

//}
////检测到触摸中断时调用，
//void GTP_TouchProcess(void)
//{
//	GT9147_Scan(0);
//}
///**
//  * @brief   触屏处理函数，轮询或者在触摸中断调用
//  * @param 无
//  * @retval 无
//  */
//u8 GT9147_Scan(u8 mode)
//{
//    uint8_t  end_cmd[3] = {GT_GSTID_REG >> 8, GT_GSTID_REG & 0xFF, 0};
//    uint8_t  point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1]={GT_GSTID_REG >> 8, GT_GSTID_REG & 0xFF};
//    uint8_t  touch_num = 0;
//    uint8_t  finger = 0;
//    static uint16_t pre_touch = 0;
//    static uint8_t pre_id[GTP_MAX_TOUCH] = {0};

////    uint8_t client_addr=GTP_ADDRESS;
//    uint8_t* coor_data = NULL;
//    int32_t input_x = 0;
//    int32_t input_y = 0;
//    int32_t input_w = 0;
//    uint8_t id = 0;
// 
//    int32_t i  = 0;
//    int32_t ret = -1;
//		GUI_PID_STATE State;

//		
// 	static u8 t=0;//控制查询间隔,从而降低CPU占用率   
//	t++;
//	if(t>0)	
//	{
//	t=0;
//		
//		GT9147_RD_Reg(GT_GSTID_REG,point_data+2,10);//10字节寄存器加2字节地址:前两个字节不是数据
//		
////    if (ret < 0)
////    {
////        return 0;
////    }
//    
//    finger = point_data[2];//状态寄存器数据：索引2才是正常数据的开始

//    if (finger == 0x00)		//没有数据，退出
//    {
//        return 0;
//    }

//    if((finger & 0x80) == 0)//判断buffer status位
//    {
//        goto exit_work_func;//坐标未就绪，数据无效
//    }

//    touch_num = finger & 0x0f;//坐标点数
//    if (touch_num > GTP_MAX_TOUCH)
//    {
//        goto exit_work_func;//大于最大支持点数，错误退出
//    }

//    if (touch_num > 1)//不止一个点
//    {
//        uint8_t buf[8 * GTP_MAX_TOUCH] = {(GT_GSTID_REG + 10) >> 8, (GT_GSTID_REG + 10) & 0xff};

////        ret = GTP_I2C_Read(client_addr, buf, 2 + 8 * (touch_num - 1));
//				GT9147_RD_Reg(GT_GSTID_REG+10,buf+2,8 * (touch_num - 1));//10字节寄存器加2字节地址:前两个字节不是数据
//        memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1));			//复制其余点数的数据到point_data
//    }

//    
//    
//    if (pre_touch>touch_num)				//pre_touch>touch_num,表示有的点释放了
//    {
//        for (i = 0; i < pre_touch; i++)						//一个点一个点处理
//         {
//            uint8_t j;
//           for(j=0; j<touch_num; j++)
//           {
//               coor_data = &point_data[j * 8 + 3];
//               id = coor_data[0] & 0x0F;									//track id
//              if(pre_id[i] == id)
//                break;

//              if(j >= touch_num-1)											//遍历当前所有id都找不到pre_id[i]，表示已释放
//              {
////                 GTP_Touch_Up( pre_id[i]);
//									tp_dev.x[pre_id[i]]=-1;
//									tp_dev.y[pre_id[i]]=-1;
//              }
//           }
//       }
//    }


//    if (touch_num)
//    {
//        for (i = 0; i < touch_num; i++)						//一个点一个点处理
//        {
//            coor_data = &point_data[i * 8 + 3];

//            id = coor_data[0] & 0x0F;									//track id
//            pre_id[i] = id;

//            input_x  = coor_data[1] | (coor_data[2] << 8);	//x坐标
//            input_y  = coor_data[3] | (coor_data[4] << 8);	//y坐标
//            input_w  = coor_data[5] | (coor_data[6] << 8);	//size
//        
//            {
////                GTP_Touch_Down( id, input_x, input_y, input_w);//数据处理
//								tp_dev.x[pre_id[id]]=input_x;
//								tp_dev.y[pre_id[id]]=input_y;
//            }
//        }
//    }
//    else if (pre_touch)		//touch_ num=0 且pre_touch！=0
//    {
//      for(i=0;i<pre_touch;i++)
//      {
////          GTP_Touch_Up(pre_id[i]);
//				tp_dev.x[pre_id[i]]=-1;
//				tp_dev.y[pre_id[i]]=-1;
//				
//      }
//    }


//    pre_touch = touch_num;
//	}

//exit_work_func:
//    {
////        ret = GTP_I2C_Write(client_addr, end_cmd, 3);
//				GT9147_WR_Reg(GT_GSTID_REG,end_cmd+2,3-2);
////        if (ret < 0)
////        {
////            GTP_INFO("I2C write end_cmd error!");
////        }
//    }
//		
////	if((tp_dev.x[0]==-1) || (tp_dev.y[0]==-1))
////	{
////		State.x = -1;
////		State.y = -1;
////		State.Pressed = 0;
////		GUI_PID_StoreState(&State);//释放
////		return 0;
////	}
////	State.Pressed = 1;
////	State.x = tp_dev.x[0];
////	State.y =tp_dev.y[0];
////	GUI_PID_StoreState(&State);
////		
//		
//	return 0;
//}




























