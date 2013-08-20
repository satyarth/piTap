#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void main()
	{
	float v;
	FILE *ifp;
	ifp = fopen("calibration","r");
	fscanf(ifp, "%f", &v);
	fclose(ifp);
	printf("%f", v);
	}