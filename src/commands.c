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
 *  commands.c
 *  dEngine
 *
 *  Created by fabien sanglard on 09-10-09.
 *
 */

#include "commands.h"
#include "camera.h"
#include "timer.h"

#define MAX_NUM_COMMANDS 256
command_t commands[MAX_NUM_COMMANDS];
int last_command_id=-1;

void Comm_AddCommand(command_t* command)
{
	if (last_command_id >= MAX_NUM_COMMANDS)
	{
		printf("******[Comm] Command overflow");
		exit(0);
	}
	
	commands[++last_command_id] = *command;
}



#define LOOK_AROUND_ANGULAR_SPEED_MS (0.0001f/2.0f*3.14f)
#define MOVE_SPEED_MS (0.1f)

void Comm_AddPitch(float value)
{
	camera.pitch += LOOK_AROUND_ANGULAR_SPEED_MS * timediff * value;
}

void Comm_AddHead(float value)
{
	camera.head += LOOK_AROUND_ANGULAR_SPEED_MS * timediff * value;
}

void Comm_AddForBackWard(float value)
{
	vec3_t depVector;
	
	vectorCopy(camera.forward,depVector);
	
	depVector[1] = 0;
	vectorScale(depVector,timediff*MOVE_SPEED_MS* value,depVector);
	vectorAdd(camera.position,depVector,camera.position);
}

void Comm_Strafe(float value)
{
	vec3_t depVector;
	
	vectorCopy(camera.up,depVector);
	
	depVector[1] = 0;
	vectorScale(depVector,timediff*MOVE_SPEED_MS* value,depVector);
	vectorAdd(camera.position,depVector,camera.position);
}

void Comm_Update(void)
{
	int i;
	command_t* currentCommand;
	//printf("com buffsize=%d\n",last_command_id+1);
	for (i=0; i<=last_command_id; i++) 
	{
		
		currentCommand = &commands[i];
		
		switch (currentCommand->type) 
		{
			case COMMAND_TYPE_HEAD				:	Comm_AddHead(currentCommand->value);		break;
			case COMMAND_TYPE_PITCH				:	Comm_AddPitch(currentCommand->value);		break;
			case COMMAND_TYPE_ROLL				:									;			break;
			case COMMAND_TYPE_MOVE_NORTH_SOUTH	:	Comm_AddForBackWard(currentCommand->value);	break;
			case COMMAND_TYPE_MOVE_EAST_WEST	:	Comm_Strafe(currentCommand->value);			break;
			default:			break;
		}
	}
	
	last_command_id = -1;
}


