#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void LED_Config()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void Button_Config()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitButton;
  GPIO_InitButton.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitButton.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitButton.GPIO_OType = GPIO_OType_PP;
  GPIO_InitButton.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitButton.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitButton);
}

void TIM_Config()
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  uint32_t valuePSC = (SystemCoreClock / 1000) - 1;

  TIM_TimeBaseInitTypeDef TIM_InitStructure;
  TIM_InitStructure.TIM_Prescaler = valuePSC;
  TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_InitStructure.TIM_Period = 2000 - 1;
  TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_InitStructure.TIM_RepetitionCounter = 0;
  
  TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
  TIM_Cmd(TIM2, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void Enable_IRQ()
{
  NVIC_InitTypeDef NVIC_Structure;
  NVIC_Structure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Structure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_Structure);
}

void TIM2_IRQHandler()
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);    
  }
}

void main()
{
  LED_Config();
  TIM_Config();
  Button_Config();  
  
  for(;;)
  {      
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
      GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
      Enable_IRQ();
    }
  }  
}