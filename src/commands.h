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
 *  commands.h
 *  dEngine
 *
 *  Created by fabien sanglard on 09-10-09.
 *
 */



#ifndef DE_COMMAND
#define DE_COMMAND

#include "globals.h"

typedef enum command_type_t
{
	COMMAND_TYPE_HEAD,
	COMMAND_TYPE_PITCH,
	COMMAND_TYPE_ROLL,
	COMMAND_TYPE_MOVE_NORTH_SOUTH,
	COMMAND_TYPE_MOVE_EAST_WEST
} command_type_t ;


typedef struct command_t
{
	command_type_t type;
	float value;
} command_t;

void Comm_AddCommand(command_t* command);

void Comm_Update(void) ;


#endif