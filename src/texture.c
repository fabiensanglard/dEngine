/*
 *  texture.c
 *  dEngine
 *
 *  Created by fabien sanglard on 16/08/09.
 *
 */

#include "texture.h"
#include "ItextureLoader.h"
#include "filesystem.h"

//Cache
#define HASH_MAX_VALUE 1024
tex_cache_bucket_t* tex_hashtable[HASH_MAX_VALUE];

void TEXT_InitTextureLibrary(void)
{
	int i;
	
	for (i=0 ; i < HASH_MAX_VALUE ; i++)
		tex_hashtable[i] = NULL;
}

void TEXT_ClearTextureLibrary(void)
{
	printf("TEXT_CleanTextureLibrary IS NOT IMPLEMENTED !! DO IT !!!.\n");
}


W32 tex_strhash( const char *string )
{
	W32 hash = *string;
	
	if( hash )
	{
		for( string += 1; *string != '\0'; ++string )
		{
			hash = (hash << 5) - hash + *string;
		}
	}
	
	return hash;
}



texture_t* TEX_GetTexture(char* mtlName)
{ 
	unsigned int hashValue ;
	tex_cache_bucket_t* bucket;
	
	hashValue= tex_strhash(mtlName) % HASH_MAX_VALUE;
	
	bucket = tex_hashtable[hashValue];
	
	if (bucket == NULL)
		return NULL;
	else
	{
		while (strcmp(bucket->texture->path,mtlName) && bucket->next != NULL)
			bucket = bucket->next;
	}
	
	if (!strcmp(bucket->texture->path,mtlName))
		return bucket->texture;
	else
		return NULL;
}

void TEX_PutTexture(texture_t* texture)
{
	tex_cache_bucket_t* bucket;
	unsigned int hashValue ; 
	
	hashValue= tex_strhash(texture->path) % HASH_MAX_VALUE;
	
	if ( tex_hashtable[hashValue] == NULL)
	{
		bucket = (tex_cache_bucket_t*)calloc(1,sizeof(tex_cache_bucket_t));
		tex_hashtable[hashValue] = bucket;
	}
	else
	{
		//printf("[MTL parser] Collision detected, while inserting '%s'.\n",mtlName);
		bucket = tex_hashtable[hashValue];
		
		while (bucket->next)
			bucket = bucket->next;
		
		bucket->next = (tex_cache_bucket_t*)calloc(1,sizeof(tex_cache_bucket_t));
		bucket = bucket->next;
	}
	
	
	bucket->texture = texture;
	bucket->next = NULL;
	
}


void TEX_LoadTexture(texture_t* tmpTex)
{
	char* extension; 
	texture_t* cacheTest;
	
	
	cacheTest = TEX_GetTexture(tmpTex->path);
	
	if (cacheTest != NULL)
	{
		//printf("[LoadTexturer] Cache hit.\n");
		*tmpTex = *cacheTest;
		return;
	}
	
	extension = FS_GetExtensionAddress(tmpTex->path);
	
	tmpTex->format = TEXTURE_TYPE_UNKNOWN;
	
	
	if (!strcmp(extension,"pvr") )
	{
		loadNativePVRT(tmpTex);
	}
	
	if (!strcmp(extension,"png"))
	{
		loadNativePNG(tmpTex);
	}
	
	if (tmpTex->format == TEXTURE_TYPE_UNKNOWN)
	{
		printf("[Texture loader] Texture type for %s is UNKNOWN !!\n",tmpTex->path);
	}
	
	//Cache for futur usage
	TEX_PutTexture(tmpTex);
	

}
