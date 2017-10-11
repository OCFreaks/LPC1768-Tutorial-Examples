/* (C) Umang Gajera. www.ocfreaks.com
Taken from OCFreaks LPC1768 UART Tutorial
@ http://www.ocfreaks.com/lpc1768-uart-programming-tutorial/
License: GPL.*/

#include <lpc17xx.h>

#define RDR         (1<<0)
#define THRE        (1<<5) //Transmit Holding Register Empty
#define MULVAL      15
#define DIVADDVAL   2
#define Ux_FIFO_EN  (1<<0)
#define Rx_FIFO_RST (1<<1)
#define Tx_FIFO_RST (1<<2)
#define DLAB_BIT    (1<<7)
#define LINE_FEED   0x0A //LF, For Linux, MAC and Windows Terminals  
#define CARRIAGE_RETURN 0x0D //CR, For Windows Terminals (CR+LF).

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
