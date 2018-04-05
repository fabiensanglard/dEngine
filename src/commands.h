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