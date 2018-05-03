/*(C) Umang Gajera - www.ocfreaks.com
LPC176x IR Proximity/Obstacle-avoidance Sensor Interfacing Example 2 Source Code for KEIL ARM.
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
Also refer: http://www.ocfreaks.com/lpc1768-adc-programming-tutorial/
License : GPL.*/

#include <lpc17xx.h>
#include <stdio.h> //for printf() - http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/
#include "ocf_lpc176x_lib.h" //contains initUART0(), initTimer0() & putc() to retarget printf()
#define PIN0_1 (1<<1)

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initUART0(); //Initialize UART0 for uart_printf() - defined in ocf_lpc176x_lib.c
	initTimer0(); //For delayMS() - defined in ocf_lpc176x_lib.c
	
	printf("OCFreaks.com LPC176x ADC Tutorial Example 1.\nSoftware Controlled ADC Mode on AD0.0 Channel.\n");
	
	while(1)
	{
		//Check the O/P Logic of your IR module. Mine gives LOW when obstacle is detected else HIGH.
		if((LPC_GPIO0->FIOPIN & PIN0_1) == 0 ) 
		{
			printf("Obstacle Detected!\n");
		}
		else
		{
			printf("No Obstacle ahead.\n");
		}

		delayMS(500); //Slowing down Updates to 2 Updates per second
	}
	//return 0; //This won't execute
}

