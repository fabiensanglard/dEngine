/*
 *  stats.c
 *  dEngine
 *
 *  Created by fabien sanglard on 16/08/09.
 *
 */

#include "stats.h"


unsigned int triCount = 0;
unsigned int textSwitchCount = 0;
unsigned int shaderSwitchCount = 0;
unsigned int blendingSwitchCount = 0;

void STATS_Begin()
{
	triCount = 0;
	textSwitchCount = 0;
	shaderSwitchCount = 0;
	blendingSwitchCount=0;
}

void STATS_AddTriangles(int count)
{
	triCount+= count;
}

void STATS_AddTexSwitch(){textSwitchCount++;}
void STATS_AddShaderSwitch(){shaderSwitchCount++;}
void STATS_AddBlendingSwitch(){blendingSwitchCount++;}


int STATS_GetTrianglesCount(void){return triCount;}
int STATS_GetTexSwitchCount(void){return textSwitchCount;}
int STATS_GetShaderSwitchCount(void){return shaderSwitchCount;}
int STATS_GetBlendingSwitchCount(void){return blendingSwitchCount;}