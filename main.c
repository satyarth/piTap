#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "fmin.h"
#include "findTimeDiff.h"
#include "mouseClick.h"

int main() {
	int *timingsPtr, *positionPtr, timings[4], coords[2];
	piHiPri(99);
	if (wiringPiSetup () == -1)
		exit (1);
	while(1)
		{
		timingsPtr=findTimeDiff();
		for(int i=0;i<4;i++)
			{
			timings[i]=*(timingsPtr+i);
			}
		positionPtr=findPosition(timings[0],timings[1],timings[2],timings[3]);
		for(int i=0;i<2;i++)
			{
			coords[i]=*(positionPtr+i);
			}
		clickCoords(coords[0],coords[1]);
		delay(500);
		}
	return 0;
}
