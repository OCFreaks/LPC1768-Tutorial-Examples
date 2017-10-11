#ifndef OCF_UART_HEADER
#define OCF_UART_HEADER

/* (C) Umang Gajera. www.ocfreaks.com
Taken from OCFreaks LPC1768 UART Tutorial
@ http://www.ocfreaks.com/lpc1768-uart-programming-tutorial/
License: GPL.*/

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

void initUART0(void);
void uartWrite(char c);
char uartRead(void);

#endif
