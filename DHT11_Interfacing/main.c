/*(C) Umang Gajera - www.ocfreaks.com
DHT11 Humidity and Temperature Sensor Interfacing with LCP1768/LPC1769 Example Source Code for KEIL ARM
http://www.ocfreaks.com/interfacing-dht11-dht22-sensor-lpc1768/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License : GPL.*/
#include <lpc17xx.h>
#include <stdio.h> //For retargeted printf() - http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/
#include "ocf_lpc176x_lib.h" //contains initUART0(), initTimer0() & putc() to retarget printf()

#define LOW 0
#define HIGH 1
void printError(const char * str);
void checkResponse(unsigned int waitTimeUS, unsigned int margin, unsigned char pinValue);
char getDataBit(void);

#define DATA_PIN (1<<0) //Using P0.0 for data communication

int main(void)
{
	unsigned char dataBits[40] = {0};
	char dataBytes[5] = {0};
	
	initTimer0();
	initUART0();
	
	printf("OCFreaks.com - Interfacing DHT11 with LPC1768 Example.\n");
	
	while(1)
	{
		//STEP 1: Set pin to output HIGH which represents idle state
		LPC_GPIO0->FIODIR |= DATA_PIN;
		
		//STEP 2: Pull down pin for 18ms(min) to denote START
		LPC_GPIO0->FIOCLR |= DATA_PIN;
		delayUS(18000); //wait for 18ms
		
		//STEP 3: Pull HIGH and switch to input mode
		//pull-up will pull it HIGH after switching to input mode.
		LPC_GPIO0->FIODIR &= ~(DATA_PIN);
		
		//STEP 4: Wait between 20 to 40us for sensor to respond
		startTimer0();
		while((LPC_GPIO0->FIOPIN & DATA_PIN) != 0)
		{
			if(LPC_TIM0->TC > 40) break; //Timeout
		}
		unsigned int time = stopTimer0();
		
		if(time < 10 || time > 40) 
		{ 
			printError("Failed to communicate with sensor");
		}
		
		//STEP 5: Check for 80us LOW followed by 80us HIGH
		checkResponse(80,5,LOW);
		checkResponse(80,5,HIGH);
		
		//After this DHT sends data. Each bit has a preceding 50us LOW. After which 26-28us means '0' and 70us means '1'
		
		//STEP 6: Fetch data
		char data;
		for(int i=0; i < 40; i++)
		{
			data = getDataBit();
			
			if(data == 0 || data == 1)
			{
				dataBits[i] = data;
			}
			else printError("Data Error");
		}
		
		//STEP 7: Extract data bytes from array
		data = 0;
		for(int i=0; i<5; i++) // i is the BYTE counter
		{
			for(int j=0; j<8; j++) // j gives the current position of a bit in i'th BYTE
			{
				if( dataBits[ 8*i + j ] )
					data |= (1<<(7-j)); //we need to only shift 1's by ([BYTESZIE-1] - bitLocation) = (7-j)
			}
			dataBytes[i] = data;
			data = 0;
		}		
		
		printf("Humidity=%d%%, Temp=%d Deg. C\n",dataBytes[0], dataBytes[2]);
		
		//STEP8: Wait for atleast 1 second before probing again
		delayUS(1000000); 
	}
	//return 0;
}

void printError(const char * str)
{
	/*Print error and enter infinite loop to HALT program*/
	printf("%s\n",str);
	while(1);
}

void checkResponse(unsigned int waitTimeUS, unsigned int margin, unsigned char pinValue)
{
	int time = 0;
	int maxwait = waitTimeUS + margin;
	
	startTimer0();
	if(pinValue)
	{
		while(LPC_GPIO0->FIOPIN & DATA_PIN)
		{
			if(LPC_TIM0->TC > (maxwait)) break; 
		}
	}
	else
	{
		while( !(LPC_GPIO0->FIOPIN & DATA_PIN) )
		{
			if(LPC_TIM0->TC > (maxwait)) break; 
		}
	}
	time = stopTimer0();
	
	if(time < (waitTimeUS-margin) || time > maxwait) 
	{
		//printf("Error for wait=%d,margin=%d,pinVal=%d,time=%d\n",waitTimeUS,margin,pinValue,time);
		printError("checkResponse() Error"); //Out of range, including error margin
	}
}

char getDataBit(void)
{
	int time = 0;
	
	checkResponse(50,5,LOW); //Each data bit starts with 50us low
	
	startTimer0();
	while(LPC_GPIO0->FIOPIN & DATA_PIN)
	{
		if(LPC_TIM0->TC > 75)
		{
			//printError("Data Error");
			return 2; //2 = Error (Timeout for 50us LOW)
		}
	}
	time = stopTimer0();
	
	if((time > (27-10)) && (time < (27+10))) //I am getting 21 for HIGH using my DHT11 sensor, so using higher margin
	{
		return 0;
	}
	else if((time > (70-5)) && (time < (70+5)))
	{
		return 1;
	}
	else 
	{ 
		//printError("Data Error");
		return 2; //Timeout for data pulse 
	}
}
