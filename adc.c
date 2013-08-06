#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

//#define sleep delayMicroseconds(0.9)
#define sleep mySleep()
#define length 500
#define conditionx (x<1500&&x>1200)||(x>2300&&x<2999)
#define conditiony (y<1500&&y>1200)||(y>2300&&y<2999)

void mySleep() //Custom sleep function, wrote it to have greater control of sleep timings than delayMictoseconds(int)
	{
	int n=0;
	for(int i=0;i<1;i++)
		{
		n+=1;
		asm volatile("":::);
		}
	}

int isLegit(int n)
	{
	if(n==0||n==1||n==3||n==7||n==15||n==31||n==63||n==127||n==255||n==511||n==1023)
		return(0);
	else
		return(1);
	}

int doubleread(int clock, int cs, int adc0, int adc1) {	//Input variables refer to the pin numbers of the clock, chip select and adc output pins respectively
	pinMode(clock,OUTPUT);
	pinMode(cs, OUTPUT);
	pinMode(adc0, INPUT);
	pinMode(adc1, INPUT);
	digitalWrite(cs, 1); //Initializes chip select to 1
	digitalWrite(clock, 0); //Initializes clock to 1
	sleep;
	int arrx[length],arry[length],flagx=0,flagy=0,counter=0, xthresh=0, xgot=0, ythresh=0, ygot=0, xlast, ylast, event=0;
	//arrx = array containing last length x values
	//arry = array containing last length y values
	volatile uint16_t x=0,y=0,dx=0,dy=0;
	//while(1)
	//while(counter<100)
	while((xgot==0)||(ygot==0))	//Condition which terminates the loop after both x,y channels have been triggered
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
			dx = digitalRead(adc0); //Adding an extra digitalRead() here seems to fix a lot of the shoddy values... not too sure why but it works!
			dx = digitalRead(adc0);
			dy = digitalRead(adc1);
			x=x|(dx<<(11-j));
			y=y|(dy<<(11-j));
			}
		arrx[counter%length]=x;
		arry[counter%length]=y;
		if(isLegit(x)&&(x<1800||x>2200))
			flagx++;
		else
			flagx=0;
		if(isLegit(y)&&(y<1800||y>2200))
			flagy++;
		else
			flagy=0;
		if(flagx==2&&xgot==0) //Event considered triggered when two consecutive legit values are not in range [1800,2200]
			{
			xthresh=counter;
			xgot=1;
			if(ygot==1)
				event=counter;
			xlast=x;
			}
		if(flagy==2&&ygot==0) //Same as above on y channel
			{
			ythresh=counter;
			ygot=1;
			if(xgot==1)
				event=counter;
			ylast=y;
			}
		if(xgot==1&&((counter-xthresh)>700)) //Resets xgot,xthresh if y is untriggered 700 loops after x is triggered
			{
			xgot=0;
			xthresh=0;
			}
		if(ygot==1&&((counter-ythresh)>700)) //Resets ygot,ythresh if x is untriggered 700 loops after y is triggered
			{
			ygot=0;
			ythresh=0;
			}
		digitalWrite(cs, 1); //Raise chip select
		sleep;
		counter++;
		}	
	for(int i=(event%length)+1;i<length;i++)
		printf("%d\t%d \n",arrx[i],arry[i]);
	for(int i=0;i<(event%length)+1;i++)
		printf("%d\t%d \n",arrx[i],arry[i]);
	printf("%d\t%d x\n",xthresh,xlast);
	printf("%d\t%d y\n",ythresh,ylast);
	printf("%d",xthresh-ythresh);
	}



int main() {
	piHiPri(99);
	if (wiringPiSetup () == -1)
		exit (1);
	doubleread(7,14,11,1);
	return 0;
}
