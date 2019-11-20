#include "gt9147.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "delay.h" 
#include "string.h" 
#include "tftlcd.h" 
#include "GUI_Type.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//4.3����ݴ�����-GT9147 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved		 
////////////////////////////////////////////////////////////////////////////////// 
void I2C_GTP_IRQEnable(void);

//GT9147���ò�����
//��һ���ֽ�Ϊ�汾��(0X60),���뱣֤�µİ汾�Ŵ��ڵ���GT9147�ڲ�
//flashԭ�а汾��,�Ż��������.
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
//����GT9147���ò���
//mode:0,���������浽flash
//     1,�������浽flash
u8 GT9147_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
	buf[1]=mode;	//�Ƿ�д�뵽GT9147 FLASH?  ���Ƿ���籣��
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//����У���
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(u8*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//���ͼĴ�������
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//д��У���,�����ø��±��
	return 0;
} 
//��GT9147д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
//��GT9147����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //����д���� 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����    
} 
//��ʼ��GT9147������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
u8 GT9147_Init(void)
{
	u8 temp[5]; 
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOH_CLK_ENABLE();			//����GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();			//����GPIOIʱ��
                
		//PD13:INT
    GPIO_Initure.Pin=GPIO_PIN_13;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //��ʼ��
            
		//PI8:RST
    GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��
	
		CT_IIC_Init();      	//��ʼ����������I2C����  
		GT_RST(0);				//��λ
		delay_ms(10);
		GT_RST(1);				//�ͷŸ�λ		    
		delay_ms(10); 
		
    GPIO_Initure.Pin=GPIO_PIN_13;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_NOPULL;          //��������������������
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //��ʼ��
		/*RST��INT�ϵ�ʱ�����IIC��ַ*/
		
//		//�ж�
//  I2C_GTP_IRQEnable();
	
	
	delay_ms(100);  
	GT9147_RD_Reg(GT_PID_REG,temp,4);//��ȡ��ƷID
	temp[4]=0;
	printf("CTP ID:%s\r\n",temp);	//��ӡID
	if(strcmp((char*)temp,"9157")==0)//ID==9147
	{
		temp[0]=0X02;			
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//��λGT9147
		GT9147_RD_Reg(GT_CFGS_REG,temp,1);//��ȡGT_CFGS_REG�Ĵ���
		if(temp[0]<0X60)//Ĭ�ϰ汾�Ƚϵ�,��Ҫ����flash����
		{
			printf("Default Ver:%d\r\n",temp[0]);
           // if(lcddev.id==0X5510)
							GT9147_Send_Cfg(1);//���²���������
		}
		delay_ms(10);
		temp[0]=0X00;	 
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//������λ   
		return 0;
	} 
	return 1;
}
const u16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};


//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 GT9147_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
 	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	if(GT_INT==0)
	{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//���־ 	���˳�	
		return 0;
	}
//	t++;
//	if((t%5)==0||t<5)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
//	{
		GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//��ȡ�������״̬  
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//���־ 	���˳�	
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tempsta=tp_dev.sta;			//���浱ǰ��tp_dev.staֵ
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//���津��0������
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//������Ч?
				{
					GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
//                    if(lcddev.id==0X5510)   //4.3��800*480 MCU��
//                    {
//                        if(tp_dev.touchtype&0X01)//����
//                        {
//                            tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
//                            tp_dev.x[i]=(((u16)buf[3]<<8)+buf[2]);
//                        }else
//                        {
                            tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
                            tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
//                        }  
//                    }else if(lcddev.id==0X4342) //4.3��480*272 RGB��
//                    {
//                        if(tp_dev.touchtype&0X01)//����
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
			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//�Ƿ�����(���곬����)
			{ 
				if((mode&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//����һ��,��������������10��,�Ӷ����������
				}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//�ָ�tp_dev.sta
				}
			}else t=0;					//����һ��,��������������10��,�Ӷ����������
		}
//	}
	if((mode&0X8F)==0X80)//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	 
	} 	
	if(t>240)t=5;//���´�10��ʼ����
	return res;
}
 
 /**
  * @brief  ���� PB7 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
//void I2C_GTP_IRQEnable(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure; 

//    /*��������GPIO�ڵ�ʱ��*/
//    GTP_INT_GPIO_CLK_ENABLE();

//    /* ѡ���ж����� */ 
//    GPIO_InitStructure.Pin = GTP_INT_GPIO_PIN;
//    /* ��������Ϊ����ģʽ */ 
//    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	    		
//    /* �������Ų�����Ҳ������ */
//    GPIO_InitStructure.Pull = GPIO_NOPULL;
//    /* ʹ������Ľṹ���ʼ������ */
//    HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure); 
//    /* �����ж����ȼ� */
//    HAL_NVIC_SetPriority(GTP_INT_EXTI_IRQ, 1, 1);
//    /* ʹ���ж� */
//    HAL_NVIC_EnableIRQ(GTP_INT_EXTI_IRQ);

//}
////��⵽�����ж�ʱ���ã�
//void GTP_TouchProcess(void)
//{
//	GT9147_Scan(0);
//}
///**
//  * @brief   ��������������ѯ�����ڴ����жϵ���
//  * @param ��
//  * @retval ��
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
// 	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
//	t++;
//	if(t>0)	
//	{
//	t=0;
//		
//		GT9147_RD_Reg(GT_GSTID_REG,point_data+2,10);//10�ֽڼĴ�����2�ֽڵ�ַ:ǰ�����ֽڲ�������
//		
////    if (ret < 0)
////    {
////        return 0;
////    }
//    
//    finger = point_data[2];//״̬�Ĵ������ݣ�����2�����������ݵĿ�ʼ

//    if (finger == 0x00)		//û�����ݣ��˳�
//    {
//        return 0;
//    }

//    if((finger & 0x80) == 0)//�ж�buffer statusλ
//    {
//        goto exit_work_func;//����δ������������Ч
//    }

//    touch_num = finger & 0x0f;//�������
//    if (touch_num > GTP_MAX_TOUCH)
//    {
//        goto exit_work_func;//�������֧�ֵ����������˳�
//    }

//    if (touch_num > 1)//��ֹһ����
//    {
//        uint8_t buf[8 * GTP_MAX_TOUCH] = {(GT_GSTID_REG + 10) >> 8, (GT_GSTID_REG + 10) & 0xff};

////        ret = GTP_I2C_Read(client_addr, buf, 2 + 8 * (touch_num - 1));
//				GT9147_RD_Reg(GT_GSTID_REG+10,buf+2,8 * (touch_num - 1));//10�ֽڼĴ�����2�ֽڵ�ַ:ǰ�����ֽڲ�������
//        memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1));			//����������������ݵ�point_data
//    }

//    
//    
//    if (pre_touch>touch_num)				//pre_touch>touch_num,��ʾ�еĵ��ͷ���
//    {
//        for (i = 0; i < pre_touch; i++)						//һ����һ���㴦��
//         {
//            uint8_t j;
//           for(j=0; j<touch_num; j++)
//           {
//               coor_data = &point_data[j * 8 + 3];
//               id = coor_data[0] & 0x0F;									//track id
//              if(pre_id[i] == id)
//                break;

//              if(j >= touch_num-1)											//������ǰ����id���Ҳ���pre_id[i]����ʾ���ͷ�
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
//        for (i = 0; i < touch_num; i++)						//һ����һ���㴦��
//        {
//            coor_data = &point_data[i * 8 + 3];

//            id = coor_data[0] & 0x0F;									//track id
//            pre_id[i] = id;

//            input_x  = coor_data[1] | (coor_data[2] << 8);	//x����
//            input_y  = coor_data[3] | (coor_data[4] << 8);	//y����
//            input_w  = coor_data[5] | (coor_data[6] << 8);	//size
//        
//            {
////                GTP_Touch_Down( id, input_x, input_y, input_w);//���ݴ���
//								tp_dev.x[pre_id[id]]=input_x;
//								tp_dev.y[pre_id[id]]=input_y;
//            }
//        }
//    }
//    else if (pre_touch)		//touch_ num=0 ��pre_touch��=0
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
////		GUI_PID_StoreState(&State);//�ͷ�
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




























