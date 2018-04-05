/*
 *  texture.h
 *  dEngine
 *
 *  Created by fabien sanglard on 16/08/09.
 *
 */


#ifndef FS_TEXTURE
#define FS_TEXTURE

#include "globals.h"
#include "filesystem.h"

#define TEXTURE_TYPE_UNKNOWN	0x0
#define TEXTURE_GL_RGB			0x1907
#define TEXTURE_GL_RGBA			0x1908



typedef struct  {
	uint textureId;
	char* path;
	
	ubyte* data;
	int dataLength; //compressed texture need to provide their size
	
	filehandle_t* file;
	
	uint width;
	uint height;
	uint bpp;
	uint format;
	
} texture_t;

typedef struct tex_cache_bucket_t {
	texture_t* texture;
	struct tex_cache_bucket_t *next;
} tex_cache_bucket_t;

void TEX_LoadTexture(texture_t* tmpTex);


void TEXT_InitTextureLibrary(void);
void TEXT_ClearTextureLibrary(void);

#endif