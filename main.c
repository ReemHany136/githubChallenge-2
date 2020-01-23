#include "std_types.h"
#include "registers.h"
#include "gpio.h"
#include "timers.h"
#include "softwareDelay.h"
#include "led.h"
#include "pushButton.h"
#include "sevenSeg.h"

typedef enum stateMachine{
	go,getReady,stop
}stateMachine;


/*Functions prototype*/
void SystemInit(void);

void Stop(void);
void GetReady(void);
void Go(void);

void APP_REQ7_LED_7Seg(void);
void APP_REQ8_LED_PUSHBTN(void);
void APP_REQ9_STATEMACHINE(void);

void APP_REQ10_UPDATED_LED_7Seg(void);
void APP_REQ10_UPDATED_LED_PUSHBTN(void);
void APP_REQ10_UPDATED_STATEMACHINE(void);


int main(void)
{
	SystemInit();
	Led_Init(LED_0);
	while(1)
    {
		APP_REQ10_UPDATED_LED_PUSHBTN();
//		APP_REQ10_UPDATED_STATEMACHINE();
		
    }
}
void SystemInit(void){
	Led_Init(LED_0);
	Led_Init(LED_1);
	Led_Init(LED_2);
	Led_Init(LED_3);
	
	pushButtonInit(BTN_0);
	pushButtonInit(BTN_1);
	pushButtonInit(BTN_2);
	pushButtonInit(BTN_3);
	
	sevenSegInit(SEG_0);
	sevenSegInit(SEG_1);
}

void Stop(void){
	Led_Off(LED_1);
	Led_On(LED_2);
	Led_Off(LED_3);
}
void GetReady(void){
	Led_Off(LED_1);
	Led_Off(LED_2);
	Led_On(LED_3);
}
void Go(void){
	Led_On(LED_1);
	Led_Off(LED_2);
	Led_Off(LED_3);
}

void APP_REQ7_LED_7Seg(void){
	uint32_t counter =0,Numbers=0;
	while (Numbers<100)
	{
		for(counter=0;counter<(350);counter++){
			sevenSegWrite2Numbers(Numbers);
			softwareDelayMs(1);
		}
		Numbers++;
	}
	Numbers=0;
	
}
void APP_REQ8_LED_PUSHBTN(void){
	uint16_t i=0;
	uint32_t delayCounter=0;
	uint8_t isPressedFlag = FALSE,isReleased=TRUE;
	if((pushButtonGetStatus(BTN_0)==Pressed)&&isReleased==TRUE){
		
		isPressedFlag =TRUE;
		isReleased=FALSE;
		delayCounter++;
	}
	else{
		if (isPressedFlag)//pushed before
		{
			isPressedFlag=FALSE;
			isReleased=TRUE;
			for (i =0; i< (8*delayCounter);i++)
			{Led_On(LED_2);
				softwareDelayMs(1);
				Led_On(LED_0);
				if((pushButtonGetStatus(BTN_0)==Pressed) && isReleased == TRUE){
					
					delayCounter++;
					isReleased = FALSE;
				}
				else{
					isReleased = TRUE;
				}
				
			}
			delayCounter=0;
			Led_Off(LED_0);
		}
	}
}
void APP_REQ9_STATEMACHINE(void){
	static stateMachine state;
	switch(state){
		case go:
		Go();
		softwareDelayMs(1000);
		state = stop;
		break;
		case stop:
		Stop();
		softwareDelayMs(1000);
		state = getReady;
		break;
		case getReady:
		GetReady();
		softwareDelayMs(1000);
		state = go;
		break;
		default:
		break;
	}
	
}

void APP_REQ10_UPDATED_LED_7Seg(void){
	uint32_t Numbers=0,i;
	while (Numbers<100)
	{
		for(i=0;i<500;i++)
		{
			sevenSegWrite2Numbers(Numbers);
			timer1DelayMs(1);
		}
		Numbers++;
	}
	Numbers=0;
	
}
void APP_REQ10_UPDATED_LED_PUSHBTN(void){
	uint16_t i=0;
	uint32_t delayCounter=0;
	uint8_t isPressedFlag = FALSE,isReleased=TRUE;
	if((pushButtonGetStatus(BTN_0)==Pressed)&&isReleased==TRUE){
		
		isPressedFlag =TRUE;
		isReleased=FALSE;
		delayCounter++;
	}
	else{
		if (isPressedFlag)//pushed before
		{
			isPressedFlag=FALSE;
			isReleased=TRUE;
			for (i =0; i< (8*delayCounter);i++)
			{Led_On(LED_2);
				timer1DelayMs(1);
				Led_On(LED_0);
				if((pushButtonGetStatus(BTN_0)==Pressed) && isReleased == TRUE){
					
					delayCounter++;
					isReleased = FALSE;
				}
				else{
					isReleased = TRUE;
				}
				
			}
			delayCounter=0;
			Led_Off(LED_0);
		}
	}
}
void APP_REQ10_UPDATED_STATEMACHINE(void){
	static stateMachine state;
	switch(state){
		case go:
		Go();
		timer1DelayMs(1000);
		state = stop;
		break;
		case stop:
		Stop();
		timer1DelayMs(1000);
		state = getReady;
		break;
		case getReady:
		GetReady();
		timer1DelayMs(1000);
		state = go;
		break;
		default:
		break;
	}
}