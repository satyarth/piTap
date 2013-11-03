pitap: main.c findTimeDiff.c fmin.c mouseClick.c
	gcc main.c findTimeDiff.c fmin.c mouseClick.c -std=gnu99 -lwiringPi -lX11 -lpthread -lm -o pitap
