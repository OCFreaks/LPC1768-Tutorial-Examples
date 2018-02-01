/*(C) Umang Gajera - www.ocfreaks.com
LPC1768/LPC1769 ADC Example 2 Source Code using KEIL ARM
http://www.ocfreaks.com/interfacing-ldr-lpc1768/
Also see: http://www.ocfreaks.com/lpc1768-adc-programming-tutorial/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License : GPL.*/

#include <lpc17xx.h>
#include "ocf_lpc176x_lib.h"

#define ADC_CLK_EN (1<<12)
#define SEL_AD0_0  (1<<0) //Select Channel AD0.0 
#define CLKDIV     1 //ADC clock-divider (ADC_CLOCK=PCLK/CLKDIV+1) = 12.5Mhz @ 25Mhz PCLK
#define PWRUP      (1<<21) //setting it to 0 will power it down
#define START_CNV  (1<<24) //001 for starting the conversion immediately
#define ADC_DONE   (1U<<31) //define it as unsigned value or compiler will throw #61-D warning
#define ADCR_SETUP_SCM ((CLKDIV<<8) | PWRUP)
//#define VREF       3.3 //Reference Voltage at VREFP pin, given VREFN = 0V(GND) - not used in this example.

#define LDR_CUT_OFF 750 //Define your cut-off value here

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initTimer0(); //For delayMS()

	LPC_SC->PCONP |= ADC_CLK_EN;
	LPC_PINCON->PINSEL1 |= (1<<14); //select AD0.0 for P0.23
	LPC_ADC->ADCR = ADCR_SETUP_SCM | SEL_AD0_0;
	LPC_GPIO0->FIODIR |= (1<<0); //Set P0.0 as output
	LPC_GPIO0->FIOCLR |= (1<<0); //LED initially OFF
	
	int result = 0;
	
	while(1)
	{
		LPC_ADC->ADCR |= START_CNV; //Start new Conversion (Software controlled)

		while((LPC_ADC->ADDR0 & ADC_DONE) == 0);
		
		result = (LPC_ADC->ADDR0>>4) & 0xFFF; //12 bit Mask to extract result
		
		if(result < LDR_CUT_OFF)
		{
			LPC_GPIO0->FIOSET |= (1<<0); //set P0.0 HIGH - LED ON
		}
		else
		{
			LPC_GPIO0->FIOCLR |= (1<<0); //set P0.0 LOW - LED OFF
		}
		
		delayMS(50); //wait some time since LDRs don't react immediately.
	}
	//return 0; //This won't execute normally
}
