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
 *  material.c
 *  dEngine
 *
 *  Created by fabien sanglard on 24/09/09.
 *
 */

#include "material.h"
#include "filesystem.h"
#include "lexer.h"
#include "texture.h"
#include "renderer.h"




typedef struct mat_bucket_t {
	material_t* material ;
	struct mat_bucket_t *next;
} mat_bucket_t;

#define SIZE_MAT_HASHTABLE 1024
mat_bucket_t* mat_hashtable[SIZE_MAT_HASHTABLE];


uint Get_Mat_HashValue( const char *string )
{
	uint hash = *string;
	
	if( hash )
	{
		for( string += 1; *string != '\0'; ++string )
		{
			hash = (hash << 5) - hash + *string;
		}
	}
	
	return hash % SIZE_MAT_HASHTABLE;
}

void MAT_Put(material_t* material)
{
	mat_bucket_t* bucket;
	unsigned int hashValue ; 
	
	hashValue= Get_Mat_HashValue(material->name);
	
	bucket = mat_hashtable[hashValue] ;
	
	if ( bucket == NULL)
	{
		bucket = (mat_bucket_t*)calloc(1,sizeof(mat_bucket_t));
		mat_hashtable[hashValue] = bucket;
	}
	else
	{		
		while (bucket->next)
			bucket = bucket->next;
		
		bucket->next = (mat_bucket_t*)calloc(1,sizeof(mat_bucket_t));
		bucket = bucket->next;
	}
	
	
	bucket->material = material;
	bucket->next = NULL;
}

material_t* MATLIB_Get(char* materialName)
{ 
	unsigned int hashValue ;
	mat_bucket_t* bucket;
	
	hashValue= Get_Mat_HashValue(materialName);
	
	bucket = mat_hashtable[hashValue];
	
	if (bucket == NULL)
		return NULL;
	
	while (1)
	{
		if (strcmp(bucket->material->name,materialName))
		{
			if (bucket->next != NULL)
				bucket = bucket->next;
			else
				return NULL;
		}
		else
			return bucket->material;
	}
	
	//return NULL;
}




void MATLIB_FreeMemory(void){}



material_t* MATLIB_Create(char* materialName)
{
	material_t* material;
	char* newMaterialName;
	int i;
	
//	printf("[MATLIB]: Creating %s\n",materialName);
	material = calloc(1, sizeof(material_t));
	material->textures[TEXTURE_DIFFUSE]=0;
	material->textures[TEXTURE_BUMP]=0;
	material->textures[TEXTURE_SPECULAR]=0;
	material->loaded = 0;
	
	material->shininess = 40 ;  //10
	
	for (i=0; i<3; i++) 
		material->specularColor[i] = 1;
	
	material->prop = 0;
	material->prop |= PROP_SHADOW;
	
	//strcpy(material->name, materialName);
	newMaterialName = material->name;
	while(*materialName != '\0')
	{
		if (*materialName != '"')
			*newMaterialName++ = *materialName++;
		else
			materialName++;
	}
	
	
	MAT_Put(material);
	
	return material;
}

/*
 #define PROP_BUMP      1
 #define PROP_SPEC      2
 #define PROP_DIFF	   4
 #define PROP_UNDEF1    8
 #define PROP_UNDEF2   16
 #define PROP_UNDEF3   32
 #define PROP_UNDEF    64
 #define PROP_SHADOW  128
*/

void MATLIB_printProp(uchar props)
{
    
	printf("prop for %X %1d%1d%1d%1d%1d%1d%1d%1d\n",
		   props,
		   (props & PROP_SHADOW) >> 7,
		   (props & PROP_UNDEF) >> 6,
		   (props & PROP_UNDEF3) >> 5,
		   (props & PROP_UNDEF2) >> 4,
		   (props & PROP_UNDEF1) >> 3,
		   (props & PROP_DIFF) >> 2,
		   (props & PROP_SPEC) >> 1,
		   (props & PROP_BUMP)
		   );
	if ((props & PROP_BUMP) == PROP_BUMP) printf("PROP_BUMP\n");
	if ((props & PROP_SPEC) == PROP_SPEC) printf("PROP_SPEC\n");
	if ((props & PROP_DIFF) == PROP_DIFF) printf("PROP_DIFF\n");
	if ((props & PROP_UNDEF1) == PROP_UNDEF1) printf("PROP_UNDEF1\n");
	if ((props & PROP_UNDEF2) == PROP_UNDEF2) printf("PROP_UNDEF2\n");
	if ((props & PROP_UNDEF3) == PROP_UNDEF3) printf("PROP_UNDEF3\n");
	if ((props & PROP_UNDEF) == PROP_UNDEF) printf("PROP_UNDEF\n");
	if ((props & PROP_SHADOW) == PROP_SHADOW) printf("PROP_SHADOW\n");
}



void MATLIB_LoadLibraries(void)
{
	int i;
	filehandle_t* library;
	
	for (i=0 ; i < SIZE_MAT_HASHTABLE ; i++)
		mat_hashtable[i] = 0;
	
	library = FS_OpenFile("data/materials.lbr","rt");
	
	LE_init(library);
	
	while (LE_hasMoreData()) 
	{
		LE_readToken();
		
		if (!strcmp("highQuality", LE_getCurrentToken()))
		{
			if (renderer.materialQuality == MATERIAL_QUALITY_LOW)
			{
				while (strcmp("}", LE_getCurrentToken())) {
					LE_readToken();
				}
			}
			else 
			{
				
				LE_readToken(); // {
				LE_readToken();
				while (strcmp("}", LE_getCurrentToken())) 
				{
					MATLIB_LoadLibrary(LE_getCurrentToken());
					LE_readToken();
				}
			}

		}
		
		if (!strcmp("lowQuality", LE_getCurrentToken()))
		{
			if (renderer.materialQuality == MATERIAL_QUALITY_HIGH)
			{
				while (strcmp("}", LE_getCurrentToken())) {
					LE_readToken();
				}
			}
			else 
			{
				LE_readToken(); // {
				LE_readToken();
				while (strcmp("}", LE_getCurrentToken())) 
				{
					MATLIB_LoadLibrary(LE_getCurrentToken());
					LE_readToken();
				}
			}			
		}
			
	}
	
}

void MATLIB_LoadLibrary(char* mtlPath)
{
	filehandle_t*	mtlFile			;
	material_t*		currentMaterial	;
	int				i				;
	
	
	
	mtlFile = FS_OpenFile(mtlPath, "rt");
	
	if (!mtlFile)
	{
		printf("Material loader could not find '%s'",mtlPath);
		return;
	}
	
	LE_pushLexer();
	LE_init(mtlFile);
	
	while(LE_hasMoreData())
	{
		LE_readToken();
		if (!strcmp("newmtl",LE_getCurrentToken()))
		{
			LE_readToken();
			currentMaterial = MATLIB_Create(LE_getCurrentToken());
			currentMaterial->hasAlpha  = 0;
		}
		else 
		if (!strcmp("map_Kd",LE_getCurrentToken()))
		{
			if(currentMaterial->textures[TEXTURE_DIFFUSE] == 0)
			{
				currentMaterial->textures[TEXTURE_DIFFUSE] = calloc(1,sizeof(texture_t));
				LE_readToken();
				currentMaterial->textures[TEXTURE_DIFFUSE]->path = malloc((strlen(LE_getCurrentToken())+1)*sizeof(char));
				strcpy(currentMaterial->textures[TEXTURE_DIFFUSE]->path, LE_getCurrentToken());
				
				currentMaterial->prop |= PROP_DIFF ;
			//	printf("	[MATLIB] Adding diffuse texture= %s.\n",currentMaterial->textures[TEXTURE_DIFFUSE]->path);
			}
		}
		else 
		if (!strcmp("BumpKd",LE_getCurrentToken()))
		{
			if(currentMaterial->textures[TEXTURE_BUMP] == 0)
			{
				currentMaterial->textures[TEXTURE_BUMP] = calloc(1,sizeof(texture_t));
				LE_readToken();
				currentMaterial->textures[TEXTURE_BUMP]->path = malloc((strlen(LE_getCurrentToken())+1)*sizeof(char));
				strcpy(currentMaterial->textures[TEXTURE_BUMP]->path, LE_getCurrentToken());
				
				currentMaterial->prop |= PROP_BUMP;
			//	printf("	[MATLIB] Adding bump texture= %s.\n",currentMaterial->textures[TEXTURE_BUMP]->path);
			}
			
		}	
		else 
		if (!strcmp("SpecKd",LE_getCurrentToken()))
		{
			if(currentMaterial->textures[TEXTURE_SPECULAR] == 0)
			{
				currentMaterial->textures[TEXTURE_SPECULAR] = calloc(1,sizeof(texture_t));
				LE_readToken();
				currentMaterial->textures[TEXTURE_SPECULAR]->path = malloc((strlen(LE_getCurrentToken())+1)*sizeof(char));
				strcpy(currentMaterial->textures[TEXTURE_SPECULAR]->path, LE_getCurrentToken());
				
				currentMaterial->prop |= PROP_SPEC;
			//	printf("	[MATLIB] Adding specular texture= %s.\n",currentMaterial->textures[TEXTURE_SPECULAR]->path);
			}
		}
		else 
		if (!strcmp("Ks",LE_getCurrentToken()))
		{
			for(i=0;i<3 ; i++)
			{
				currentMaterial->specularColor[i] = LE_readReal();
			}
		}
		else 
		if (!strcmp("illum",LE_getCurrentToken()))
		{
			currentMaterial->shininess = LE_readReal();
		}
		else 
		if (!strcmp("hasAlpha",LE_getCurrentToken()))
		{
			currentMaterial->hasAlpha = LE_readReal();
		}
		 
	}
	
	LE_popLexer();
	FS_CloseFile(mtlFile);
}


#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03

void MATLIB_MakeAvailable(material_t* material)
{
	if (material == 0)
		return;
	
	if (material->loaded)
		return;
	
	
	//printf("[MATLIB] Loading MAT %s\n",material->name);
	
	if ((material->prop & PROP_DIFF) == PROP_DIFF)
	{
		
		
		//printf("	[MATLIB] Loading %s\n",material->textures[TEXTURE_DIFFUSE]->path);
		TEX_LoadTexture(material->textures[TEXTURE_DIFFUSE]);
		renderer.UpLoadTextureToGpu(material->textures[TEXTURE_DIFFUSE]);
		
		if (
			material->textures[TEXTURE_DIFFUSE]->format == GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG || 
			material->textures[TEXTURE_DIFFUSE]->format == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG)
		{
			material->hasAlpha = 1;
		}
	}
	
	if ((material->prop & PROP_BUMP) == PROP_BUMP )
	{
		//printf("	[MATLIB] Loading %s\n",material->textures[TEXTURE_BUMP]->path);
		TEX_LoadTexture(material->textures[TEXTURE_BUMP]);
		renderer.UpLoadTextureToGpu(material->textures[TEXTURE_BUMP]);
	}
	
	if ((material->prop & PROP_SPEC) == PROP_SPEC )
	{
		//printf("	[MATLIB] Loading %s\n",material->textures[TEXTURE_SPECULAR]->path);
		TEX_LoadTexture(material->textures[TEXTURE_SPECULAR]);
		renderer.UpLoadTextureToGpu(material->textures[TEXTURE_SPECULAR]);
	}
	
	material->loaded = 1;
}