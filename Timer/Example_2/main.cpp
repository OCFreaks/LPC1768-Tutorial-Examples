/*(C) Umang Gajera - https://www.ocfreaks.com 2011-17.
https://www.ocfreaks.com/lpc1768-timer-programming-tutorial/
LPC1768 Timer example 2.

More Embedded tutorials @ https://www.ocfreaks.com/cat/embedded/
*/

#include <LPC17xx.h>

#define PRESCALE (25000-1) //25000 PCLK clock cycles to increment TC by 1 

void initTimer0(void);

int main(void) 
{
	//SystemInit(); //called by Startup Code before main(), hence no need to call again.

	LPC_PINCON->PINSEL3 |= (1<<25) | (1<<24) | (1<<27) | (1<<26); //config MAT0.0(P1.28) and MAT0.1(P1.29) outputs

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
	LPC_SC->PCONP |= (1<<1); //Power up Timer0. By default Timer0 and Timer1 are enabled.
	LPC_SC->PCLKSEL0 &= ~(0x3<<2); //Set PCLK for Timer0 = CCLK/4 = 100/4 (default)
	
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = PRESCALE; //Increment LPC_TIM0->TC at every 24999+1 clock cycles
	//25000 clock cycles @25Mhz = 1 mS
	
	LPC_TIM0->MR0 = 500; //toggle time in mS
	LPC_TIM0->MCR = (1<<1); //Reset on MR0 Match
	LPC_TIM0->EMR |= (1<<5) | (1<<4) | (1<<7) | (1<<6); //Toggle Match output for MAT0.0(P1.28), MAT0.1(P1.29)
	//bits in LPC_TIM0->EMR[5:4] corresponds to MAT0.0 functionality, similarly LPC_TIM0->EMR[7:6] corresponds to MAT0.1
	
	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR = 0x01; //Enable timer
}
