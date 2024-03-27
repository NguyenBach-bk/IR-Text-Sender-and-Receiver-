#include <REGX52.H>

void delay_hfms(unsigned int t) //Delay half millisecond
{
	unsigned int x, y;
	for(x=0; x<t; x++)
	{
		for(y=0; y<57; y++);
	}
}

void uart_declr() //Declare mode 1 UART, 300 baud
{
	SM0 = 0; SM1 = 1;       //Select mode 1 UART
	TMOD = 0x20;            //0010 xxxx - 8 bit timer 1 auto-reload   		
	TH1 = 0xA0;             //Baudspeed 300
	TR1 = 1;                //Start timer 1
	TI = 1;                 //Ready to send data
	REN = 1;                //Allow receive data
}

void uart_write(char c) //Send byte to terminal
{
	while(TI == 0);		
	TI = 0;
	SBUF = c;
}

void main()
{
	unsigned char b, i;
	uart_declr();
 	while(1)
	{
		while(P1_0);
		b=0x00;	
		for(i=0;i<8;i++)
		{
			delay_hfms(2);
			if(P1_0)
			{
			 	b=(b<<1)|(0x01);
			}
			else
			{
				b=b<<1;
			 	while(P1_0==0);
			}
			while(P1_0==1);
		}
	  uart_write(b);
	}
}
