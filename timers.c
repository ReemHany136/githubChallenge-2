#include "utilities.h"
#include "registers.h"
#include "interrupts.h"
#include "timers.h"
#include "led.h"

#define F_CPU 16000000.0


static En_timer0Mode_t mode_T0;
static En_timer0Interrupt_t int_mode_T0;
static En_timer0perscaler_t prescale_mask_T0;
static En_timer0OC_t tOC_T0;

static En_timer1Mode_t mode_T1;
static En_timer1Interrupt_t int_mode_T1;
static En_timer1perscaler_t prescale_mask_T1;
static En_timer1OC_t tOC_T1;

static En_timer2Mode_t mode_T2;
static En_timer2Interrupt_t int_mode_T2;
static En_timer2perscaler_t prescale_mask_T2;
static En_timer2OC_t tOC_T2;




static uint16_t prescale_T0,prescale_T1,prescale_T2;
static float TickTime_T0,OvfTime_T0,CTCTime_T0,
				TickTime_T1,OvfTime_T1,CTCTime_T1,
				TickTime_T2,OvfTime_T2,CTCTime_T2;

volatile uint32_t CTC_counter =0;

isr_(TIMER2_COMP_vect){
	CTC_counter++;
	
}


void timer0Init(En_timer0Mode_t en_mode , En_timer0OC_t en_OC0,
				En_timer0perscaler_t en_prescal , uint8_t u8_initialValue,
			    uint8_t u8_outputCompare, En_timer0Interrupt_t en_interruptMask){
	/*Initializing timer0 control register*/
	TCCR0=0;
	TCCR0 = en_mode|en_OC0;
	TIMSK |= en_interruptMask;
	/*Check whether it's in the interrupt mode and set the global interrupt*/
	if(en_interruptMask == T0_INTERRUPT_NORMAL||en_interruptMask == T0_INTERRUPT_CMP){
		set_bit(SREG,GIE_bit);
	}
	else{
		clr_bit(SREG,GIE_bit);
	}
	
	OCR0   = u8_outputCompare;
	timer0Set(u8_initialValue);
		
	prescale_mask_T0 = en_prescal;
	mode_T0 = en_mode;
	int_mode_T0 = en_interruptMask;
	tOC_T0= en_OC0;


	/*Identify the prescaler value*/
	switch(en_prescal){
		case T0_PRESCALER_1024:
			prescale_T0 = 1024;
			break;
			
		case T0_PRESCALER_256:
			prescale_T0 = 256;
			break;
			
		case T0_PRESCALER_64:
			prescale_T0 = 64;
			break;
			
		case T0_PRESCALER_8:
			prescale_T0 = 8;
			break;
			
		case T0_PRESCALER_NO:
			prescale_T0 = 1;
			break;
			
		default:
		break;
	}
	/*Timer 0 Calculations*/
	TickTime_T0 = (double)(prescale_T0/F_CPU);
	OvfTime_T0 = TickTime_T0*_8_BIT_RESOLUTION;
	CTCTime_T0 = TickTime_T0*OCR0;

//	timer0Stop();
	 
}

void timer0Set(uint8_t u8_value){
	TCNT0 = u8_value;
}
uint8_t timer0Read(void){
	return TCNT0;
}

void timer0Start(void){
	TCCR0 |= prescale_mask_T0;
}
void timer0Stop(void){
	TCCR0 &= T0_NO_CLOCK;
}

void timer0DelayMs(uint16_t u16_delay_in_ms){
	uint16_t oc, i =0;
	En_timer0perscaler_t prescale_mask;
	/*Saving timer0 registers*/
	oc= OCR0;
	prescale_mask = prescale_mask_T0;
	
	/*Updating timer1 registers values to suit the delay*/
	TCCR0=0;
	prescale_mask_T0=T0_PRESCALER_64;
	TCCR0|=T0_COMP_MODE|T0_OC0_CLEAR;
	TIMSK|=T0_POLLING;
	OCR0 =250;
	
	/*Delay steps using polling*/
	timer0Start();
	TCNT0=0;
	
	for(i=0;i<u16_delay_in_ms;i++){
		/*pol until the flag is set*/
		while(!(TIFR & (1<<OCF0_T0)));
		TIFR |= 1<<OCF0_T0;
		TCNT0=0;
	}
	timer0Stop();
	
	/*Restoring Timer0 register values*/
	prescale_mask_T0=prescale_mask;
	TCCR0|=mode_T0|tOC_T0;
	TIMSK|=int_mode_T0;
	OCR0 =oc;
}
void timer0DelayUs(uint32_t u32_delay_in_us){
	uint32_t i;		
	/*Re-initialize timer0 to be suitable for the delay*/	
	timer0Init(T0_COMP_MODE,T0_OC0_CLEAR,T0_PRESCALER_NO,0,16,T0_POLLING);	
	
	/*Delay steps using polling*/
	timer0Start();
	for(i=0;i<u32_delay_in_us;i++){
		/*Pol until the flag is set*/
		while(!(TIFR & (1<<OCF0_T0)));
		TIFR |= 1<<OCF0_T0;
		TCNT0=0;
	}
	timer0Stop();
	
}



void timer1Init(En_timer1Mode_t en_mode,En_timer1OC_t en_OC,
				En_timer1perscaler_t en_prescal, uint16_t u16_initialValue, 
				uint16_t u16_outputCompareA, uint16_t u16_outputCompareB,uint16_t u16_inputCapture, En_timer1Interrupt_t en_interruptMask){
	/*Initializing timer1 registers*/
	TCCR1=0;
	TCCR1 |= en_mode|en_OC;
	TIMSK |= en_interruptMask;
	/*Check whether it's in the interrupt mode and set the global interrupt*/
	if(en_interruptMask == T1_INTERRUPT_NORMAL||en_interruptMask == T1_INTERRUPT_CMP_1A||en_interruptMask ==T1_INTERRUPT_ICAPTURE||en_interruptMask == T0_INTERRUPT_CMP_1B){
		set_bit(SREG,GIE_bit);
	}
	else{
		clr_bit(SREG,GIE_bit);
	}
	
	OCR1A   = u16_outputCompareA;
	OCR1B	= u16_outputCompareB;
	ICR1 = u16_inputCapture;
	timer1Set(u16_initialValue);
	
	
	prescale_mask_T1 = en_prescal;
	mode_T1 = en_mode;
	int_mode_T1 = en_interruptMask;
	tOC_T1= en_OC;
	
	/*Identify the prescaler value*/
	switch(en_prescal){
		case T1_PRESCALER_1024:
		prescale_T1 = 1024;
		break;
		
		case T1_PRESCALER_256:
		prescale_T1 = 256;
		break;
		
		case T1_PRESCALER_64:
		prescale_T1 = 64;
		break;
		
		case T1_PRESCALER_8:
		prescale_T1 = 8;
		break;
		
		case T1_PRESCALER_NO:
		prescale_T1 = 1;
		break;
		
		default:
		break;
	}
	
	/*Timer1 Calculations*/
	TickTime_T1 = (double)(prescale_T1/F_CPU);
	OvfTime_T1 = TickTime_T1*_16_BIT_RESOLUTION;
	CTCTime_T1 = TickTime_T1*OCR1A;
}

void timer1Set(uint16_t u16_value){
	TCNT1 = u16_value;
}
uint16_t timer1Read(void){
	return TCNT1;
}

void timer1Start(void){
	TCCR1 |= prescale_mask_T1;
}
void timer1Stop(void){
	TCCR1 &= T1_NO_CLOCK;
}

void timer1DelayMs(uint16_t u16_delay_in_ms){
	uint16_t oc, i =0;
	En_timer1perscaler_t prescale_mask;
	/*Saving timer1 registers*/
	oc= OCR1A;
	prescale_mask = prescale_mask_T1;
	
	/*Updating Timer1 registers to suits the delay*/
	TCCR1=0;
	prescale_mask_T1=T1_PRESCALER_64;
	TCCR1|=T1_COMP_MODE_OCR1A_TOP|T1_OC1A_CLEAR;
	TIMSK|=T1_POLLING;
	OCR1A =250;
	
	/*Delay steps using polling*/
	timer1Start();
	TCNT1=0;
	for(i=0;i<u16_delay_in_ms;i++){
		while(!(TIFR & (1<<OCFA_T1)));
		TIFR |= 1<<OCFA_T1;
		TCNT1=0;
	}
	timer1Stop();
	
	/*Restoring Timer1 register values*/
	prescale_mask_T1=prescale_mask;
	TCCR1|=mode_T1|tOC_T1;
	TIMSK|=int_mode_T1;
	OCR1A =oc;
}
void timer1DelayUs(uint32_t u32_delay_in_us){
	uint16_t oc, i =0;
	En_timer1perscaler_t prescale_mask;
	/*Saving timer1 registers*/
	oc= OCR1A;
	prescale_mask = prescale_mask_T1;
	
	/*Updating Timer1 registers to suits the delay*/
	TCCR1=0;
	prescale_mask_T1=T1_PRESCALER_NO;
	TCCR1|=T1_COMP_MODE_OCR1A_TOP|T1_OC1A_CLEAR;
	TIMSK|=T1_POLLING;
	OCR1A =16;
	
	/*Delay steps using polling*/
	timer1Start();
	TCNT1=0;
	for(i=0;i<u32_delay_in_us;i++){
		/*pol until the flag is set*/
		while(!(TIFR & (1<<OCFA_T1)));
		TIFR |= 1<<OCFA_T1;
		TCNT1=0;
	}
	timer1Stop();
	
	/*Restoring timer1 register values*/
	prescale_mask_T1=prescale_mask;
	TCCR1|=mode_T1|tOC_T1;
	TIMSK|=int_mode_T1;
	OCR1A =oc;
}

void timer1SwPWM(uint8_t u8_dutyCycle,uint8_t u8_frequency);




void timer2Init(En_timer2Mode_t en_mode,En_timer2OC_t en_OC,En_timer2perscaler_t en_prescal,
		 uint8_t u8_initialValue, uint8_t u8_outputCompare, 
		 uint8_t u8_assynchronous, En_timer2Interrupt_t en_interruptMask)
{
	/*Initializing timer2 registers*/
	TCCR2=0;
	TCCR2 = en_mode|en_OC;
	TIMSK |= en_interruptMask;
	/*Check whether it's in the interrupt mode and set the global interrupt*/
	if(en_interruptMask == T2_INTERRUPT_NORMAL||en_interruptMask == T2_INTERRUPT_CMP){
		set_bit(SREG,GIE_bit);
	}
	else{
		clr_bit(SREG,GIE_bit);
	}
	OCR2   = u8_outputCompare;
	timer2Set(u8_initialValue);
	
	prescale_mask_T2 = en_prescal;
	mode_T2 = en_mode;
	int_mode_T2 = en_interruptMask;
	tOC_T2= en_OC;
	
	/*Identify the prescaler value*/
	switch(en_prescal){
		case T2_PRESCALER_1024:
		prescale_T2 = 1024;
		break;
		
		case T2_PRESCALER_256:
		prescale_T2 = 256;
		break;
		
		case T2_PRESCALER_128:
		prescale_T2 = 128;
		break;
		
		case T2_PRESCALER_64:
		prescale_T2 = 64;
		break;
		
		case T2_PRESCALER_32:
		prescale_T2 = 32;
		break;
		
		case T2_PRESCALER_8:
		prescale_T2 = 8;
		break;
		
		case T2_PRESCALER_NO:
		prescale_T2 = 1;
		break;
		
		default:
		break;
	}
	
	TickTime_T2 = (double)(prescale_T2/F_CPU);
	OvfTime_T2 = TickTime_T2*_8_BIT_RESOLUTION;
	CTCTime_T2 = TickTime_T2*OCR2;
	
}

void timer2Set(uint8_t u8_a_value){
	TCNT2 = u8_a_value;
}
uint8_t timer2Read(void){
	return TCNT2;
}

void timer2Start(void){
	TCCR2 |= prescale_mask_T2;
}
void timer2Stop(void){
	TCCR2 &= T2_NO_CLOCK;
}

void timer2DelayMs(uint16_t u16_delay_in_ms){
	uint16_t oc, i =0;
	En_timer2perscaler_t prescale_mask;
	
	/*Saving timer2 registers*/
	oc= OCR2;
	prescale_mask = prescale_mask_T2;
	
	/*Updating timer2 register values to suits the delay*/
	prescale_mask_T2=T2_PRESCALER_64;
	TCCR2|=T2_COMP_MODE|T2_OC2_CLEAR;
	TIMSK|=T2_POLLING;
	OCR2 =250;
	
	/*Delay steps using polling*/
	timer2Start();
	TCNT2=0;
	for(i=0;i<u16_delay_in_ms;i++){
		Led_On(LED_1);
		while(!(TIFR & (1<<OCF2_T2)));
		TIFR |= 1<<OCF2_T2;
		TCNT2=0;
		Led_Off(LED_1);
	}
	timer2Stop();
	
	/*Restoring timer2 registers*/
	prescale_mask_T2=prescale_mask;
	TCCR2|=mode_T2|tOC_T2;
	TIMSK|=int_mode_T2;
	OCR2 =oc;
}
void timer2DelayUs(uint32_t u16_delay_in_us){
	/*Delay steps using interrupt */
	timer2Start();
	/*wait until the delay time is finished*/
	while(CTC_counter<u16_delay_in_us);
	CTC_counter=0;
	timer2Stop();
}
