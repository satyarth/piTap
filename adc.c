#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define sleep delayMicroseconds(3)


void mySleep()
	{
	int n=0;
	for(int i=0;i<3000;i++)
		{
		n+=1;
		asm volatile("":::);
		}
	}

int singleread(int clock, int cs, int adc0) {	//Input variables refer to the pin numbers of the clock, chip select and adc output pins respectively
	pinMode(clock,OUTPUT);
	pinMode(cs, OUTPUT);
	pinMode(adc0, INPUT);
	digitalWrite(cs, 1); //Initializes chip select to 1
	digitalWrite(clock, 1); //Initializes clock to 1
	while(1)
		{
		uint16_t x=0;
		digitalWrite(cs, 0); //Falling chip select initiates conversion and data transfer
		for(int i=0;i<2;i++)
			{
			digitalWrite(clock, 0);
			sleep;
			digitalWrite(clock, 1);
			sleep;
			}
		for(int j=0;j<12;j++) //Begin reading data at the falling edge after the null bit
			{
			digitalWrite(clock, 0);
			sleep;
			x=x|((digitalRead(adc0)<<(11-j)));
			digitalWrite(clock, 1);
			sleep;
			}
		//if(x>2300)
			printf("%d \n",x);
		digitalWrite(cs, 1); //Raise chip select
		sleep;
		}	
	}

int doubleread(int clock, int cs, int adc0, int adc1) {	//Input variables refer to the pin numbers of the clock, chip select and adc output pins respectively
	pinMode(clock,OUTPUT);
	pinMode(cs, OUTPUT);
	pinMode(adc0, INPUT);
	pinMode(adc1, INPUT);
	digitalWrite(cs, 1); //Initializes chip select to 1
	digitalWrite(clock, 1); //Initializes clock to 1
	sleep;
	int flag=0,counter=0, xthresh=0, xgot=0, ythresh=0, ygot=0;
	volatile uint16_t x=0,y=0,dx=0,dy=0;
	while(1)
		{
		x=y=dx=dy=0;
		digitalWrite(cs, 0); //Falling chip select initiates conversion and data transfer
		sleep;
		for(int i=0;i<2;i++)
			{
			digitalWrite(clock, 0);
			sleep;
			digitalWrite(clock, 1);
			sleep;
			}
		for(int j=0;j<12;j++) //Begin reading data at the falling edge after the null bit
			{
			digitalWrite(clock, 0);
			sleep;
			dx = digitalRead(adc0);
			dy = digitalRead(adc1);
			//int dy = 1;
			x=x|(dx<<(11-j));
			y=y|(dy<<(11-j));
			digitalWrite(clock, 1);
			sleep;
			}
		//if(x>2200||y>2200)
		//	flag=1;
		//if((x>2200||x<1000)&&xgot==0)
		//	{
		//	xthresh=counter;
		//	xgot=1;
		//	}
		//if((y>2200|y<1000)&&ygot==0)
		//	{
		//	ythresh=counter;
		//	ygot=1;
		//	}
		//if(x>2300||y>2300)
		//	printf("%d\t%d\t%d\n",xthresh,ythresh,counter);
		printf("%d\t%d\n",x,y);
		digitalWrite(cs, 1); //Raise chip select
		sleep;
		sleep;
		counter++;
		}	
	//printf("%d\t%d\n",x,y);
	}


int main() {
	if (wiringPiSetup () == -1)
		exit (1);
	//singleread(7,14,11);
	doubleread(7,14,11,1);
	return 0;
}
