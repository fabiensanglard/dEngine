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
 *  obj.h
 *  dEngine
 *
 *  Created by fabien sanglard on 24/09/09.
 *
 */

#ifndef DE_OBJ
#define DE_OBJ

//#define OBJ_FACE_CCW 
#define OBJ_FACE_CW

#include "math.h"
#include "material.h"
#include "entities.h"

typedef struct obj_vertex_t
{
	vec3_t position;
	vec3short_t normal;
	vec3short_t tangent;
	vec2_t textCoo;
} obj_vertex_t;

typedef struct obj_t {
	obj_vertex_t* vertices;
	ushort num_vertices;
	
	ushort* indices;
	ushort num_indices;
	
	
	
} obj_t;



void OBJ_Free(obj_t* obj);
char OBJ_Load(char* filename,entity_t* entity);


#endif