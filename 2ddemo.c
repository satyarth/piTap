#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define clockpin 7
#define cspin 14
#define adc0pin 4
#define adc1pin 1
#define adc2pin 6
#define adc3pin 11


//#define sleep delayMicroseconds(0.1)
#define sleep mySleep()
#define length 500
#define maxloops 1000

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

int findTimeDiff(int clock, int cs, int adc0, int adc1,int adc2, int adc3) {	//Input variables refer to the pin numbers of the clock, chip select and adc output pins respectively
	pinMode(clock,OUTPUT);
	pinMode(cs, OUTPUT);
	pinMode(adc0, INPUT);
	pinMode(adc1, INPUT);
	pinMode(adc2, INPUT);
	pinMode(adc3, INPUT);
	digitalWrite(cs, 1); //Initializes chip select to 1
	digitalWrite(clock, 0); //Initializes clock to 1
	sleep;
	int i,j,flag[4],counter=0,thresh[4],got[4],event=0;
	//arr[length] are arrays containing the last [length] elements in the respective channel (starting at position (counter%length)+1)
	for(i=0;i<4;i++)
		{
		flag[i]=0;
		got[i]=0;
		}
	volatile uint16_t val[4],bit[4];
	//while(1)
	while(event==0)	//Condition which terminates the loop after all channels are triggered within maxloops of each other
		{
		for(i=0;i<4;i++)
			{
			bit[i]=0;
			val[i]=0;
			}
		digitalWrite(cs, 0); //Falling chip select initiates conversion and data transfer
		for(i=0;i<2;i++)
			{
			digitalWrite(clock, 1);
			sleep;
			digitalWrite(clock, 0);
			sleep;
			}
		for(i=0;i<12;i++) //Begin reading data at the falling edge after the null bit
			{
			digitalWrite(clock, 1);
			sleep;
			digitalWrite(clock, 0);
			sleep;
			bit[0] = digitalRead(adc0); //Adding an extra digitalRead() here seems to fix a lot of the shoddy values... not too sure why but it works!
			bit[0] = digitalRead(adc0);
			bit[1] = digitalRead(adc1);
			bit[2] = digitalRead(adc2);
			bit[3] = digitalRead(adc3);
			for(j=0;j<4;j++)
				val[j]=val[j]|(bit[j]<<(11-i));
			}
		for(i=0;i<4;i++)
			{
			if(isLegit(val[i])&&(val[i]<1800||val[i]>2200))
				flag[i]++;
			else
				flag[i]=0;
			}
		for(i=0;i<4;i++)
			{
			if(flag[i]==2&&got[i]==0) //Event considered triggered when two consecutive legit values are not in range [1800,2200]
				{
				thresh[i]=counter;
				got[i]=1;
				}
			}
		if(got[0]==1&&got[1]==1&&got[2]==1&&got[3]==1)
			event=counter;
		for(i=0;i<4;i++)
			{
			if(got[i]==1&&((counter-thresh[i])>maxloops))
				{
				got[i]=0;
				thresh[i]=0;
				}
			}
		digitalWrite(cs, 1); //Raise chip select
		sleep;
		counter++;
		}	
	FILE *ofp = fopen("timings.pitap","w");
	for(i=0;i<4;i++)
		fprintf(ofp,"%d\n",event-thresh[i]);
	fclose(ofp);
	}



int main() {
	piHiPri(99);
	if (wiringPiSetup () == -1)
		exit (1);
	findTimeDiff(clockpin,cspin,adc0pin,adc1pin,adc2pin,adc3pin);
	return 0;
}
