#include "oled.h"
#include "stdlib.h"
#include "os.h"
#include "oledfont.h"

uint8_t OLED_GRAM[128 * 8 + 1];

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;

static void MX_I2C1_Init(void);

__STATIC_INLINE void delayt(uint16_t de){
	while(de--);
}

__STATIC_INLINE void I2C_WriteByte(uint8_t addr,uint8_t data){
	uint8_t dat[2];
	dat[0] = addr;
	dat[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, dat, 2,100);
}

__STATIC_INLINE void WriteCmd(unsigned char I2C_Command){//д����
	I2C_WriteByte(0x00, I2C_Command);
}

void OLED_Display_On(void){
	WriteCmd(0X8D);
	WriteCmd(0X14);
	WriteCmd(0XAF);
}

void OLED_Display_Off(void){
	WriteCmd(0X8D);
	WriteCmd(0X10);
	WriteCmd(0XAE);
}

void OLED_Clear(void){
	uint8_t i, n;
	for (i = 0; i < 8; i++)
		for (n = 0; n < 128; n++)
			OLED_GRAM[n + i * 128] = 0X00;
}

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t){
	uint8_t pos, bx, temp = 0;
	if (x > 127 || y > 63)
		return;
	
	bx = y % 8;
	pos = y / 8;
	temp = 1 << bx;
	if (t)
		OLED_GRAM[x + pos * 128 + 1] |= temp;
	else
		OLED_GRAM[x + pos * 128 + 1] &= ~temp;
}

void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot){
	uint8_t x, y;
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
			OLED_DrawPoint(x, y, dot);
	}
	OLED_Refresh_Gram();
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode){
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
	chr = chr - ' ';
	for (t = 0; t < csize; t++)
	{
		if (size == 12)
			temp = asc2_1206[chr][t];
		else if (size == 16)
			temp = asc2_1608[chr][t];
		else if (size == 24)
			temp = asc2_2412[chr][t];
		else
			return;
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				OLED_DrawPoint(x, y, mode);
			else
				OLED_DrawPoint(x, y, !mode);
			temp <<= 1;
			y++;
			if ((y - y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

uint32_t mypow(uint8_t m, uint8_t n){
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size){
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (size / 2) * t, y, ' ', size, 1);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size, 1);
	}
}

void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size){
	while ((*p <= '~') && (*p >= ' ')) 
	{
		if (x > (128 - (size / 2)))
		{
			x = 0;
			y += size;
		}
		if (y > (64 - size))
		{
			y = x = 0;
			OLED_Clear();
		}
		OLED_ShowChar(x, y, *p, size, 1);
		x += size / 2;
		p++;
	}
}

void OLED_Init(void){
	OLED_GRAM[0] = 0x40;
	
	//HAL_I2C1_MspInit();
	MX_I2C1_Init();

	delayt(0xfff);

	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y){ //������ʼ������ 
	WriteCmd(0xb0 + y);
	WriteCmd(((x & 0xf0) >> 4)|0x10);
	WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Refresh_Gram(){
	OLED_SetPos(0, 0);
	__HAL_I2C_DISABLE(&hi2c1);
	OLED_GRAM[0] = 0x40;
	__HAL_I2C_ENABLE(&hi2c1);
	HAL_I2C_Master_Transmit_DMA(&hi2c1, OLED_ADDRESS, OLED_GRAM, 128 * 8 + 1);
	//__HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_BUSY);
}

void OLED_ADDval(uint8_t x){
    static uint8_t max = 0;
    uint8_t i = 0, j = 0;
    if (x > 64)
        return;
    if (max < 128)
    {
        OLED_DrawPoint(max, 63 - x, 1);
        max++;
    }
    else
    {
        for (i = 0; i < 8; i++)
            OLED_GRAM[i * 128 + 1] = OLED_GRAM[2 + i * 128];
        for (i = 1; i < 127; i++)
        {
            for (j = 0; j < 8; j++)
                OLED_GRAM[i + j * 128 + 1] = OLED_GRAM[i + 2 + j * 128];
        }
        for (j = 0; j < 8; j++)
            OLED_GRAM[128 + j * 128] = 0;
        OLED_DrawPoint(127, 63 - x, 1);
    }
}

__STATIC_INLINE void MX_I2C1_Init(void){
  __HAL_RCC_DMA1_CLK_ENABLE();
  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);


  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20200719;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C1_MspInit();
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    while(1);
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    while(1);
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    while(1);
  }
  /** I2C Enable Fast Mode Plus 
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
}

 void HAL_I2C1_MspInit(){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
    /**I2C1 GPIO Configuration    
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL 
    */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  
    /* I2C1 DMA Init */
    /* I2C1_TX Init */
    hdma_i2c1_tx.Instance = DMA1_Stream0;
    hdma_i2c1_tx.Init.Request = DMA_REQUEST_I2C1_TX;
    hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_i2c1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c1_tx) != HAL_OK)
    {
    //  Error_Handler();
    }

    __HAL_LINKDMA(&hi2c1,hdmatx,hdma_i2c1_tx);

  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
}
