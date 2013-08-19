#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "fmin.h"
#include "findTimeDiff.h"

//Positions of four microphones + accuracy of minimization program
#define divisions_x 125
#define divisions_y 85
//#define v 1.163  Velocity in centimeters per cycle
#define x_a 0
#define y_a 0
#define x_b 125
#define y_b 0
#define x_c 0
#define y_c 85
#define x_d 125
#define y_d 85

float error(float x,float y,float x1,float y1,float x2,float y2,float tdiff)
	{
	float error;
	error=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1))-sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2))-tdiff;
	return(error);
	}
	
float cost_function(float x,float y,float d_ba,float d_dc,float d_ca,float d_db, float v)
	{
	float cost;
	cost=error(x,y,x_b,y_b,x_a,y_a,d_ba)*error(x,y,x_b,y_b,x_a,y_a,d_ba)+error(x,y,x_d,y_d,x_c,y_c,d_dc)*error(x,y,x_d,y_d,x_c,y_c,d_dc)+error(x,y,x_c,y_c,x_a,y_a,d_ca)*error(x,y,x_c,y_c,x_a,y_a,d_ca)+error(x,y,x_d,y_d,x_b,y_b,d_db)*error(x,y,x_d,y_d,x_b,y_b,d_db);
	return(cost);
	}

int * findPosition(int a, int b, int c, int d) //Reads timing differences from a file and outputs the position of the tap
	{
	int i,j;
	static int coords[2];
	float minx=50,miny=4;	
	float d_ba,d_dc,d_ca,d_db,min;
	const float v;
	FILE *ifp;
	ifp = fopen("calibration.txt","r");
	fscanf(fp, "%d", v);
	fclose(ifp); 
	d_ba=v*((float)a-(float)b);
	d_dc=v*((float)c-(float)d);
	d_ca=v*((float)a-(float)c);
	d_db=v*((float)b-(float)d);
	float val[x_d][y_d];
	min=cost_function(50,50,d_ba,d_dc,d_ca,d_db);
	for(i=0;i<divisions_x;i++)
		for(j=0;j<divisions_y;j++)
			{
			val[i][j]=cost_function(i*(x_d-x_a)/divisions_x,j*(y_d-y_a)/divisions_y,d_ba,d_dc,d_ca,d_db,v);
			if(val[i][j]<min)
				{
				min=val[i][j];
				minx=(float)i*(x_d-x_a)/divisions_x;
				miny=(float)j*(y_d-y_a)/divisions_y;
				}
			}
	coords[0]=(int)minx;
	coords[1]=(int)miny;
	return coords;
	}
