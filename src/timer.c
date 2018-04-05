/*
 *  timer.c
 *  dEngine
 *
 *  Created by fabien sanglard on 15/08/09.
 *
 */

#include "timer.h"


int forcedTimeIncrement=0;
void Timer_ForceTimeIncrement(int ms)
{
	printf("***********[Timer] !! WARNING !! Time increment is forced !! WARNING !!");
	forcedTimeIncrement = ms;
}

#ifndef WIN32
#include <sys/time.h>
int E_Sys_Milliseconds( void )
{
	struct timeval tp;
	static int	secbase;
	
	
	
	gettimeofday( &tp, 0 );
	
	if( ! secbase )
	{
		secbase = tp.tv_sec;
		return tp.tv_usec / 1000;
	}
	
	return (tp.tv_sec - secbase) * 1000 + tp.tv_usec / 1000;
}
#else
#include "windows.h"
#include "MMSystem.h"
int E_Sys_Milliseconds( void )
{
	//return 0;
	return (int)timeGetTime();
}
#endif

unsigned char paused = 1;
int fps;
int lastTime;
int currentTime;
int fpsAcc;
int fpsTimeAcc;
unsigned int simulationTime;
int timediff;

void Timer_Pause(void)
{
	paused=1;
}

void Timer_Resume(void)
{
	paused = 0;
	lastTime = currentTime;
}



void Timer_tick(void)
{
	if (paused)
	{
		timediff = 0;
		return;
	}
	
	
	
	lastTime = currentTime;
	currentTime = E_Sys_Milliseconds();
	
	
	if (forcedTimeIncrement)
		timediff = forcedTimeIncrement;
	else
		timediff = currentTime - lastTime;
	
	
	
	
	simulationTime += timediff;
	
	
	
	fpsTimeAcc += timediff;
	fpsAcc++;
	
	if (fpsTimeAcc > 1000)
	{
		fps = (int)(fpsAcc * 1000.0f/ fpsTimeAcc);
		fpsAcc = 0;
		fpsTimeAcc = 0;
	}
}


int renderedFrames=0;

void Timer_resetTime(void)
{
	//printf("[Timer] simulationTime = %d.\n",simulationTime);
	//lastTime = E_Sys_Milliseconds();
	
	Timer_tick();
	simulationTime = 0;
}