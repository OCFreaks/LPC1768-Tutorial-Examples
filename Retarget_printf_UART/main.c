/* (C) Umang Gajera. www.ocfreaks.com
Example: Retarget printf to UART on LPC1768/LPC1769
UART code from OCFreaks LPC1768 UART Tutorial
@ http://www.ocfreaks.com/lpc1768-uart-programming-tutorial/
License: GPL.*/

#include <lpc17xx.h>
#include <stdio.h> //Obviously!
#include "uart.h"  //for initUART0() and other macros

struct __FILE
{
  int dummyVar; //Just for the sake of redefining __FILE, we won't we using it anyways ;)
};

FILE __stdout; //STDOUT
FILE __stdin;  //STDIN

int fputc(int c, FILE * stream)
{
	uartWrite(c); //Transmit Character
	return c; //return the character written to denote a successful write
}

int fgetc(FILE * stream)
{
	char c = uartRead(); //Receive Character
	uartWrite(c); //To echo Received characters back to serial Terminal
	return c;
}

void uartWrite(char c)
{
	while (!(LPC_UART0->LSR & THRE)); // wait till the THR is empty
	if( c == '\n' ) //Send <CR+LF>
	{
		LPC_UART0->THR = CARRIAGE_RETURN;
		while(!(LPC_UART0->LSR & THRE )); 
		LPC_UART0->THR = LINE_FEED;
	}
	else
	{
		LPC_UART0->THR = c; 
	}
}

char uartRead(void)
{
	while(!(LPC_UART0->LSR & RDR)); //wait for data to arrive
	return LPC_UART0->RBR;
}

int main(void)
{
	int num=0;
	//SystemInit(); //gets called by Startup code before main()
	initUART0(); //Init UART0 at baud-rate: 115200 @ 25Mhz PLCK
	
	printf("Hey! This is your beloved printf().\nI have been redirected here with scanf()!\n");
	
	while(1)
	{
		printf("Enter an Integer between 0 and 1000:");
		scanf("%d",&num);
		if(num>=0 && num<= 1000) printf("\nYou entered: %d\n",num);
		else printf("\nInvalid input!\n");
	}	
	
	//return 0; //This won't execute.
}

