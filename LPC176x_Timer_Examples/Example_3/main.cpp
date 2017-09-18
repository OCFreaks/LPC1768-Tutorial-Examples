/*(C) Umang Gajera - http://www.ocfreaks.com 2011-17.
http://www.ocfreaks.com/lpc1768-timer-tutorial/
LPC1768 Timer example 3.

More Embedded tutorials @ http://www.ocfreaks.com/cat/embedded/
*/

#include <LPC17xx.h>

#define PRESCALE 25000 //25000 PCLK clock cycles to increment TC by 1 

void initTimer0();

int main(void) 
{
	LPC_GPIO1->FIODIR |= (1<<29); //set P1.29 as output

	initTimer0();
	
	while(1)
	{
		//Idle loop
	}
	//return 0; //normally this won't execute
}


void initTimer0(void)
{
	/*Assuming that PLL0 has been setup with CCLK = 100Mhz and PCLK = 25Mhz.*/
	LPC_SC->PCONP |= (1<<1); //Power up TIM0. By default TIM0 and TIM1 are enabled.
	LPC_SC->PCLKSEL0 |= (1<<3); //Set PCLK for timer = CCLK/4 = 100/4 (default)
	
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = PRESCALE-1; //Increment LPC_TIM0->TC at every 24999+1 clock cycles
	//Count begins from zero hence subtracting 1
	//25000 clock cycles @25Mhz = 1 mS
	
	LPC_TIM0->MR0 = 500; //Toggle Time in mS
	LPC_TIM0->MCR |= (1<<0) | (1<<1); // Interrupt & Reset on MR0 match
	LPC_TIM0->TCR |= (1<<1); // Reset Timer0

	NVIC_EnableIRQ(TIMER0_IRQn); //Enable timer interrupt
	
	LPC_TIM0->TCR = 0x01; //Enable timer
}

extern "C" void TIMER0_IRQHandler(void) //Use extern "C" so C++ can link it properly, for C it is not required
{
	LPC_TIM0->IR |= (1<<0); //Clear MR0 Interrupt flag
	LPC_GPIO1->FIOPIN ^= (1<<29); //Toggle LED
}
