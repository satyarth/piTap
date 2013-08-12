#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define num 100
#define v 0.37
#define x_a 0.0
#define y_a 0.0
#define x_b 100.0
#define y_b 0.0
#define x_c 0.0
#define y_c 80.0
#define x_d 100.0
#define y_d 80.0

float error(float x,float y,float x1,float y1,float x2,float y2,float tdiff)
	{
	float error;
	error=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1))-sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2))-tdiff;
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
	float val[num][num];
	min=f(50,50,d_ba,d_dc,d_ca,d_db);
	for(i=0;i<num;i++)
		for(j=0;j<num;j++)
			{
			val[i][j]=f(i,j,d_ba,d_dc,d_ca,d_db);
			if(val[i][j]<min)
				{
				min=val[i][j];
				minx=i;
				miny=j;
				}
			}
	printf("%f\t%f",minx,miny);
	}
