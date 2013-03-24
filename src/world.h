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
 *  world.h
 *  dEngine
 *
 *  Created by fabien sanglard on 24/08/09.
 *
 */

#ifndef DE_WORLD
#define DE_WORLD

#include "md5model.h"
#include "entities.h"


typedef struct light_t {
	
	vec4_t position;
	vec3_t lookAt;
	vec3_t upVector;
	float fov;
	
	vec4_t ambient;
	vec4_t diffuse;
	vec4_t specula;
	float constantAttenuation;
	float linearAttenuation;
	//float quadraticAttenuation;
	
	char attachedToCamera;
} light_t;

void World_Init(void);
void World_Update(void);

extern light_t light;

extern entity_t map[256];
extern uchar num_map_entities;

#endif