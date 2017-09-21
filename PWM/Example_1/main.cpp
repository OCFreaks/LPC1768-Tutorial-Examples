/*(C) Umang Gajera - www.ocfreaks.com
http://www.ocfreaks.com/lpc1768-timer-programming-tutorial/
LPC1768 PWM Tutorial Example 1 - Simple RC Servo Control.
License: GPL. 
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
*/

#include <LPC17xx.h>

#define PWMPRESCALE (25-1) //25 PCLK cycles to increment TC by 1 i.e. 1 Micro-second

void initPWM(void);
void updatePulseWidth(unsigned int pulseWidth);
void delayMS(unsigned int milliseconds);
void initTimer0(void);

int main(void)
{
	//SystemInit(); //gets called by Startup code before main()
	initTimer0(); //Initialize Timer
	initPWM(); //Initialize PWM
	
	int pulseWidths[] = {1000,1250,1500,1750,2000};
	const int numPulseWidths = 5;
	int count=1;
	int dir=0; //direction, 0 = Increasing, 1 = Decreasing 

	while(1)
	{
		updatePulseWidth(pulseWidths[count]); //Update Servo Pulse Width
		delayMS(1000); //Wait for servo to reach the new position

		if(count == (numPulseWidths-1) || count == 0)
		{
			dir = !dir; //Toggle direction if we have reached count limit
		}
		
		if(dir) count--;
		else count++;
	}
  //return 0; //normally this won't execute ever
}

void initPWM(void)
{
	/*Assuming that PLL0 has been setup with CCLK = 100Mhz and PCLK = 25Mhz.*/

	LPC_PINCON->PINSEL3 |= (1<<5); //Select PWM1.1 output for Pin1.18
	LPC_PWM1->PCR = 0x0; //Select Single Edge PWM - by default its single Edged so this line can be removed
	LPC_PWM1->PR = PWMPRESCALE; //1 micro-second resolution
	LPC_PWM1->MR0 = 20000; //20000us = 20ms period duration
	LPC_PWM1->MR1 = 1250; //1ms - default pulse duration i.e. width
	LPC_PWM1->MCR = (1<<1); //Reset PWM TC on PWM1MR0 match
	LPC_PWM1->LER = (1<<1) | (1<<0); // update values in MR0 and MR1
	LPC_PWM1->PCR = (1<<9); //enable PWM output
	LPC_PWM1->TCR = (1<<1); //Reset PWM TC & PR

	LPC_PWM1->TCR = (1<<0) | (1<<3); //enable counters and PWM Mode

	//PWM Generation goes active now!
	//Now you can get the PWM output on Pin P1.18
}

void updatePulseWidth(unsigned int pulseWidth)
{
	LPC_PWM1->MR1 = pulseWidth; //Update MR1 with new value
	LPC_PWM1->LER = (1<<1); //Load the MR1 new value at start of next cycle
}


void delayMS(unsigned int milliseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR = 0x01; //Enable timer

	while(LPC_TIM0->TC < milliseconds); //wait until timer counter reaches the desired delay

	LPC_TIM0->TCR = 0x00; //Disable timer
}

void initTimer0(void) //To setup Timer0 used delayMS() function
{
	/*Assuming that PLL0 has been setup with CCLK = 100Mhz and PCLK = 25Mhz.*/

	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = 25000-1; //Increment TC at every 24999+1 clock cycles
	//25000 clock cycles @25Mhz = 1 mS

	LPC_TIM0->TCR = 0x02; //Reset Timer
}
