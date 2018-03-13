/* Basic Support Library for OCFreaks LPC176x Tutorial Examples @ www.ocfreaks.com
Adapted from:
http://www.ocfreaks.com/lpc1768-timer-programming-tutorial/
http://www.ocfreaks.com/lpc1768-uart-programming-tutorial/
More Embedded stuff @ http://www.ocfreaks.com/cat/embedded/ 
*/

#include <lpc17xx.h>
#include "ocf_lpc176x_lib.h"
#include <stdio.h>

//Retarget printf() - check: http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/
struct __FILE
{
  int dummyVar; //Just for the sake of redefining __FILE, we won't we using it anyways ;)
};

FILE __stdout;
FILE __stdin;

int fputc(int c, FILE * stream)
{
	U0Write(c);
	return c; //return the character written to denote a successfull write
}

int fgetc(FILE * stream)
{
	char c = U0Read();
	U0Write(c); //To echo Received characters back to serial Terminal
	return c;
}

void initUART0(void) //PCLK must be = 25Mhz!
{
	LPC_PINCON->PINSEL0 |= (1<<4) | (1<<6); //Select TXD0 and RXD0 function for P0.2 & P0.3!
	//LPC_SC->PCONP |= 1<<3; //Power up UART0 block. By Default it is enabled after RESET.
	LPC_UART0->LCR = 3 | DLAB_BIT ; /* 8 bits, no Parity, 1 Stop bit & DLAB set to 1  */
	LPC_UART0->DLL = 12;
	LPC_UART0->DLM = 0;
	LPC_UART0->FCR |= Ux_FIFO_EN | Rx_FIFO_RST | Tx_FIFO_RST;
	LPC_UART0->FDR = (MULVAL<<4) | DIVADDVAL; /* MULVAL=15(bits - 7:4) , DIVADDVAL=2(bits - 3:0)  */
	LPC_UART0->LCR &= ~(DLAB_BIT);
	//Baud= ~115200(114882). Now we can perform UART communication!
}

void initTimer0(void) //PCLK must be = 25Mhz!
{
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = PRESCALE; //Increment TC at every 24999+1 clock cycles
	LPC_TIM0->TCR = 0x02; //Reset Timer
}

void startTimer0(void)
{
	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR = 0x01; //Enable timer
}

unsigned int stopTimer0(void)
{
	LPC_TIM0->TCR = 0x00; //Disable timer
	return LPC_TIM0->TC;
}

void delayUS(unsigned int microseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR = 0x01; //Enable timer
	while(LPC_TIM0->TC < microseconds); //wait until timer counter reaches the desired delay
	LPC_TIM0->TCR = 0x00; //Disable timer
}

void delayMS(unsigned int milliseconds) //Using Timer0
{
	delayUS(milliseconds * 1000);
}

void U0Write(char cChar)
{
	while ( !(LPC_UART0->LSR & THRE) ); // wait till the THR is empty
	// now we can write to the queue
	if( cChar == '\n' ) //Send <CR+LF>
	{
		LPC_UART0->THR = CARRIAGE_RETURN;
		while( !(LPC_UART0->LSR & THRE )); 
		LPC_UART0->THR = LINE_FEED;
	}
	else
	{
		LPC_UART0->THR = cChar;
	}
}

char U0Read(void)
{
	while( !(LPC_UART0->LSR & RDR )); 
	return LPC_UART0->RBR; 
}
