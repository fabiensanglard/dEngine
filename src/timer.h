
/*
 *  timer.h
 *  dEngine
 *
 *  Created by fabien sanglard on 15/08/09.
 *
 */

#ifndef FS_TIMER
#define FS_TIMER

#include "globals.h"

void Timer_resetTime(void);
void Timer_tick(void);

void Timer_Pause(void);
void Timer_Resume(void);

void Timer_ForceTimeIncrement(int ms);

extern int fps;
extern unsigned int simulationTime;
extern int timediff;

#endif