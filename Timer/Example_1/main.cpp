/*(C) Umang Gajera - http://www.ocfreaks.com 2011-17.
http://www.ocfreaks.com/lpc1768-timer-programming-tutorial/
LPC1768 Timer example 1.

More Embedded tutorials @ http://www.ocfreaks.com/cat/embedded/
*/

#include <lpc17xx.h>

#define PRESCALE (25000-1)  //25000 PCLK clock cycles to increment TC by 1 

void delayMS(unsigned int milliseconds);
void initTimer0(void);

int main(void)
{
	//SystemInit(); //called by Startup Code before main(), hence no need to call again.
	initTimer0(); //Initialize Timer0 
	LPC_GPIO0->FIODIR = (1<<22); //Configure P0.22 as output

	while(1)
	{
		LPC_GPIO0->FIOSET = (1<<22); //Turn ON LED
		delayMS(500); //0.5 Second(s) Delay
		LPC_GPIO0->FIOCLR = (1<<22); //Turn LED OFF
		delayMS(500);
	}
	//return 0; //normally this won't execute
}

void initTimer0(void)
{
	/*Assuming that PLL0 has been setup with CCLK = 100Mhz and PCLK = 25Mhz.*/
	LPC_SC->PCONP |= (1<<1); //Power up Timer0. By default Timer0 and Timer1 are enabled.
	LPC_SC->PCLKSEL0 &= ~(0x3<<2); //Set PCLK for timer = CCLK/4 = 100/4 (default)
	
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = PRESCALE; //Increment LPC_TIM0->TC at every 24999+1 clock cycles
	//25000 clock cycles @25Mhz = 1 mS

	LPC_TIM0->TCR = 0x02; //Reset Timer
	//LPC_TIM0->TCR = 0x01; //Enable timer, done inside delayMS()
}

void delayMS(unsigned int milliseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR = 0x01; //Enable timer

	while(LPC_TIM0->TC < milliseconds); //wait until timer counter reaches the desired delay

	LPC_TIM0->TCR = 0x00; //Disable timer
}
