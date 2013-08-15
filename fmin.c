#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "fmin.h"
#include "findTimeDiff.h"

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
