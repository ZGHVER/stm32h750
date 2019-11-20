#include "DIALOG.h"
#include "ButtonUse.h"
#include "led.h"
#include <string.h>
#include "includes.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmbk_4;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemWin BUTTONʹ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/19
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
int timerKtId = 3;
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
unsigned int timer_cnt = 0;
//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = 
{
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 300, 122, 150, 50, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 300, 251, 150, 50, 0, 0x0, 0 },
};

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	char buf[40];
	int     NCode;
	int     Id;
    static u8 beepsta=1;
    
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			
		//������ʱ����ˢ��ʱ��
//		WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	      
			WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 1000, 0);
			GUI_SetBkColor(GUI_WHITE);
				GUI_Clear();
			//��ʼ���Ի���
			hItem = pMsg->hWin;
			FRAMEWIN_SetTitleHeight(hItem, 30);
			FRAMEWIN_SetText(hItem, "emwin button");
			FRAMEWIN_SetFont(hItem, GUI_FONT_24_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetTextColor(hItem, 0xFF);
		
			//��ʼ��BUTTON0
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
			BUTTON_SetText(hItem, "BTN1");
			
			//��ʼ��BUTTON1
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetText(hItem, "BTN2");
			BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
			break;
					
		case WM_TIMER:
			//timer_cnt++;
			//����ӣ���Ȼ��ˢ��
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 1000);
			break;

		case WM_PAINT:
			GUI_SetBkColor(GUI_WHITE);
				GUI_Clear();
			GUI_DrawBitmap(&bmbk_4,0,0);
			GUI_DrawBitmap(&bmbk_4,0,240);
//			//��ʼ��BUTTON0
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
//			BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
//			BUTTON_SetText(hItem, "LED3");
//			
//			//��ʼ��BUTTON1
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//			BUTTON_SetText(hItem, "BEEP2");
//			BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
//		
		
			GUI_SetColor(GUI_BLUE);
			GUI_SetFont(GUI_FONT_24_1);
			//emwin�ڴ�ռ��
			#if 1
				sprintf(buf, "emWin: %uKB/8MB", GUI_ALLOC_GetNumUsedBytes()>>10);
			#else
				sprintf(buf, "emWin: %dB", (int)GUI_ALLOC_GetNumFreeBytes());
			#endif
			GUI_DispStringAt(buf, 100, 50);
		
			//cpuռ��
			
      sprintf(buf,"CPU:%02d%%,CNT:%d",(uint8_t)(OSStatTaskCPUUsage/100),timer_cnt);
			GUI_DispStringAt(buf, 100, 90);
			


			break;
		
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_0: //BUTTON_0��֪ͨ���룬����LED1
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED: //��ť�����²��ͷ�
							LED1_Toggle;		//LED1��ת
						timer_cnt++;
							break;	
					}
					break;
				case ID_BUTTON_1: //BUTTON_1��֪ͨ���룬����BEEP
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							timer_cnt--;
							break;
					}
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

static void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_SetBkColor(0xff);
			GUI_SetColor(0x00);
			GUI_Clear();
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
//����һ���Ի���
WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin;
	
//	WMHTIMER hTime;
	hWin = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), _cbDialog, WM_HBKWIN, 0, 0);

	return hWin;
}

//BUTTO��ʾ����
void ButtonUse_Demo(void)
{
	WM_HWIN hWin;
		//?????? 
		WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	hWin=CreateFramewin();
	while(1)
	{
		GUI_Delay(100);
	}
}

