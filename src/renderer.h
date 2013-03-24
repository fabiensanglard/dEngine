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
 *  renderer.h
 *  dEngine
 *
 *  Created by fabien sanglard on 09/08/09.

 *
 */



#ifndef DE_RENDERER
#define DE_RENDERER

#define GL_11_RENDERER 0
#define GL_20_RENDERER 1



#define SHADOW_TYPE_NORMAL 0
#define SHADOW_TYPE_DISABLED 1
#define SHADOW_TYPE_VSM 2

#include "md5model.h"
#include "texture.h"
#include "font.h"

extern int renderWidth;
extern int renderHeight;

typedef struct renderer_t 
{
	uint type;
	
	uchar props;
	//uint supportBumpMapping;
	//uint shadowType;
	uchar isRenderingShadow;
	uchar isBlending;
	
	uint mainFramebufferId;
	
	uint statsEnabled;
	uint materialQuality;
	
	
	
	void (*Set3D)(void); 
	void (*StopRendition  )(void); 
//	void (*PushMatrix)(void);
//	void (*PopMatrix)(void);
	void (*SetTexture)(unsigned int);
	void (*RenderEntities)(void);
	void (*UpLoadTextureToGpu)(texture_t* texture);
	void (*Set2D)(void);
	void (*RenderString)(svertex_t* vertices,ushort* indices, uint numIndices);
	void (*GetColorBuffer)(uchar* data);
	void (*RenderSprite)(uint textureId, svertex_t* vertices, uint* indices, uint numIndices);
} renderer_t;

extern renderer_t renderer;

void SCR_Init(int rendererType,int viewPortWidth,int viewPortHeight);
void SCR_RenderFrame(void);
void SCR_GetColorBuffer(uchar* data);
void gluPerspective(float fovy,float aspect, float zNear, float zFar,matrix_t projectionMatrix);
void gluLookAt(  vec3_t vEye,  vec3_t vLookat, vec3_t vUp,matrix_t fModelView );

/* TextureMagFilter */
#define DE_GL_NEAREST                        0x2600
#define DE_GL_LINEAR                         0x2601
#define DE_DEFAULT_FILTERING				DE_GL_LINEAR

#endif
