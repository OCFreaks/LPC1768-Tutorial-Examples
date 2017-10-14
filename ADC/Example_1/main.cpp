/*(C) Umang Gajera - www.ocfreaks.com
LPC1768/LPC1769 ADC Example 1 Source Code using KEIL ARM
http://www.ocfreaks.com/lpc1768-adc-programming-tutorial/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License : GPL.*/

#include <lpc17xx.h>
#include <stdio.h>
#include "ocf_lpc176x_lib.h" //contains initUART0, initTimer0 & putc to retarget printf 

#define ADC_CLK_EN (1<<12)
#define SEL_AD0_0  (1<<0) //Select Channel AD0.0 
#define CLKDIV     1 //ADC clock-divider (ADC_CLOCK=PCLK/CLKDIV+1) = 12.5Mhz @ 25Mhz PCLK
#define PWRUP      (1<<21) //setting it to 0 will power it down
#define START_CNV  (1<<24) //001 for starting the conversion immediately
#define ADC_DONE   (1U<<31) //define it as unsigned value or compiler will throw #61-D warning
#define ADCR_SETUP_SCM ((CLKDIV<<8) | PWRUP)
#define VREF       3.3 //Reference Voltage at VREFP pin, given VREFN = 0V(GND)

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initUART0(); //Initialize UART0 for uart_printf() - both defined in tmr_uart_printf.cpp
	initTimer0(); //For delayMS() - both defined in tmr_uart_printf.cpp

	LPC_SC->PCONP |= ADC_CLK_EN;
	LPC_PINCON->PINSEL1 |= (1<<14); //select AD0.0 for P0.23
	LPC_ADC->ADCR =  ADCR_SETUP_SCM | SEL_AD0_0;
	int result = 0;
	
	printf("OCFreaks.com LPC176x ADC Tutorial Example 1.\nSoftware Controlled ADC Mode on AD0.0 Channel.\n");
	
	while(1)
	{
		LPC_ADC->ADCR |= START_CNV; //Start new Conversion

		while((LPC_ADC->ADDR0 & ADC_DONE) == 0); //Wait untill conversion is finished
		
		result = (LPC_ADC->ADDR0>>6) & 0x3ff; //12 bit Mask to extract result
		printf("AD0.0 = %dmV\n" , (int)( result*VREF ));
		delayMS(500); //Slowing down Updates to 2 Updates per second
	}
	
	//return 0; //This won't execute
}

