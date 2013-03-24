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
 *  entities.h
 *  dEngine
 *
 *  Created by fabien sanglard on 11/09/09.
 *
 */


#ifndef ED_ENTITIES
#define ED_ENTITIES

#define ENTITY_MD5 0
#define ENTITY_OBJ 1

#include "globals.h"
#include "math.h"
#include "matrix.h"
#include "material.h"

typedef struct entity_t {
	uchar type;
	void* model;
	
	material_t* material;
	
	matrix_t matrix;
	matrix_t cachePVMShadow;
	ushort id;
	
} entity_t;



#endif
