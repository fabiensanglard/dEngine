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
 *  obj.c
 *  dEngine
 *
 *  Created by fabien sanglard on 24/09/09.
 *
 */

#include "obj.h"
#include "filesystem.h"
#include "material.h"
#include "globals.h"
#include "lexer.h"


typedef struct vertex_ref_t
{
	vec2_t textCoo;
	ushort vertexRef;
} vertex_ref_t;

typedef struct vertex_cache_entry_t
{
	uchar num_values;
	vertex_ref_t verticesRef[16];
	
} vertex_cache_entry_t;

vertex_cache_entry_t* vertex_cache;
ushort verticesCacheCreated;

void OBJ_InitVerticeCache()
{
	vertex_cache = (vertex_cache_entry_t*)calloc(DE_USHRT_MAX, sizeof(vertex_cache_entry_t));
	verticesCacheCreated=0;
}

void OBJ_DestroyVerticeCache()
{
	free(vertex_cache);
}


int GetCacheVertex(ushort indice, vec2_t textCoo)
{
	vertex_cache_entry_t* currentCacheEntry;
	int i;
	int found = -1;
	
	currentCacheEntry = &vertex_cache[indice];
	
	for(i=0;i<currentCacheEntry->num_values;i++)
	{
		if (currentCacheEntry->verticesRef[i].textCoo[0] == textCoo[0]&&
			currentCacheEntry->verticesRef[i].textCoo[1] == textCoo[1]
			)
		{
			found = currentCacheEntry->verticesRef[i].vertexRef;
			break;
		}
	}
	
	if (found != -1)
	{
		//printf("Vertex cache hit !\n");
		return found;
	}
	
	//Need to create one
	currentCacheEntry->verticesRef[currentCacheEntry->num_values].textCoo[0] = textCoo[0];
	currentCacheEntry->verticesRef[currentCacheEntry->num_values].textCoo[1] = textCoo[1];
	currentCacheEntry->verticesRef[currentCacheEntry->num_values].vertexRef = verticesCacheCreated;
	
	verticesCacheCreated++;
	currentCacheEntry->num_values++;
	
	//printf("Vertex cache miss !\n");
	return verticesCacheCreated-1;
}



void OBJ_Free(obj_t* obj)
{
	free(obj->vertices);
	free(obj->indices);
	free(obj);
}

void OBJ_GenerateLightingInfo(obj_t* obj)
{
	vec3_t v1,v2 ;
	int i;
	vec2_t st1,st2;
	float coef;
	vec3_t currentTangent;
	vec3_t currentNormal;
	vec3_t* tangents;
	vec3_t* normals;
	
	tangents = (vec3_t*)calloc(obj->num_vertices, sizeof(vec3_t));
	normals  = (vec3_t*)calloc(obj->num_vertices, sizeof(vec3_t));
	

	
	//Accumulate tangents & normals
	for(i=0;i<obj->num_indices/3;i++)
	{
		vectorSubtract(obj->vertices[obj->indices[i*3+2]].position , obj->vertices[obj->indices[i*3]].position, v1);
		vectorSubtract(obj->vertices[obj->indices[i*3+1]].position , obj->vertices[obj->indices[i*3]].position, v2);
		
		vector2Subtract(obj->vertices[obj->indices[i*3+2]].textCoo,obj->vertices[obj->indices[i*3]].textCoo,st1);
		vector2Subtract(obj->vertices[obj->indices[i*3+1]].textCoo,obj->vertices[obj->indices[i*3]].textCoo,st2);
	
		//Normal part
		vectorCrossProduct(v2, v1, currentNormal);
		vectorAdd(normals[obj->indices[i*3+0]],currentNormal,normals[obj->indices[i*3+0]]);
		vectorAdd(normals[obj->indices[i*3+1]],currentNormal,normals[obj->indices[i*3+1]]);
		vectorAdd(normals[obj->indices[i*3+2]],currentNormal,normals[obj->indices[i*3+2]]);
		
		//Tangent part
		coef = 1/ (st1[0] * st2[1] - st2[0] * st1[1]);
		
		currentTangent[0] = coef * (v1[0] * st2[1]  + v2[0] * -st1[1]);
		currentTangent[1] = coef * (v1[1] * st2[1]  + v2[1] * -st1[1]);
		currentTangent[2] = coef * (v1[2] * st2[1]  + v2[2] * -st1[1]);
		
		vectorAdd(tangents[obj->indices[i*3+0]],currentTangent,tangents[obj->indices[i*3+0]]);
		vectorAdd(tangents[obj->indices[i*3+1]],currentTangent,tangents[obj->indices[i*3+1]]);
		vectorAdd(tangents[obj->indices[i*3+2]],currentTangent,tangents[obj->indices[i*3+2]]);
	}
	
	//Vector Normalize + Normalize
	for(i=0;i<obj->num_vertices;i++)
	{
		normalize(tangents[i]);
		vectorScale(tangents[i],DE_SHRT_MAX,obj->vertices[i].tangent);
		
		normalize(normals[i]);
		vectorScale(normals[i],DE_SHRT_MAX,obj->vertices[i].normal);
	}
	
	free(tangents);
	free(normals);
}


char OBJ_Load(char* filename,entity_t* entity)
{
	static vec3_t tmpVertices[DE_USHRT_MAX];
	ushort num_vertices=0;
	
	static vec2_t tmpTextureCoor[DE_USHRT_MAX];
	ushort num_textuCoor=0;
	
	int idata[9];	
	int i;
	
	ushort indiceOfCurrentVertex;
	
	filehandle_t* objFile ;
	
	obj_t* obj;
	
	
	
	
	objFile = FS_OpenFile(filename, "rt");
	
	if (!objFile)
	{
		printf("Could not Load OBJ: '%s'.\n",filename);
		return 0;
	}
	
	
		
	OBJ_InitVerticeCache();
	
	obj = (obj_t*)entity->model;
	obj->vertices = (obj_vertex_t*)calloc(DE_USHRT_MAX, sizeof(obj_vertex_t));
	obj->num_vertices = 0;
	obj->indices = (ushort*)calloc(DE_USHRT_MAX, sizeof(ushort));
	obj->num_indices = 0;
	
	LE_pushLexer();
	
	LE_init(objFile);
	
	while (LE_hasMoreData()) 
	{
		LE_readToken();
		
		if (!strcmp("v",LE_getCurrentToken()))
		{
			tmpVertices[num_vertices][0] = LE_readReal();
			tmpVertices[num_vertices][1] = LE_readReal();
			tmpVertices[num_vertices][2] = LE_readReal();
			num_vertices++;
			
		}
		else
		if (!strcmp("vt",LE_getCurrentToken()))
		{
			tmpTextureCoor[num_textuCoor][0] = LE_readReal();
			tmpTextureCoor[num_textuCoor][1] = 1- LE_readReal();
			num_textuCoor++;
		}
		else
		if (!strcmp("vn",LE_getCurrentToken()))
		{
			for(i=0;i<3;i++)
				LE_readReal();
			
			//Drop it (like it's hot)
		}
		else
		if (!strcmp("f",LE_getCurrentToken()))
		{
			LE_SetWhiteCharValue('/', 1);
			
			for(i=0;i<9;i++)
			{
				idata[i] = LE_readReal();
				idata[i]--;
			}
			
			#ifdef OBJ_FACE_CCW
			for(i=0;i<3;i++)//OBJ Standard: Couter-clockwise
			#else
			for(i=2;i>=0;i--) //Bugged expoter.
			#endif			
			{
				indiceOfCurrentVertex = GetCacheVertex(idata[3*i],tmpTextureCoor[idata[3*i+1]]);
				//indiceOfCurrentVertex = obj->num_indices;
				
				if (indiceOfCurrentVertex+1 > obj->num_vertices)
					obj->num_vertices = indiceOfCurrentVertex+1;
				
				vectorCopy (tmpVertices[idata[3*i]]		, obj->vertices[indiceOfCurrentVertex].position);
				//for(i=0;i<3;i++)
				//{
				//		printf("Vertices: %d, %d, %d.\n",obj->vertices[indiceOfCurrentVertex].position[0],obj->vertices[indiceOfCurrentVertex].position[1],obj->vertices[indiceOfCurrentVertex].position[2]);
				//}
				vector2Copy(tmpTextureCoor[idata[3*i+1]], obj->vertices[indiceOfCurrentVertex].textCoo);
				
				//printf("num_indices = %d, indice pointer=%d\n",obj->num_indices,indiceOfCurrentVertex);
				obj->indices[obj->num_indices++] = indiceOfCurrentVertex ;
				
			}
			
			LE_SetWhiteCharValue('/', 0);
		}
		else
		if (!strcmp("usemtl",LE_getCurrentToken()))
		{
			LE_readToken();
			entity->material = MATLIB_Get(LE_getCurrentToken());
			if (entity->material == 0)
				printf("************[OBJ Loader] Could not find material: '%s'\n",LE_getCurrentToken());
			
		}
	}
	
	
	LE_popLexer();
	
	FS_CloseFile(objFile);
	
	//Adjust size of vertices and indices
	obj->vertices = realloc(obj->vertices,obj->num_vertices  * sizeof(obj_vertex_t));
	obj->indices  = realloc(obj->indices ,obj->num_indices   * sizeof(ushort));
	
	//printf("Obj %s has %d vertices.\n",filename,obj->num_vertices);
	//printf("Obj %s has %d indices.\n",filename,obj->num_indices);
	
	OBJ_DestroyVerticeCache();
	
	//Generate lighting infos.
	OBJ_GenerateLightingInfo(obj);
	
	return 1;
}

/*
void OBJ_GetStats(int* stats,FILE* file)
{
	
	char buff[512];
	char string[512];
	float fdata[9];
	int idata[9];
	
	stats[0] = 0;	//num_v
	stats[1] = 0;	//num_vt
	stats[2] = 0;	//num_vn	
	stats[3] = 0;	//num_i	
	
	while (!feof (file))
	{
		
		fgets (buff, sizeof (buff), file);
		
		if (sscanf(buff,"mtllib %s",string) == 1) ;
		else if (sscanf (buff, "v %f %f %f", &fdata[0],&fdata[1],&fdata[2]) == 3)
		{
			stats[0]++;
			
		}
		else if (sscanf (buff, "vt %f %f", &fdata[0],&fdata[1]) == 2)
		{

			stats[1]++;
		}
		else if (sscanf (buff, "vn %f %f %f", &fdata[0],&fdata[1],&fdata[2]) == 3)
		{
			
			stats[2]++;
		}
		else if (sscanf(buff, "usemtl %s",string) ==1)
		{
			;
		}
		else if (sscanf (buff, "f %d/%d/%d %d/%d/%d %d/%d/%d", &idata[0],&idata[1],&idata[2],&idata[3],&idata[4],&idata[5],&idata[6],&idata[7],&idata[8]) == 9)
		{
			stats[3]++;
			//			printf("%d\n",stats[3]);
		}
	}
	
	
	fseek(file,0,SEEK_SET);
}

void OBJ_TextureNormalize(vec2_t textCooVector)
{
	
	int i;
	
	printf("texture in = %.3f,%.3f -> ",textCooVector[0],textCooVector[1]);
	
	for(i=0;i<2;i++)
	{
		//while (textCooVector[i] < -1)
		if (textCooVector[i] < -1)
			textCooVector[i] += (int)textCooVector[i];
		
		if (textCooVector[i] >= 1) 			
			textCooVector[i] -= (int)textCooVector[i];
		
		//textCooVector[i] *= 32767;
	}
	
	printf("normalized text coo= %.3f , %.3f \n",textCooVector[0],textCooVector[1]);
}

void OBJ_NormalVectorNormalize(vec3_t vector)
{
	vectorScale(vector,32767,vector);
}




char OBJ_Load(char* filename,obj_t* obj)
{
	
	FILE* file;
	int i;
	float fdata[9];
	int idata[9];	
	int num_vertices=0;
	int num_textCoo=0;
	int num_normals=0;
	char buff[512];
	char materialName[512];
	vec2_t* textCooTmpArray;
	vec3_t* normalsTmpArray;	
	char* fullPath;
	int currentIndicePointer=0;
	
	if (strlen(filename) == 0)
	{
		printf("[OBJ_Load] Empty string .\n");
		return 0;
	}	
	
	fullPath = (char*)calloc((strlen(filename)+1+strlen(FS_Gamedir()) + 1) , sizeof(char));
	strcat(fullPath, FS_Gamedir());
	strcat(fullPath, "/");
	strcat(fullPath,filename);
	
	file =  fopen(fullPath, "rt");
	
	if (!file)
	{
		printf("[OBJ_Load] Cannot open file '%s'.\n",fullPath);
		free(fullPath);
		return 0;
	}
	

	
	//Get number of v,vt and vn, 
	OBJ_GetStats(idata,file);
	

	
	//allocate buffers
	obj->num_vertices = idata[0]*3;
	obj->vertices =  calloc(obj->num_vertices, sizeof(obj_vertex_t));

	
	obj->num_indices = idata[3]*3 ;
	obj->indices = calloc(obj->num_indices, sizeof(ushort));
	
	
	//allocate tmp buffers
	textCooTmpArray = calloc(idata[1], sizeof(vec2_t));
	normalsTmpArray = calloc(idata[2], sizeof(vec3_t));
	
	//Generate vertexArray
	while (!feof (file))
	{
		fgets (buff, sizeof (buff), file);
		
		if (sscanf(buff,"mtllib ") == 1) 
		{
			;
		}
		else if (sscanf (buff, "v %f %f %f", &fdata[0],&fdata[1],&fdata[2]) == 3)
		{
			for(i=0;i<3;i++)
				obj->vertices[num_vertices].position[i] = fdata[i];
			
			num_vertices++;
		}
		else if (sscanf (buff, "vt %f %f", &fdata[0],&fdata[1]) == 2)
		{
		//	OBJ_TextureNormalize(fdata);
			
			
			textCooTmpArray[num_textCoo][0] = fdata[0];
			textCooTmpArray[num_textCoo][1] = fdata[1];
			
			num_textCoo++;
		}
		else if (sscanf (buff, "vn %f %f %f", &fdata[0],&fdata[1],&fdata[2]) == 3)
		{
			for(i=0;i<3;i++)
				normalsTmpArray[num_normals][i] = fdata[i];
			
			normalize(normalsTmpArray[num_normals]);
			
			OBJ_NormalVectorNormalize(normalsTmpArray[num_normals]);
			
			num_normals++;
		}
		else if (sscanf(buff, "usemtl %s",materialName) ==1)
		{
			obj->material = MATLIB_Get(materialName);
			if (obj->material == 0)
				printf("[OBJ Loader] Could not find material: '%s'\n",materialName);
		}
		else if (sscanf (buff, "f %d/%d/%d %d/%d/%d %d/%d/%d", &idata[0],&idata[1],&idata[2],&idata[3],&idata[4],&idata[5],&idata[6],&idata[7],&idata[8]) == 9)
		{
			for(i=0;i<9;i++)
				idata[i] -= 1;
				
			vector2Copy(textCooTmpArray[idata[1]],obj->vertices[idata[0]].textCoo);		
			vectorCopy(normalsTmpArray[idata[2]],obj->vertices[idata[0]].normal);
			
    		vector2Copy(textCooTmpArray[idata[4]],obj->vertices[idata[3]].textCoo);		
			vectorCopy(normalsTmpArray[idata[5]],obj->vertices[idata[3]].normal);
			
			vector2Copy(textCooTmpArray[idata[7]],obj->vertices[idata[6]].textCoo);					
			vectorCopy(normalsTmpArray[idata[8]],obj->vertices[idata[6]].normal);
			

			

			obj->indices[currentIndicePointer++] = idata[6];
			obj->indices[currentIndicePointer++] = idata[3];
			obj->indices[currentIndicePointer++] = idata[0];
			
		}
	}
	
	
	
	//Generate lighting informations if required
#ifdef TANGENT_ENABLED
	OBJ_GenerateLightingInfo(obj);
#endif
	
	
	//Free tmp buffers
	free(textCooTmpArray);
	free(normalsTmpArray);
	
	free(fullPath);
	
	fclose(file);
	
	return 1;
}
*/