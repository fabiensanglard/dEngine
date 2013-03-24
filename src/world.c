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
 *  world.c
 *  dEngine
 *
 *  Created by fabien sanglard on 24/08/09.
 *
 */

#include "world.h"
#include "md5model.h"
#include "renderer.h"
#include "timer.h"
#include "dEngine.h"
#include "camera.h"
#include "timer.h"
#include "filesystem.h"
#include "lexer.h"
#include "obj.h"

light_t light;

entity_t map[256];
uchar num_map_entities;

void World_ReadMatrix(matrix_t target)
{
	int i,j ;
	
	LE_readToken(); //{
	for(i=0 ; i < 4 ; i++)
		for(j=0 ; j < 4 ; j++)
			target[j*4+i] = LE_readReal();
	LE_readToken(); //}
}


void World_ReadOBJs(matrix_t currentMatrix)
{
	entity_t* currentEntity;
	obj_t* obj;
	
	
	LE_readToken(); // {
	LE_readToken();
	while (strcmp(LE_getCurrentToken(), "}"))
	{
		currentEntity = &map[num_map_entities++];
		currentEntity->type = ENTITY_OBJ;
		obj = calloc(1, sizeof(obj_t));
		currentEntity->model = obj ;
		
		if (!OBJ_Load(LE_readToken(), currentEntity))
		{
			free(currentEntity->model);
			num_map_entities--;
		}
		else
		{
			matrixCopy(currentMatrix, currentEntity->matrix);
			MATLIB_MakeAvailable(currentEntity->material);
		}
		
		LE_readToken();
	}
}

void World_ReadMD5s(matrix_t currentMatrix)
{
	md5_object_t* md5Object;
	entity_t* currentEntity;
	md5_mesh_t* currentMesh;
	int i;
	
	LE_readToken(); // {
	LE_readToken();
	while (strcmp(LE_getCurrentToken(), "}"))
	{
		if (!strcmp("model",LE_getCurrentToken()))
		{
			LE_readToken();
			currentEntity = &map[num_map_entities++];
			currentEntity->type = ENTITY_MD5;
			md5Object = calloc(1, sizeof(md5_object_t));
			currentEntity->model = md5Object ;
			
			if (!ReadMD5Model(LE_getCurrentToken(),currentEntity ))
			{
				free(currentEntity->model);
				num_map_entities--;
			}
			else
			{
				matrixCopy(currentMatrix, currentEntity->matrix);
				for(i = 0; i < 1; i++)
				{
					currentMesh = &md5Object->md5Model.meshes[i];
					MATLIB_MakeAvailable(currentEntity->material);
				}
			}
		}
		
		if (!strcmp("anim",LE_getCurrentToken()))
		{
			LE_readToken();
			
			ReadMD5Anim (LE_getCurrentToken(), &md5Object->md5Anim);
						 
			 for(i = 0; i < 1; i++)
			 {
				 currentMesh = &md5Object->md5Model.meshes[i];
				 GenerateGPUVertices (currentMesh,md5Object->md5Model.baseSkel);
				 GenerateLightingInfo (currentMesh,md5Object->md5Model.baseSkel);
			 }
			 
		}
		
		LE_readToken();	
	}	
}

void World_Loadmap(char* mapFileName)
{
	filehandle_t* mapFile ;
	matrix_t currentMatrix;
	
	mapFile = FS_OpenFile(mapFileName, "rt");
	
	if (!mapFile)
	{
		printf("Map file '%s' cannot be opened.\n",mapFileName);
		return;
	}
	
	printf("[World_Loadmap] Found map: '%s'.\n",mapFileName);
	
	LE_pushLexer();
	LE_init(mapFile);
	
	while(LE_hasMoreData())
	{
		LE_readToken();
		
		if (!strcmp(LE_getCurrentToken(), "matrix"))
			World_ReadMatrix(currentMatrix);
		
		if (!strcmp(LE_getCurrentToken(), "entities"))
		{
			LE_readToken(); // OBJ or MD5 ?
			
			if (!strcmp("OBJ",LE_getCurrentToken()))
				World_ReadOBJs(currentMatrix);
				
			if (!strcmp("MD5",LE_getCurrentToken()))
				World_ReadMD5s(currentMatrix);

		}
	}
	
	LE_popLexer();
}



void World_OpenScene(char* filename)
{
	filehandle_t* sceneFile;
	
	sceneFile = FS_OpenFile(filename, "rt");
	
	if (!sceneFile)
	{
		printf("Could not find scene file: %s.\n",filename);
		return;
	}
	
	printf("[World_OpenScene] Found scene: '%s'.\n",filename);
	
	LE_pushLexer();
	LE_init(sceneFile);
	
	
	while (LE_hasMoreData()) 
	{
		LE_readToken();
		
		if (!strcmp("map", LE_getCurrentToken()))
		{
			LE_readToken();	//{
			LE_readToken();
			while (strcmp("}", LE_getCurrentToken()))
			{
				if (!strcmp("filename", LE_getCurrentToken()))
				{
					LE_readToken();
					num_map_entities = 0;
					World_Loadmap(LE_getCurrentToken());
				}
				LE_readToken();
			}
		}
		else 
		if (!strcmp("light", LE_getCurrentToken()))
		{
			LE_readToken(); // {
			LE_readToken();
			while (strcmp("}", LE_getCurrentToken()))
			{
				if (!strcmp("position", LE_getCurrentToken()))
				{
					light.position[0] = LE_readReal();
					light.position[1] = LE_readReal();
					light.position[2] = LE_readReal();
					light.position[3] = 1;
				}else 
				if (!strcmp("lookAt", LE_getCurrentToken()))
				{
					light.lookAt[0] = LE_readReal();
					light.lookAt[1] = LE_readReal();
					light.lookAt[2] = LE_readReal();
				}else 
				if (!strcmp("upVector", LE_getCurrentToken()))
				{
					light.upVector[0] = LE_readReal();
					light.upVector[1] = LE_readReal();
					light.upVector[2] = LE_readReal();
				}else 
				if (!strcmp("fov", LE_getCurrentToken()))
				{
					light.fov = LE_readReal();
				}else 
				if (!strcmp("ambientColor", LE_getCurrentToken()))
				{
					light.ambient[0] = LE_readReal();
					light.ambient[1] = LE_readReal();
					light.ambient[2] = LE_readReal();
				}else 
				if (!strcmp("diffuseColor", LE_getCurrentToken()))
				{
					light.diffuse[0] = LE_readReal();
					light.diffuse[1] = LE_readReal();
					light.diffuse[2] = LE_readReal();
				}else 
				if (!strcmp("speculaColor", LE_getCurrentToken()))
				{
					light.specula[0] = LE_readReal();
					light.specula[1] = LE_readReal();
					light.specula[2] = LE_readReal();
				}else 
				if (!strcmp("constantAttenuation", LE_getCurrentToken()))
				{
					light.constantAttenuation = LE_readReal();
				}else 
				if (!strcmp("linearAttenuation", LE_getCurrentToken()))
				{
					light.linearAttenuation = LE_readReal();
				}
				else 
					if (!strcmp("attachedToCamera", LE_getCurrentToken()))
					{
						LE_readToken();
						light.attachedToCamera = !strcmp("1", LE_getCurrentToken());
					}

				LE_readToken();
				//printf("lgiht token=%s\n",LE_getCurrentToken());
			}
		}
	}

	LE_popLexer();
}

void World_ReadSceneBlock(void)
{
	
	LE_readToken(); // {
	LE_readToken(); // scene fileName;
	
	World_OpenScene(LE_getCurrentToken());
	
	LE_readToken(); // }
	
}

void World_Init(void)
{
	filehandle_t* config;
	
	config = FS_OpenFile("data/config.cfg", "rt");
	
	
	if (!config)
	{
		printf("Conf file: data/config.cfg not found");
		exit(0);
	}
	
	LE_init(config);
	
	while (LE_hasMoreData()) 
	{
		LE_readToken();
		
		if (!strcmp("scene", LE_getCurrentToken()))
		{
			World_ReadSceneBlock();
		}
	}
	
			
	FS_CloseFile(config);
	
	
}


void World_Update(void)
{
	int i;
	md5_object_t* md5Object;
	entity_t* entity;
	
	for(i=0;i<num_map_entities;i++)
	{
		entity = &map[i];
		
		if (entity->type == ENTITY_OBJ)
			continue;
		
		md5Object = (md5_object_t*)entity->model;
		
		MD5_Update(md5Object);
	}
	
	
	

	//simulationTime = 25000;
	if (light.attachedToCamera)
	{
		//light.position = camera.position + camera.right * 5;
		vectorCopy(camera.up,light.position);
		vectorScale(light.position,-20,light.position);
		vectorAdd(camera.position,light.position,light.position);
		
		//light.lookAt = camera.forward * 10;
		vectorCopy(camera.forward,light.lookAt);
		vectorScale(light.lookAt,100,light.lookAt);
		
		vectorCopy(camera.up,light.upVector);
		
		
	}
	//camera.vEye[0] = -cos(simulationTime*0.0001f)*170	;
	//camera.vEye[2] = -sin(simulationTime*0.0001f)*170	;
	
	//light.position[0] = cos(simulationTime*0.001f)*200	;
	//light.position[2] = sin(simulationTime*0.001f)*200	;
	
	
	//light.position[0] = cos(simulationTime*0.002f)*80	;
	//light.position[1] = cos(simulationTime*0.001f)*10	+120;

	
}