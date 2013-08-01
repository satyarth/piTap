#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

//#define sleep delayMicroseconds(0.9)
#define sleep mySleep()

void mySleep()
	{
	int n=0;
	for(int i=0;i<50;i++)
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
	digitalWrite(clock, 0); //Initializes clock to 1
	sleep;
	int arr[100],flag=0,counter=0, xthresh=0, xgot=0, ythresh=0, ygot=0;
	volatile uint16_t x=0,y=0,dx=0,dy=0;
	while(1)
	//while(counter<100)
		{
		x=y=dx=dy=0;
		digitalWrite(cs, 0); //Falling chip select initiates conversion and data transfer
		for(int i=0;i<2;i++)
			{
			digitalWrite(clock, 1);
			sleep;
			digitalWrite(clock, 0);
			sleep;
			}
		for(int j=0;j<12;j++) //Begin reading data at the falling edge after the null bit
			{
			digitalWrite(clock, 1);
			sleep;
		digitalWrite(clock, 0);
			sleep;
			dx = digitalRead(adc0);
			dy = digitalRead(adc1);
			x=x|(dx<<(11-j));
			y=y|(dy<<(11-j));
			}
		if((x>2300&&x<2999)&&xgot==0)
			{
			xthresh=counter;
			printf("%d\n",xthresh);
			xgot=1;
			}
		if((y>2300&&y<2999)&&ygot==0)
			{
			ythresh=counter;
			printf("%d\n",ythresh);
			ygot=1;
			}
		//arr[counter]=x;
		//if((x>2300&&x<2999)||y>2300)
		//	printf("%d\t%d\n",x,y);
		//	printf("%d\n",counter);
		digitalWrite(cs, 1); //Raise chip select
		sleep;
		counter++;
		}	
	for(int i=0;i<100;i++)
		printf("%d\n",arr[i]);
	}


int main() {
	piHiPri(99);
	if (wiringPiSetup () == -1)
		exit (1);
	//singleread(7,14,11);
	doubleread(7,14,11,1);
	return 0;
}
