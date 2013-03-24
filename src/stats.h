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
 *  stats.h
 *  dEngine
 *
 *  Created by fabien sanglard on 16/08/09.
 *
 */

void STATS_Begin();
void STATS_AddTriangles(int count);
void STATS_AddTexSwitch();
void STATS_AddShaderSwitch();
void STATS_AddBlendingSwitch();

int STATS_GetTrianglesCount(void);
int STATS_GetTexSwitchCount(void);
int STATS_GetShaderSwitchCount(void);
int STATS_GetBlendingSwitchCount(void);