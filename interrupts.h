/*
 * interrupts.h
 *
 * Created: 1/22/2020 5:00:10 PM
 *  Author: Reem
 */ 


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#define INT0_vect			__vector_1
#define INT1_vect			__vector_2
#define INT2_vect			__vector_3
#define TIMER2_COMP_vect	__vector_4
#define TIMER2_OVF_vect		__vector_5
#define TIMER1_CAPT_vect	__vector_6
#define TIMER1_COMPA_vect	__vector_7
#define TIMER1_COMPB_vect	__vector_8
#define TIMER1_OVF_vect		__vector_9
#define TIMER0_COMP_vect	__vector_10
#define TIMER0_OVF_vect		__vector_11
#define SPI_STC_vect		__vector_12
#define USART_RXC_vect		__vector_13
#define USART_UDRE_vect		__vector_14
#define USART_TXC_vect		__vector_15
#define ADC_vect			__vector_16
#define EE_RDY_vect			__vector_17
#define ANA_COMP_vect		__vector_18
#define TWI_vect			__vector_19
#define SPM_RDY_vect		__vector_20

#define __INTRS_ATTRS used,externally_visible 
#define isr_(vector, ...)            \
void vector (void) __attribute__ ((signal, __INTRS_ATTRS)) ; \
void vector (void)

#define SREG		(*(volatile uint8_t*)   0x5F)
#define GIE_bit		7

#endif /* INTERRUPTS_H_ */