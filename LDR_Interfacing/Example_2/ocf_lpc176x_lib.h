#ifndef OCF_LPC176X_LIB
#define OCF_LPC176X_LIB
/* Basic Support Library for OCFreaks LPC176x Tutorial Examples @ www.ocfreaks.com
Adapted from:
http://www.ocfreaks.com/lpc1768-timer-programming-tutorial/
http://www.ocfreaks.com/lpc1768-uart-programming-tutorial/
More Embedded stuff @ http://www.ocfreaks.com/cat/embedded/ 
*/

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

#define PRESCALE (25-1)

void initUART0(void);
void initTimer0(void);
void startTimer0(void);
unsigned int stopTimer0(void);
void delayUS(unsigned int microseconds);
void delayMS(unsigned int milliseconds);
void U0Write(char data);
char U0Read(void);

#endif
