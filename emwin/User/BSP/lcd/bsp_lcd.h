#ifndef __BSP_MLCD_H
#define	__BSP_MLCD_H


/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx.h"
#include "./sdram/bsp_sdram.h"
#include "./fonts/fonts.h"
#include"stm32h7xx_hal_conf.h"
#include"stm32h7xx_hal_ltdc.h"
#include"stm32h7xx_hal_dma2d.h"

/* MLCD 物理像素大小 (宽度和高度) */
#define  MLCD_PIXEL_WIDTH    ((uint16_t)800)
#define  MLCD_PIXEL_HEIGHT   ((uint16_t)480)

/* MLCD 层像素格式*/
#define ARGB8888 	LTDC_PIXEL_FORMAT_ARGB8888  /*!< ARGB8888 LTDC像素格式 */
#define RGB888 		LTDC_PIXEL_FORMAT_RGB888    /*!< RGB888 LTDC像素格式   */
#define RGB565 		LTDC_PIXEL_FORMAT_RGB565    /*!< RGB565 LTDC像素格式   */
#define ARGB1555 	LTDC_PIXEL_FORMAT_ARGB1555  /*!< ARGB1555 LTDC像素格式 */
#define ARGB4444 	LTDC_PIXEL_FORMAT_ARGB4444  /*!< ARGB4444 LTDC像素格式 */

typedef struct 
{ 
  uint32_t TextColor; 
  uint32_t BackColor;  
  sFONT    *pFont;
}MLCD_DrawPropTypeDef;   
   
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint; 

/** 
  * @brief  字体对齐模式  
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /* 居中对齐 */
  RIGHT_MODE              = 0x02,    /* 右对齐   */
  LEFT_MODE               = 0x03     /* 左对齐   */
}Text_AlignModeTypdef;

#define MAX_LAYER_NUMBER       ((uint32_t)2)

#define LTDC_ACTIVE_LAYER	     ((uint32_t)1) /* Layer 1 */
/** 
  * @brief  MLCD status structure definition  
  */     
#define MLCD_OK                 ((uint8_t)0x00)
#define MLCD_ERROR              ((uint8_t)0x01)
#define MLCD_TIMEOUT            ((uint8_t)0x02)

/** 
  * @brief  MLCD FB_StartAddress  
  */
#define MLCD_FB_START_ADDRESS       ((uint32_t)0xD0000000)


/** 
  * @brief  MLCD color  
  */ 
#define MLCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define MLCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define MLCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define MLCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define MLCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define MLCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define MLCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define MLCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define MLCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define MLCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define MLCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define MLCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define MLCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
#define MLCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
#define MLCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define MLCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define MLCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define MLCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define MLCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define MLCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
#define MLCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define MLCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define MLCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define MLCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define MLCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define MLCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)
#define MLCD_COLOR_LUCENCY       ((uint32_t)0x00000000)

/** 
  * @brief MLCD default font 
  */ 
#define MLCD_DEFAULT_FONT        Font24 

//红色数据线
#define LTDC_R0_GPIO_PORT        	GPIOI
#define LTDC_R0_GPIO_CLK_ENABLE()   __GPIOI_CLK_ENABLE()
#define LTDC_R0_GPIO_PIN         	GPIO_PIN_15
#define LTDC_R0_AF			        	GPIO_AF14_LTDC

#define LTDC_R1_GPIO_PORT        	GPIOJ
#define LTDC_R1_GPIO_CLK_ENABLE()   __GPIOJ_CLK_ENABLE()
#define LTDC_R1_GPIO_PIN         	GPIO_PIN_0
#define LTDC_R1_AF			        	GPIO_AF14_LTDC

#define LTDC_R2_GPIO_PORT        	GPIOJ
#define LTDC_R2_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_R2_GPIO_PIN         	GPIO_PIN_1
#define LTDC_R2_AF			        	GPIO_AF14_LTDC

#define LTDC_R3_GPIO_PORT        	GPIOJ
#define LTDC_R3_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_R3_GPIO_PIN         	GPIO_PIN_2
#define LTDC_R3_AF			        	GPIO_AF14_LTDC

#define LTDC_R4_GPIO_PORT        	GPIOJ
#define LTDC_R4_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_R4_GPIO_PIN         	GPIO_PIN_3
#define LTDC_R4_AF			        	GPIO_AF14_LTDC

#define LTDC_R5_GPIO_PORT        	GPIOJ
#define LTDC_R5_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_R5_GPIO_PIN         	GPIO_PIN_4
#define LTDC_R5_AF			        	GPIO_AF14_LTDC

#define LTDC_R6_GPIO_PORT        	GPIOJ
#define LTDC_R6_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_R6_GPIO_PIN         	GPIO_PIN_5
#define LTDC_R6_AF			        	GPIO_AF14_LTDC

#define LTDC_R7_GPIO_PORT        	GPIOJ
#define LTDC_R7_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_R7_GPIO_PIN         	GPIO_PIN_6
#define LTDC_R7_AF			        	GPIO_AF14_LTDC
//绿色数据线
#define LTDC_G0_GPIO_PORT        	GPIOJ
#define LTDC_G0_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_G0_GPIO_PIN         	GPIO_PIN_7
#define LTDC_G0_AF			        	GPIO_AF14_LTDC

#define LTDC_G1_GPIO_PORT        	GPIOJ
#define LTDC_G1_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_G1_GPIO_PIN         	GPIO_PIN_8
#define LTDC_G1_AF			        	GPIO_AF14_LTDC

#define LTDC_G2_GPIO_PORT        	GPIOJ
#define LTDC_G2_GPIO_CLK_ENABLE() 	__GPIOJ_CLK_ENABLE()
#define LTDC_G2_GPIO_PIN         	GPIO_PIN_9
#define LTDC_G2_AF			        	GPIO_AF14_LTDC

#define LTDC_G3_GPIO_PORT        	GPIOJ
#define LTDC_G3_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_G3_GPIO_PIN         	GPIO_PIN_10
#define LTDC_G3_AF			        	GPIO_AF14_LTDC

#define LTDC_G4_GPIO_PORT        	GPIOJ
#define LTDC_G4_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_G4_GPIO_PIN         	GPIO_PIN_11
#define LTDC_G4_AF			        	GPIO_AF14_LTDC

#define LTDC_G5_GPIO_PORT        	GPIOK
#define LTDC_G5_GPIO_CLK_ENABLE()	__GPIOK_CLK_ENABLE()
#define LTDC_G5_GPIO_PIN         	GPIO_PIN_0
#define LTDC_G5_AF			        	GPIO_AF14_LTDC

#define LTDC_G6_GPIO_PORT        	GPIOK
#define LTDC_G6_GPIO_CLK_ENABLE()	__GPIOK_CLK_ENABLE()
#define LTDC_G6_GPIO_PIN         	GPIO_PIN_1
#define LTDC_G6_AF			        	GPIO_AF14_LTDC

#define LTDC_G7_GPIO_PORT        	GPIOK
#define LTDC_G7_GPIO_CLK_ENABLE() 	__GPIOK_CLK_ENABLE()
#define LTDC_G7_GPIO_PIN         	GPIO_PIN_2
#define LTDC_G7_AF			        	GPIO_AF14_LTDC

//蓝色数据线
#define LTDC_B0_GPIO_PORT        	GPIOJ
#define LTDC_B0_GPIO_CLK_ENABLE()  	__GPIOJ_CLK_ENABLE()
#define LTDC_B0_GPIO_PIN         	GPIO_PIN_12
#define LTDC_B0_AF			        	GPIO_AF14_LTDC

#define LTDC_B1_GPIO_PORT        	GPIOJ
#define LTDC_B1_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_B1_GPIO_PIN         	GPIO_PIN_13
#define LTDC_B1_AF			        	GPIO_AF14_LTDC

#define LTDC_B2_GPIO_PORT        	GPIOJ
#define LTDC_B2_GPIO_CLK_ENABLE()	__GPIOJ_CLK_ENABLE()
#define LTDC_B2_GPIO_PIN         	GPIO_PIN_14
#define LTDC_B2_AF			        	GPIO_AF14_LTDC

#define LTDC_B3_GPIO_PORT        	GPIOJ
#define LTDC_B3_GPIO_CLK_ENABLE() 	__GPIOJ_CLK_ENABLE()
#define LTDC_B3_GPIO_PIN         	GPIO_PIN_15
#define LTDC_B3_AF			        	GPIO_AF14_LTDC

#define LTDC_B4_GPIO_PORT        	GPIOK
#define LTDC_B4_GPIO_CLK_ENABLE() 	__GPIOK_CLK_ENABLE()
#define LTDC_B4_GPIO_PIN         	GPIO_PIN_3
#define LTDC_B4_AF			        	GPIO_AF14_LTDC

#define LTDC_B5_GPIO_PORT        	GPIOK
#define LTDC_B5_GPIO_CLK_ENABLE()	__GPIOK_CLK_ENABLE()
#define LTDC_B5_GPIO_PIN         	GPIO_PIN_4
#define LTDC_B5_AF			        	GPIO_AF14_LTDC

#define LTDC_B6_GPIO_PORT        	GPIOK
#define LTDC_B6_GPIO_CLK_ENABLE() 	__GPIOK_CLK_ENABLE()
#define LTDC_B6_GPIO_PIN         	GPIO_PIN_5
#define LTDC_B6_AF			        	GPIO_AF14_LTDC

#define LTDC_B7_GPIO_PORT        	GPIOK
#define LTDC_B7_GPIO_CLK_ENABLE()	__GPIOK_CLK_ENABLE()
#define LTDC_B7_GPIO_PIN         	GPIO_PIN_6
#define LTDC_B7_AF			        	GPIO_AF14_LTDC

//控制信号线
/*像素时钟CLK*/
#define LTDC_CLK_GPIO_PORT              GPIOI
#define LTDC_CLK_GPIO_CLK_ENABLE()      __GPIOI_CLK_ENABLE()
#define LTDC_CLK_GPIO_PIN               GPIO_PIN_14
#define LTDC_CLK_AF			            		GPIO_AF14_LTDC
/*水平同步信号HSYNC*/
#define LTDC_HSYNC_GPIO_PORT            GPIOI
#define LTDC_HSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_HSYNC_GPIO_PIN             GPIO_PIN_12
#define LTDC_HSYNC_AF			       				GPIO_AF14_LTDC
/*垂直同步信号VSYNC*/
#define LTDC_VSYNC_GPIO_PORT            GPIOI
#define LTDC_VSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_VSYNC_GPIO_PIN             GPIO_PIN_13
#define LTDC_VSYNC_AF			        			GPIO_AF14_LTDC

/*数据使能信号DE*/
#define LTDC_DE_GPIO_PORT               GPIOK
#define LTDC_DE_GPIO_CLK_ENABLE()       __GPIOK_CLK_ENABLE()
#define LTDC_DE_GPIO_PIN                GPIO_PIN_7
#define LTDC_DE_AF			            		GPIO_AF14_LTDC

/*液晶屏背光信号，高电平使能*/
#define LTDC_BL_GPIO_PORT               GPIOD
#define LTDC_BL_GPIO_CLK_ENABLE()       __GPIOD_CLK_ENABLE()
#define LTDC_BL_GPIO_PIN                GPIO_PIN_13

void  MLCD_Init(void);
uint8_t  MLCD_DeInit(void);
uint32_t MLCD_GetXSize(void);
uint32_t MLCD_GetYSize(void);
void     MLCD_SetXSize(uint32_t imageWidthPixels);
void     MLCD_SetYSize(uint32_t imageHeightPixels);

/* Functions using the LTDC controller */
void     MLCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);
void     MLCD_LayerRgb565Init(uint16_t LayerIndex, uint32_t FB_Address);
void     MLCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency);
void     MLCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address);
void     MLCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);

void     MLCD_ResetColorKeying(uint32_t LayerIndex);
void     MLCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

void     MLCD_SelectLayer(uint32_t LayerIndex);
void     MLCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State);

void     MLCD_SetTextColor(uint32_t Color);
uint32_t MLCD_GetTextColor(void);
void     MLCD_SetBackColor(uint32_t Color);
uint32_t MLCD_GetBackColor(void);

void MLCD_SetColors(uint32_t TextColor, uint32_t BackColor);
    
void     MLCD_SetFont(sFONT *fonts);
sFONT    *MLCD_GetFont(void);

uint32_t MLCD_GetActiveLayer(void);

uint32_t MLCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     MLCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t pixel);
void     MLCD_Clear(uint32_t Color);
void     MLCD_ClearLine(uint32_t Line);
void     MLCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void     MLCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
void     MLCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void     MLCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     MLCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     MLCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     MLCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     MLCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     MLCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void     MLCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     MLCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, const uint8_t *pbmp);

void     MLCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     MLCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     MLCD_FillPolygon(pPoint Points, uint16_t PointCount);
void     MLCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     MLCD_DisplayStringAtABSPos(uint16_t Xpos, uint16_t Ypos, uint8_t *Text);
void     MLCD_DisplayOff(void);
void     MLCD_DisplayOn(void);
void     LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
void     MLCD_MspInit(LTDC_HandleTypeDef *hltdc, void *Params);
void     MLCD_MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params);
void     MLCD_ClockConfig(void);

void MLCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address,uint32_t PixelFormat);

#endif /* __BSP_MLCD_H */
