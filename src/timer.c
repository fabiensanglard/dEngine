/*
 dEngine Source Code 
 Copyright (C) 2009 - Fabien Sanglard
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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