#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

//Defining GPIO pin numbers for clock, chip select and four ADC inputs

#define clockpin 7
#define cspin 14
#define adc0pin 4
#define adc1pin 1
#define adc2pin 6
#define adc3pin 11

//Positions of four microphones + accuracy of minimization program

#define divisions_x 100
#define divisions_y 80
#define v 0.37
#define x_a 0
#define y_a 0
#define x_b 100
#define y_b 0
#define x_c 0
#define y_c 80
#define x_d 100
#define y_d 80

//#define sleep delayMicroseconds(0.1)
#define sleep mySleep()
#define maxloops 1000 //Maximum number of loops that may elapse between any two events being triggered

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

void findTimeDiff() {	//Waits until it detects a tap, then writes the timing differences to a file.
	pinMode(clockpin,OUTPUT);
	pinMode(cspin, OUTPUT);
	pinMode(adc0pin, INPUT);
	pinMode(adc1pin, INPUT);
	pinMode(adc2pin, INPUT);
	pinMode(adc3pin, INPUT);
	digitalWrite(cspin, 1); //Initializes chip select to 1
	digitalWrite(clockpin, 0); //Initializes clock to 1
	sleep;
	int i,j,flag[4],counter=0,thresh[4],got[4],event=0;
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
		digitalWrite(cspin, 0); //Falling chip select initiates conversion and data transfer
		for(i=0;i<2;i++)
			{
			digitalWrite(clockpin, 1);
			sleep;
			digitalWrite(clockpin, 0);
			sleep;
			}
		for(i=0;i<12;i++) //Begin reading data at the falling edge after the null bit
			{
			digitalWrite(clockpin, 1);
			sleep;
			digitalWrite(clockpin, 0);
			sleep;
			bit[0] = digitalRead(adc0pin); //Adding an extra digitalRead() here seems to fix a lot of the shoddy values by adding a small additional delay
			bit[0] = digitalRead(adc0pin);
			bit[1] = digitalRead(adc1pin);
			bit[2] = digitalRead(adc2pin);
			bit[3] = digitalRead(adc3pin);
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
		digitalWrite(cspin, 1); //Raise chip select
		sleep;
		counter++;
		}	
	FILE *ofp = fopen("timings.pitap","w");
	for(i=0;i<4;i++)
		fprintf(ofp,"%d\n",event-thresh[i]);
	fclose(ofp);
	}

float error(float x,float y,float x1,float y1,float x2,float y2,float tdiff)
	{
	float error;
	error=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1))-sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2))-tdiff;
	return(error);
	}
	
float cost_function(float x,float y,float d_ba,float d_dc,float d_ca,float d_db)
	{
	float cost;
	cost=error(x,y,x_b,y_b,x_a,y_a,d_ba)*error(x,y,x_b,y_b,x_a,y_a,d_ba)+error(x,y,x_d,y_d,x_c,y_c,d_dc)*error(x,y,x_d,y_d,x_c,y_c,d_dc)+error(x,y,x_c,y_c,x_a,y_a,d_ca)*error(x,y,x_c,y_c,x_a,y_a,d_ca)+error(x,y,x_d,y_d,x_b,y_b,d_db)*error(x,y,x_d,y_d,x_b,y_b,d_db);
	return(cost);
	}

void findPosition() //Reads timing differences from a file and outputs the position of the tap
	{
	int i,j;
	float minx=50,miny=4,arr[4];
	FILE *ifp = fopen("timings.pitap","r");
	for(i=0;i<4;i++)
		{
		fscanf(ifp,"%f", &arr[i]);
		}
	fclose(ifp);
	float d_ba,d_dc,d_ca,d_db,min;
	d_ba=v*(arr[0]-arr[1]);
	d_dc=v*(arr[2]-arr[3]);
	d_ca=v*(arr[0]-arr[2]);
	d_db=v*(arr[1]-arr[3]);
	float val[x_d][y_d];
	min=cost_function(50,50,d_ba,d_dc,d_ca,d_db);
	for(i=0;i<divisions_x;i++)
		for(j=0;j<divisions_y;j++)
			{
			val[i][j]=cost_function(i*(x_d-x_a)/divisions_x,j*(y_d-y_a)/divisions_y,d_ba,d_dc,d_ca,d_db);
			if(val[i][j]<min)
				{
				min=val[i][j];
				minx=(float)i*(x_d-x_a)/divisions_x;
				miny=(float)j*(y_d-y_a)/divisions_y;
				}
			}
	printf("%f\t%f\n",minx,miny);
	}
int main() {
	piHiPri(99);
	if (wiringPiSetup () == -1)
		exit (1);
	while(1)
		{
		findTimeDiff();
		findPosition();
		delay(500);
		}
	return 0;
}
