#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "serial.h"
#include "gps.h"
#include "tft_lcd.h"

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

void gps_get_data(){
	// draw_text(120, 0, "GPS", LCD_WHITE, 2);
	int flag;
	int index;
	char buff[44]; // size of information needed
	int done = 0;
	while(done == 0){
		_delay_ms(500);
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
						//output_string(" NOT FIXED ");
						//draw_text(0, 0, ".", LCD_WHITE, 1); // indicate not fixed
						flag = 0;
						done = 0;
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
			char North_South[1];
			North_South[0] = buff[28];
			char East_West = buff[41];

			int minutes = (time[2] - '0') * 10 + (time[3] - '0');
			//int seconds = (time[4] - '0') * 10 + (time[5] - '0');
			int UTChours = (time[0] - '0') * 10 + (time[1] - '0');
			if(UTChours<7)
			{
				UTChours = UTChours + 24;
			}
			int PST= UTChours - 7;

			char mins [2];
			char data [20];

			sprintf(mins, "%d", minutes);
			sprintf(data, "%d", PST);

			strcat(data, ":");
			strcat(data, mins);

			if(PST < 12){
				strcat(data, "AM");
			}
			else {
				strcat(data, "PM");
			}

			// output to the LCD
			draw_text(120, 70, data, LCD_WHITE, 1);

			char latitude[14];
			int m = 0;
			for (i = 18; i<29; i++){
				latitude[m] = buff[i];
				m++;
			}

			draw_text(100, 120, latitude, LCD_WHITE, 1);

			char longitude[15];
			m = 0;
			for (i = 30; i<43; i++){
				longitude[m] = buff[i];
				m++;
			}
			draw_text(90, 170, longitude, LCD_WHITE, 1);

			flag = 5;

		}
		if (flag == 5)
		{
			done = 1;
		}
	}
}
