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