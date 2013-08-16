#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "fmin.h"
#include "findTimeDiff.h"
#include "mouseClick.h"

#define x_max_position 100
#define y_max_position 80
#define x_pixels 1280
#define y_pixels 1024

int main() {
	int *timingsPtr, *positionPtr, timings[4], position[2],x,y;
	piHiPri(99);
	if (wiringPiSetup () == -1)
		exit (1);
	while(1)
		{
		printf("start loop\n");
		timingsPtr=findTimeDiff();
		for(int i=0;i<4;i++)
			{
			timings[i]=*(timingsPtr+i);
			}
		printf("Timing: %d\n",timings[0]);
		positionPtr=findPosition(timings[0],timings[1],timings[2],timings[3]);
		for(int i=0;i<2;i++)
			{
			position[i]=*(positionPtr+i);
			}
		printf("position: %d %d",position[0],position[1]);
		x=(int)((float)position[0]*x_pixels/x_max_position);
		y=(int)((float)position[1]*y_pixels/y_max_position);
		clickCoords(x,y);
		delay(500);
		}
	return 0;
}
