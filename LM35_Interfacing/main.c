/*(C) Umang Gajera - www.ocfreaks.com
Interfacing LM35 Temperature Sensor with LPC1768/LPC1769 - Example Source Code for KEIL ARM.
http://www.ocfreaks.com/interfacing-lm35-temperature-sensor-lpc1768/
Also see: http://www.ocfreaks.com/lpc1768-adc-programming-tutorial/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License : GPL.*/

#include <lpc17xx.h>
#include <stdio.h> //For retargeted printf() - http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ 
#include "ocf_lpc176x_lib.h" //contains initUART0(), initTimer0() & putc() to retarget printf() 

#define ADC_CLK_EN (1<<12)
#define SEL_AD0_0  (1<<0) //Select Channel AD0.0 
#define CLKDIV     3 //ADC clock-divider (ADC_CLOCK=PCLK/CLKDIV+1) = 12.5Mhz @ 25Mhz PCLK
#define PWRUP      (1<<21) //setting it to 0 will power it down
#define START_CNV  (1<<24) //001 for starting the conversion immediately
#define ADC_DONE   (1U<<31) //define it as unsigned value or compiler will throw #61-D warning
#define ADCR_SETUP_SCM ((CLKDIV<<8) | PWRUP)
#define VREF       3.3 //Reference Voltage at VREFP pin, given VREFN = 0V(GND) - not used in this example.

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initUART0(); //Initialize UART0 for retargeted printf() 
	initTimer0(); //For delayMS()

	LPC_SC->PCONP |= ADC_CLK_EN;
	LPC_PINCON->PINSEL1 |= (1<<14); //select AD0.0 for P0.23
	LPC_ADC->ADCR = ADCR_SETUP_SCM | SEL_AD0_0;
	int result = 0;
	float temp = 0;
	
	printf("OCFreaks.com LPC176x LDR Interfacing - Example 1.\n");
	
	while(1)
	{
		LPC_ADC->ADCR |= START_CNV; //Start new Conversion (Software controlled)

		while((LPC_ADC->ADDR0 & ADC_DONE) == 0); //Wait untill conversion is finished
		
		result = (LPC_ADC->ADDR0>>4) & 0xFFF; //12 bit Mask to extract result
		
		temp = ((float)result * VREF * 100)/4096; //As per the Equation given in the tutorial

		printf("Temp = %0.1f Deg. Celsius\n" , temp);

		delayMS(500); //Slowing down Updates to 2 Updates per second
	}
	
	//return 0; //This won't execute
}
