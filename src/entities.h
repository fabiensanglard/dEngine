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
