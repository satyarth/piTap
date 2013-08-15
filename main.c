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
	while(1)
		{
		findTimeDiff();
		findPosition();
		delay(500);
		}
	return 0;
}
