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