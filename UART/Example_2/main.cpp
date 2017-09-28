/*(C) Umang Gajera - www.ocfreaks.com
http://www.ocfreaks.com/lpc1768-uart-programming-tutorial/
LPC1768 UART Tutorial Example 1 - Source Code.
License : GPL.
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
*/

#include <lpc17xx.h>

#define RDR         (1<<0) //Receiver Data Ready
#define THRE        (1<<5) //Transmit Holding Register Empty
#define MULVAL      15
#define DIVADDVAL   2
#define Ux_FIFO_EN  (1<<0)
#define Rx_FIFO_RST (1<<1)
#define Tx_FIFO_RST (1<<2)
#define DLAB_BIT    (1<<7)
#define LINE_FEED   0x0A //LF, For Linux, MAC and Windows Terminals  
#define CARRIAGE_RETURN 0x0D //CR, For Windows Terminals (CR+LF).
#define ENTER       CARRIAGE_RETURN //Ascii value/code for Enter is 0x0D i.e. CR

void initUART0(void);
char U0Read(void);
void U0Write(char data);

int main(void)
{
	initUART0();
	char data = 0;
	
	while(1)		
	{
		data = U0Read(); //Read Data from Rx
		if(data == ENTER) //Check if user pressed Enter key
		{
			//Send NEW Line Character(s) i.e. "\n"
			U0Write(CARRIAGE_RETURN); //Comment this for Linux or MacOS
			U0Write(LINE_FEED); //Windows uses CR+LF for newline.
		}
		else
		{
			U0Write(data); //Tx Read Data back
		}
	}
	//return 0; //Normally this won't execute
}


char U0Read(void)
{
	while(!(LPC_UART0->LSR & RDR)); //wait until data arrives in Rx FIFO
	return LPC_UART0->RBR;
}

void U0Write(char txData)
{
	while(!(LPC_UART0->LSR & THRE)); //wait until THR is empty
	//now we can write to Tx FIFO
	LPC_UART0->THR = txData;
}

void initUART0(void)
{
	/*Assuming CCLK = 100Mhz and PCLK = 25Mhz!*/
	LPC_PINCON->PINSEL0 |= (1<<4) | (1<<6); //Select TXD0 and RXD0 function for P0.2 & P0.3!
	//LPC_SC->PCONP |= 1<<3; //Power up UART0 block. By Default it is enabled after RESET.

	LPC_UART0->LCR = 3 | DLAB_BIT ; /* 8 bits, no Parity, 1 Stop bit & DLAB set to 1  */
	LPC_UART0->DLL = 12;
	LPC_UART0->DLM = 0;
	
	//LPC_UART0->IER |= ..; //Edit this if want you to use UART interrupts
	LPC_UART0->FCR |= Ux_FIFO_EN | Rx_FIFO_RST | Tx_FIFO_RST;
	LPC_UART0->FDR = (MULVAL<<4) | DIVADDVAL; /* MULVAL=15(bits - 7:4) , DIVADDVAL=2(bits - 3:0)  */
	LPC_UART0->LCR &= ~(DLAB_BIT);
	//Now since we have applied DLL and DLM we now lock or freeze those valuse by diabling DLAB i.e DLAB=0 
	//Baud= ~115200(114882). Now we can perform UART communication!
}
