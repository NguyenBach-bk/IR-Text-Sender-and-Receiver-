#include <REGX52.H>

void delay_hfms(unsigned int t) //Delay half millisecond
{
	unsigned int x, y;
	for(x=0; x<t; x++)
	{
		for(y=0; y<57; y++);
	}
}

void sendbit(bit b) 
{
	if(b) //Send bit 1, sink current
	{
		TR2 = 1;
		delay_hfms(1);
		TR2 = 0;
		P1_0 = 0;
		delay_hfms(3);
	}
	else //Send bit 0, sink current
	{
		TR2 = 1;
		delay_hfms(3);
		TR2 = 0;
		P1_0 = 0;
		delay_hfms(1);
	}
}

void sendbyte(unsigned char b) //Send byte through IR
{ 	
	unsigned char i;
	for(i = 0; i<8; i++)
	{
		sendbit(b&0x80);
		b<<=1;
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

char uart_data_ready() //Check if data has been received, return bool
{
	return RI;
}

char uart_read()   //Read byte from terminal
{
	RI = 0;
	return SBUF; 
}

void main() 
{
	P1_0 = 0;
	T2CON = 0x00;
	T2MOD = 0x02;
	RCAP2H = 0xFF;
	RCAP2L = 0xB7;
	uart_declr();
	while(1)
	{
		if(uart_data_ready())
		{
			uart_read();
			sendbyte(SBUF);
		}
	}
}