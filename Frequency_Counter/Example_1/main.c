/*(C) Umang Gajera - www.ocfreaks.com
LPC176x Input Capture Tutorial - Example 1(Using Gating) for frequency counter using ARM KEIL
http://www.ocfreaks.com/lpc1768-timer-input-capture-frequency-counter-tutorial/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License: GPL.*/

#include <lpc17xx.h>
#include <stdio.h> //for printf() - http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/
#include "ocf_lpc176x_lib.h" //contains initUART0(), initTimer0() & putc() to retarget printf()

void initPWM(void);
unsigned int pulses = 0;
#define GATE_TIME_MS 1000 // Probing/Gating Time in ms

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initUART0(); //Initialize UART0 for uart_printf() - see ocf_lpc176x_lib.c
	initTimer0(); //For delayMS() - see ocf_lpc176x_lib.c
	
	/*Using CCLK = 100Mhz and PCLK_TIMER2 = 100Mhz.*/
	LPC_SC->PCONP |= (1<<22); //Power-Up Timer2 module. It is disabled by default.
	LPC_SC->PCLKSEL1 |= (1<<12); //Set bits[13:12] = [01] to select PCLK_TIMER2 = CCLK i.e. 100Mhz in our case. 
	LPC_PINCON->PINSEL0 |= (1<<9) | (1<<8); //Set Bits[9:8] = [11] to Select CAP2.0 for P0.4
	LPC_TIM2->CTCR = 0x1; //Increment TC on rising edges of External Signal for CAP2.0
	LPC_TIM2->PR = 0; //Using lowest PR gives most accurate results
	LPC_TIM2->CCR = 0x0; //Must be [000] for selected CAP input
	LPC_TIM2->TCR = 0x2; //Reset & Disable Timer2 Initially
	
	initPWM(); //To generate test square wave signal	
	float FreqKhz = 0;
	printf("OCFreaks.com - LPC1768x Frequency Counter Example 1:\n");

	while(1)
	{
		LPC_TIM2->TCR = 0x1; //Start Timer2
		delayMS(GATE_TIME_MS); //'Gate' signal for defined Time (1 second)
		LPC_TIM2->TCR = 0x0; //Stop Timer2
		
		pulses = LPC_TIM2->TC; //Read current value in TC, which contains no.of. pulses counted in 1s
		LPC_TIM2->TCR = 0x2; //Reset Timer2 TC
		
		FreqKhz = (double)pulses/GATE_TIME_MS;
		
		if(FreqKhz >= 1000.0) //Display Freq. In MHz
		{
			printf("Frequency = %0.4f MHz\n", FreqKhz/1000.0);
		}
		else //Display Freq. in KHz
		{
			printf("Frequency = %0.2f KHz\n", FreqKhz);
		}
	}
	
	//return 0; //This won't execute normally
}


void initPWM(void)
{
	//Refer: http://www.ocfreaks.com/lpc1768-pwm-programming-tutorial/
	/*Using CCLK = 100Mhz and PCLK_PWM1 = 100Mhz.*/
	
	//By default PWM1 block is powered-on
	LPC_SC->PCLKSEL0 |= (1<<12); //Set bits[13:12] = [01] to select PCLK_PWM1 = CCLK i.e. 100Mhz in our case. 
	LPC_PINCON->PINSEL4 |= (1<<0); // Select PWM1.1 output for Pin2.0
	LPC_PWM1->PCR = 0x0; //Select Single Edge PWM - by default its single Edged so this line can be removed
	LPC_PWM1->PR = 0; //PR+1 = 0+1 = 1 Clock cycle @100Mhz = 0.01us = 10ns
	LPC_PWM1->MR0 = 4; //4x0.01 = 0.04us - period duration i.e. 25Mhz Test frequency
	LPC_PWM1->MR1 = 2; //2x0.01 = 0.02us - pulse duration (50% duty cycle)
	LPC_PWM1->MCR = (1<<1); // Reset PWMTC on PWMMR0 match
	LPC_PWM1->LER = (1<<1) | (1<<0); // update MR0 and MR2
	LPC_PWM1->PCR = (1<<9); // enable PWM1.1 output
	LPC_PWM1->TCR = (1<<1) ; //Reset PWM TC & PR

	LPC_PWM1->TCR = (1<<0) | (1<<3); // enable counters and PWM Mode
	//PWM Generation goes active now!!
}
