#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define FOSC 7372800 
#define BAUD 9600
#define UBRR (FOSC/16/BAUD-1) //Value for UBBR0

void serial_init ( unsigned short ubrr ) {
  UBRR0 = ubrr ; // Set baud rate
  UCSR0B |= (1 << TXEN0); // Turn on transmitter
  UCSR0B |= (1 << RXEN0); // Turn on receiver
  UCSR0C = (3 << UCSZ00); // Set for async . operation , no parity ,
  // one stop bit , 8 data bits
}

char receive_rx(){
  // Wait for receive complete flag to go high
  while (!( UCSR0A & (1 << RXC0 )));
  return UDR0;
}
 
void send_tx(char c){
  // Wait for transmitter data register empty
  while ((UCSR0A & (1 << UDRE0)) == 0);
  UDR0 = c;
}
void output_string(char *str)
{
	while(*str != 0x00)
	{
		send_tx(*str);
		str++;
	}
}

void output_number(int x){
	char temp[4];
	sprintf(temp, "%d", x);
	output_string(temp);
}



void data(){
	int flag; 
	int index; 
	char buff[44]; // size of information needed
	int done = 0; 
	while(done == 0){
		_delay_ms(500);
		output_string(" -- ");
		output_string("\r\n");
		flag = 0; 
		index = 0; 
		if(flag ==0)
		{
			char c;
			c = receive_rx();
			while(c != '$'){
				c = receive_rx();
			}
			if(c == '$'){
				flag = 1;
				buff[index] = '$';
				index = index + 1; 
			}
		}
		if (flag == 1)
		{
			while( index < 6 )
			{	
				char letter =  receive_rx();
				buff[index] = letter;
				index = index + 1; 
			}
			buff[6]= 0;
			if(strcmp(buff, "$GPGGA")==0){
				flag = 2;
			}
			else{
				flag = 0;
			}
		}
		if (flag == 2)
		{
			index = 6; 
			char add = receive_rx();
			buff[index] = add;
			while (index < 43)
			{
				index = index + 1; 
				add = receive_rx();
				buff[index] = add; 
			}
			buff[44] = 0; 
			flag = 3; 
		}
		if(flag ==3)
		{
			int comma = 0;
			int n = 0; 
			while (comma <7)
			{
				if (buff[n] == ',')
				{
					comma++; 
				}
				if(comma == 6)
				{
					int j = n + 1; 
					if (buff[j] == '0')
					{
						output_string(" NOT FIXED ");
						flag = 0; 
					}
					if(buff[j] == '1')
					{
						//output_string(" GPS FIXED ");
						comma = 7; 
						flag = 4; 
					}
				}
				n++;
			}
			
		}
		if(flag == 4)
		{
			int i; 
			char time[10];
			int idx = 0;		
			for (i = 7; i<17; i++){
				time[idx] = buff[i];
				idx++;
			}
			//output_string(" TIME: ");
			//output_string(time);
			char North_South = buff[28];
			char East_West = buff[41];
			
			int minutes = (time[2] - '0') * 10 + (time[3] - '0');
			int seconds = (time[4] - '0') * 10 + (time[5] - '0');
			int UTChours = (time[0] - '0') * 10 + (time[1] - '0');
			if(UTChours<7)
			{
				UTChours = UTChours + 24; 	
			}
			int PST= UTChours - 7;
			
			
			
			output_string("\r\n");
			output_string(" CURRENT TIME: ");
			output_number(PST);
			output_string(":");
			output_number(minutes);
			if(PST < 12){
				output_string(" AM ");
			}
			else {
				output_string(" PM ");
			}
			
			output_string("\r\n");
			
			//$GPGGA,172759.000,3401.2424,N,11817.3361,W,1
			
			char latitude[10];
			int m = 0;	
			for (i = 18; i<27; i++){
				latitude[m] = buff[i];
				m++;
			}
			

			char longitude[10];
			m = 0;	
			for (i = 30; i<39; i++){
				longitude[m] = buff[i];
				m++;
			}
			
			
			output_string(" LATITUDE: " );
			output_string(latitude);
			output_string(" ");
			send_tx(North_South);
			output_string("\r\n");
			output_string(" LONGITUDE: ");
			output_string(longitude);
			output_string(" ");
			send_tx(East_West);
			output_string("\r\n");

			flag = 5; 
			
		}
		if (flag == 5)
		{
			output_string(" -- ");
			done = 1;
		}
	}
}


void check(){
	while (1) 
	{
		char input = receive_rx();
		send_tx(input);
	}  
}

void main(void)
{
	serial_init(UBRR);
	data();
	//check();
}
