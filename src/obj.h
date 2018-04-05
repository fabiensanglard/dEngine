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