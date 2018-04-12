/*(C) Umang Gajera - www.ocfreaks.com
Interfacing HC-SR04 Ultrasonic Distance/Ranging sensor with LPC1768/LPC1769 - Example Source Code for KEIL ARM.
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License : GPL.*/

#include <lpc17xx.h>
#include <stdio.h> //for printf() - http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/
#include "ocf_lpc176x_lib.h" //contains initUART0(), initTimer0() & putc() to retarget printf()

#define TRIG (1<<0) //P0.0
#define ECHO (1<<1) //P0.1

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initUART0(); //Initialize UART0 for retargeted printf() - defined in ocf_lpc176x_lib.c
	initTimer0(); //Init Timer for delay functions - defined in ocf_lpc176x_lib.c
	int echoTime=0;
	float distance=0;

	LPC_GPIO0->FIODIR |= TRIG;    //Set P0.2(TRIG) as output
	LPC_GPIO0->FIODIR &= ~(ECHO); //Set P0.3(ECHO) as input (explicitly)
	LPC_GPIO0->FIOCLR |= TRIG;    //Set P0.2 LOW initially

	printf("OCFreaks.com LPC176x HC-SR04 Sensor Interfacing Tutorial\n");

	while(1)
	{
		//Output 10us HIGH on TRIG pin
		LPC_GPIO0->FIOPIN |= TRIG;
		delayUS(10);
		LPC_GPIO0->FIOCLR |= TRIG;

		while(!(LPC_GPIO0->FIOPIN & ECHO)); //Wait for a HIGH on ECHO pin
		startTimer0(); //Start counting
		while(LPC_GPIO0->FIOPIN & ECHO); //Wait for a LOW on ECHO pin
		echoTime = stopTimer0(); //Stop counting and save value(us) in echoTime

		distance = (0.0343 * echoTime)/2; //Find the distance

		printf("Distance = %0.2fcm\n",distance);
		
		delayMS(1000); //1 update per second
	}
	//return 0; //This won't execute normally
}
