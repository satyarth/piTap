#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "fmin.h"
#include "findTimeDiff.h"

int main() {
	piHiPri(99);
	if (wiringPiSetup () == -1)
		exit (1);
	int *timingsptr;
	while(1)
		{
		timingsptr=findTimeDiff();
		int timings[4];
		for(int i=0;i<4;i++)
			{
			timings[i]=*(timingsptr+i);
			printf("%d\n",timings[i]);
			}
		findPosition(timings[0],timings[1],timings[2],timings[3]);
		delay(500);
		}
	return 0;
}
