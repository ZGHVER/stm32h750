#include "./sai/sai.h"

SAI_HandleTypeDef h_sai;

void SAI_Init(){

  __SAI1_CLK_ENABLE();
  HAL_SAI_DeInit(&h_sai);
  h_sai.Instance = SAI1_Block_A;
  h_sai.Init.AudioMode = SAI_MODEMASTER_TX;//配置为发送模式
  h_sai.Init.Synchro = SAI_ASYNCHRONOUS; //模块内部为异步
  h_sai.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;//立刻输出
  h_sai.Init.NoDivider=SAI_MASTERDIVIDER_ENABLE;//使能MCK输出NOACK
  h_sai.Init.FIFOThreshold=SAI_FIFOTHRESHOLD_1QF;//1/4FIFO
  h_sai.Init.MonoStereoMode=SAI_STEREOMODE;
  h_sai.Init.MckOverSampling = SAI_MCK_OVERSAMPLING_DISABLE;
  h_sai.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
  //printf("%d", _usAudioFreq);
  SAI1_Block_A->CR1 &= 0;
  HAL_SAI_InitProtocol(&h_sai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2);//2--left_channel and right_channel
  __HAL_SAI_ENABLE(&h_sai);
  

}