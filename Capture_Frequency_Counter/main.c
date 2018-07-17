/*(C) Umang Gajera - www.ocfreaks.com
LPC176x Input Capture Tutorial - Example for frequency counter using ARM KEIL
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License: GPL.*/

#include <lpc17xx.h>
#include <stdio.h> //for printf() - http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/
#include "ocf_lpc176x_lib.h" //contains initUART0(), initTimer0() & putc() to retarget printf()

void initPWM(void);
unsigned int period = 0;
unsigned int previous = 0;
unsigned int current = 0 ;
int limitFlag = 0;
#define TIMER_RES 0.02 //Depends on Timer PCLK and PR. Used to convert measured period to frequency. 

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initUART0(); //Initialize UART0 for uart_printf() - see ocf_lpc176x_lib.c
	initTimer0(); //For delayMS() - see ocf_lpc176x_lib.c
	
	/*Using CCLK = 100Mhz and PCLK_TIMER2 = 100Mhz.*/
	LPC_SC->PCONP |= (1<<22); //Power-Up Timer2 module. It is disabled by default.
	LPC_SC->PCLKSEL1 |= (1<<12); //Set bits[13:12] = [01] to select PCLK_TIMER2 = CCLK i.e. 100Mhz in our case. 
	LPC_PINCON->PINSEL0 |= (1<<9) | (1<<8); //Set Bits[9:8] = [11] to Select CAP2.0 for P0.4
	LPC_TIM2->CTCR = 0x0;
	LPC_TIM2->PR = 1; //PR+1 = 1+1 = 2 clock cycles @ 100Mhz = 0.02us res
	LPC_TIM2->TCR = 0x02; //Reset Timer
	LPC_TIM2->CCR = (1<<0) | (1<<2); //Capture on Rising Edge(0->1) and generate an interrupt
	LPC_TIM2->TCR = 0x01; //Enable timer1
	
	NVIC_EnableIRQ(TIMER2_IRQn); //Enable TIMER2 IRQ
	initPWM(); //To generate square wave	
	
	printf("OCFreaks.com - Frequency Counter using Timer Capture\n");

	while(1)
	{
		if(limitFlag)
		{
			printf("Input Frequency limit reached!\n");
			NVIC_EnableIRQ(TIMER2_IRQn); //Try to measure signal frequency again
			delayMS(500);
		}
		else
		{
			printf("Frequency = %0.2f Khz\n",((1.0/(period*TIMER_RES)) * 1000)); //Convert to frequency, 0.02 is Timer resolution
			delayMS(500); //2 Udpates per second
		}
	}
	
	//return 0; //This won't execute normally
}

void TIMER2_IRQHandler(void)
{
	LPC_TIM2->IR |= (1<<4); //Clear Interrupt Flag
	current = LPC_TIM2->CR0;
	if(current < previous) //TC has overflowed
	{
		period = 0xFFFFFFFF + current - previous;
	}
	else
	{
		period = current - previous;
	}
	previous = current; //LPC_TIM2->CR0;
	
	if(period < 60) //Limit to 833.3 Khz
	{
		NVIC_DisableIRQ(TIMER2_IRQn);
		limitFlag = 1;
	}
	else limitFlag = 0;
}

void initPWM(void)
{
	//Refer: http://www.ocfreaks.com/lpc1768-pwm-programming-tutorial/
	/*Using CCLK = 100Mhz and PCLK_PWM1 = 100Mhz.*/
	
	//By default PWM1 block is powered-on
	LPC_SC->PCLKSEL0 |= (1<<12); //Set bits[13:12] = [01] to select PCLK_PWM1 = CCLK i.e. 100Mhz in our case. 
	LPC_PINCON->PINSEL4 |= (1<<0); // Select PWM1.1 output for Pin2.0
	LPC_PWM1->PCR = 0x0; //Select Single Edge PWM - by default its single Edged so this line can be removed
	LPC_PWM1->PR = 1; //PR+1 = 1+1 = 2 Clock cycles @100Mhz = 0.02us
	LPC_PWM1->MR0 = 80; //80x0.02 = 1.6us - period duration i.e. 625Khz Test frequency
	LPC_PWM1->MR1 = 40; //40x0.02 = 0.8us - pulse duration (50% duty cycle)
	LPC_PWM1->MCR = (1<<1); // Reset PWMTC on PWMMR0 match
	LPC_PWM1->LER = (1<<1) | (1<<0); // update MR0 and MR2
	LPC_PWM1->PCR = (1<<9); // enable PWM1.1 output
	LPC_PWM1->TCR = (1<<1) ; //Reset PWM TC & PR

	LPC_PWM1->TCR = (1<<0) | (1<<3); // enable counters and PWM Mode
	//PWM Generation goes active now!!
}
