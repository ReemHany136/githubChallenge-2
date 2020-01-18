#include "std_types.h"
#include "registers.h"
#include "gpio.h"
#include "softwareDelay.h"
#include "led.h"
#include "pushButton.h"
#include "sevenSeg.h"

void SystemInit(void);

void Stop(void);
void GetReady(void);
void Go(void);

void APP_REQ7_LED_7Seg(void);
void APP_REQ8_LED_PUSHBTN(void);
void APP_REQ9_STATEMACHINE(void);

uint32_t counter =0,Numbers=0,delayCounter=0;
uint8_t isPressedFlag = FALSE,twice=FALSE,isReleased=TRUE;
int main(void)
{
	SystemInit();
	gpioPinDirection(GPIOC,BIT7,OUTPUT);
	
	while(1)
    {
		APP_REQ7_LED_7Seg();
		APP_REQ8_LED_PUSHBTN();
		APP_REQ9_STATEMACHINE();
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
	Go();
	softwareDelayMs(1000);
	Stop();
	softwareDelayMs(1000);
	GetReady();
	softwareDelayMs(1000);
}