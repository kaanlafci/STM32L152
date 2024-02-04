/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Button.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright MAKERSAN
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "Button.h"
#include "main.h"
/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/*main task states enumaration*/
enum en_ButtonTaskState {
  InitState,
  FaultState,
  DebounceSetState,
  RoutineState
};

enum en_ButtonTaskState en_ButtonCtrlState;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

#define BC_DEBOUNCE_MAX   1000 /* max Debounce treshold val 1000ms*/
#define BC_DEBOUNCE_MN    1    /* min Debounce treshold val 1ms*/

#define BC_TASK_INTERVAL  1  /* 1 ms Button control Task interval */
#define BC_DEBOUCE_VAL    50 /* 50 ms Debounce treshold*/
#define BC_DEBOUCE_TRSH   BC_DEBOUCE_VAL/BC_TASK_INTERVAL /*Calculted Debounce treshold val*/

/*button1 hardware adress*/
#define Button1_Pin        	GPIO_PIN_0
#define Button1_GPIO_Port  	GPIOA

/*button2 hardware adress*/
#define Button2_Pin 		GPIO_PIN_1
#define Button2_GPIO_Port 	GPIOA

/*button3 hardware adress*/
#define Button3_Pin 		GPIO_PIN_2
#define Button3_GPIO_Port 	GPIOA

/* Private variables ---------------------------------------------------------*/

uint32_t BC_Debounce_Set_Val     = BC_DEBOUCE_TRSH;
uint32_t pre_BC_Debounce_Set_Val = BC_DEBOUCE_TRSH;

/* Private function prototypes -----------------------------------------------*/

/* Private structure  ---------------------------------------------------------*/

/*this struct include button spesific counter treshold and state flag variables */
typedef struct  {

    uint32_t u32_CNT;
    uint32_t u32_TRSH;
    uint8_t  u8_FLAG;

}ButtonTypeDef;


struct FailBit {
	uint8_t DBNC_MAX_OF : 1;
	uint8_t DBNC_MIN_OF : 1;
	uint8_t rsvd1 : 1;
	uint8_t rsvd2 : 1;
	uint8_t rsvd3 : 1;
	uint8_t rsvd4 : 1;
	uint8_t rsvd5 : 1;
	uint8_t rsvd6 : 1;
};

/*this union include fail bits of button ButtonMainTask*/
union st_ButtonFailBits {
    struct FailBit bits;
    uint8_t data;
};

union st_ButtonFailBits ButtontFailBits;


/* Private user code ---------------------------------------------------------*/
/*button structure set the init values*/
ButtonTypeDef st_button1 = {0,BC_DEBOUCE_TRSH,0};
ButtonTypeDef st_button2 = {0,BC_DEBOUCE_TRSH,0};
ButtonTypeDef st_button3 = {0,BC_DEBOUCE_TRSH,0};

/**
  * @brief  Button control Hardware Init Function
  * @retval
  */
void ButtonHardwareInit(void)
{

	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pins : PAPin PAPin PAPin */
	  GPIO_InitStruct.Pin = Button1_Pin|Button2_Pin|Button3_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
  * @brief  Button control Init Function
  * It set the variables to Init value
  * @param
  * @retval
  */
void ButtonInitTask(void)
{
	ButtonHardwareInit();

    ButtontFailBits.data = 0x00; /*Fail bits set the zero*/

    /*previous debounce value set the BC_DEBOUCE_TRSH*/
	pre_BC_Debounce_Set_Val = BC_DEBOUCE_TRSH;

	/*debounce value set the BC_DEBOUCE_TRSH*/
	BC_Debounce_Set_Val = BC_DEBOUCE_TRSH;

	/* st_button1 structure set the init values*/
	st_button1.u32_CNT  = 0;
	st_button1.u32_TRSH = BC_DEBOUCE_TRSH;
	st_button1.u8_FLAG  = 0;

	/* st_button2 structure set the init values*/
	st_button2.u32_CNT  = 0;
	st_button2.u32_TRSH = BC_DEBOUCE_TRSH;
	st_button2.u8_FLAG  = 0;

	/* st_button3 structure set the init values*/
	st_button3.u32_CNT  = 0;
	st_button3.u32_TRSH = BC_DEBOUCE_TRSH;
	st_button3.u8_FLAG  = 0;


}

/**
  * @brief this fumction read buton input value. It has debounce algorithm
  * @param
  * @retval
  */
void readButton( ButtonTypeDef *Button ,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
	{
		if(Button->u32_CNT < Button->u32_TRSH)
		  { Button->u32_CNT++; }
		else
		  { Button->u8_FLAG = 1; }
	}
	else
	{
		Button->u32_CNT = 0;
		Button->u8_FLAG = 0;
	}

}

/**
  * @brief  This function set the debounce value
  * @param uint32_t treshold
  * @retval
  */
void SetDebounce(uint32_t treshold)
{
	st_button1.u32_TRSH = treshold;
	st_button2.u32_TRSH = treshold;
	st_button3.u32_TRSH = treshold;
}

/**
  * @brief  Button control Task
  * @param
  * @retval
  */
void ButtonMainTask(void)
{
	switch (en_ButtonCtrlState) {
	  case InitState:

		    ButtontFailBits.data = 0x00; /*Fail bits set the zero*/

		    /*previous debounce value set the BC_DEBOUCE_TRSH*/
			pre_BC_Debounce_Set_Val = BC_DEBOUCE_TRSH;

			/*debounce value set the BC_DEBOUCE_TRSH*/
			BC_Debounce_Set_Val = BC_DEBOUCE_TRSH;

			/* st_button1 structure set the init values*/
			st_button1.u32_CNT  = 0;
			st_button1.u32_TRSH = BC_DEBOUCE_TRSH;
			st_button1.u8_FLAG  = 0;

			/* st_button2 structure set the init values*/
			st_button2.u32_CNT  = 0;
			st_button2.u32_TRSH = BC_DEBOUCE_TRSH;
			st_button2.u8_FLAG  = 0;

			/* st_button3 structure set the init values*/
			st_button3.u32_CNT  = 0;
			st_button3.u32_TRSH = BC_DEBOUCE_TRSH;
			st_button3.u8_FLAG  = 0;

			/* task state set the RoutineState*/
			en_ButtonCtrlState = RoutineState;

		  break;

	  case RoutineState:

		  readButton(&st_button1,Button1_GPIO_Port,Button1_Pin);
		  readButton(&st_button2,Button2_GPIO_Port,Button2_Pin);
		  readButton(&st_button3,Button3_GPIO_Port,Button3_Pin);

		  if(pre_BC_Debounce_Set_Val != BC_Debounce_Set_Val)
		  {
			  en_ButtonCtrlState = DebounceSetState;
		  }
	    break;

	  case DebounceSetState:
		  if(BC_Debounce_Set_Val > BC_DEBOUNCE_MAX)
		  {
			  ButtontFailBits.bits.DBNC_MAX_OF = TRUE;
		  }
		  else if(BC_Debounce_Set_Val < BC_DEBOUNCE_MN)
		  {
			  ButtontFailBits.bits.DBNC_MIN_OF = TRUE;
		  }
		  else
		  {
			  pre_BC_Debounce_Set_Val = BC_Debounce_Set_Val;
			  SetDebounce(BC_Debounce_Set_Val);
			  en_ButtonCtrlState = RoutineState;
		  }

		break;

	  case FaultState:
		/*do fault action here */
		break;

	  default:
		  en_ButtonCtrlState =FaultState;
	}

}
