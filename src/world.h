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