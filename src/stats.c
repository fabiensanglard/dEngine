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