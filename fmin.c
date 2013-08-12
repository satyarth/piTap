#include <stdio.h>
#include <stdlib.h>
#include <stdfloat.h>
#include <math.h>

#define num 100

float error(float x,float y,float x1,float y1,float x2,float y2,float tdiff)
	{
	float error;
	error=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1))-sqrt((x-x2)*(x-x2)-(y-y2)*(y-y2)-tdiff;
	return(error);
	}
	
float f(float x,float y,float d_ba,float d_dc,float d_ca,float d_db)
	{
	float fun;
	fun=error(x,y,x_b,y_b,x_a,y_a,d_ba)*error(x,y,x_b,y_b,x_a,y_a,d_ba)+error(x,y,x_d,y_d,x_c,y_c,d_dc)*error(x,y,x_d,y_d,x_c,y_c,d_dc)+error(x,y,x_c,y_c,x_a,y_a,d_ca)*error(x,y,x_c,y_c,x_a,y_a,d_ca)+error(x,y,x_d,y_d,x_b,y_b,d_db)*error(x,y,x_d,y_d,x_b,y_b,d_db);
	return(fun);
	}

void main()
	{
	int a,b,c,d;
	FILE *ifp;
	ifp = fopen("timings.pitap","r");
	fscanf(fp, "%d %d %d %d", a, b, c, d);
	fclose(ifp);
	float val[num][num];
	}
