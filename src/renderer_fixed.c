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
 *  r_fixed_renderer.c
 *  dEngine
 *
 *  Created by fabien sanglard on 09/08/09.
 *
 */

#include "renderer_fixed.h"
#include "dEngine.h"
#include "camera.h"
#include <OpenGLES/ES1/gl.h>
#include "stats.h"
#include "world.h"
#include "obj.h"

matrix_t projectionMatrix;
matrix_t modelViewMatrix;
matrix_t textureMatrix = { 1.0f/32767,0,0,0,0,1.0f/32767,0,0,0,0,1,0,0,0,0,1};	//Unpacking matrix

void SCR_CheckErrors(char* step, char* details)
{
	GLenum err = glGetError();
	switch (err) {
		case GL_INVALID_ENUM:printf("Error GL_INVALID_ENUM %s, %s\n", step,details); break;
		case GL_INVALID_VALUE:printf("Error GL_INVALID_VALUE  %s, %s\n", step,details); break;
		case GL_INVALID_OPERATION:printf("Error GL_INVALID_OPERATION  %s, %s\n", step,details); break;				
		case GL_OUT_OF_MEMORY:printf("Error GL_OUT_OF_MEMORY  %s, %s\n", step,details); break;			
		case GL_NO_ERROR: break;
		default : printf("Error UNKNOWN  %s, %s\n", step,details);break;
	}
}

void SetupCameraF(void)
{
	vec3_t vLookat;
	
	vectorAdd(camera.position,camera.forward,vLookat);
	
	gluLookAt(camera.position, vLookat, camera.up, modelViewMatrix);
	glLoadMatrixf(modelViewMatrix);
}


void SetupLightingF(void)
{
	glLightfv(GL_LIGHT0, GL_POSITION, light.position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light.ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light.specula);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light.constantAttenuation);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light.linearAttenuation);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light.quadraticAttenuation);
	
	//
}

void Set2DF(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(-renderWidth, renderWidth, -renderHeight, renderHeight, -1, 1);
	//glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
}

void Set3DF(void)
{
	
	glDisable(GL_BLEND);
	renderer.isBlending = 0;
	
	glEnable(GL_DEPTH_TEST);
	
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	
	//glCullFace(GL_FRONT);
	
	glClear (GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT); 
	
	
	
	
	
}

void StopRenditionF(void)
{
	
}


void UpLoadTextureToGPUF(texture_t* texture)
{
	if (!texture || !texture->data || texture->textureId != 0)
		return;
	
	glGenTextures(1, &texture->textureId);
	glBindTexture(GL_TEXTURE_2D, texture->textureId);
	
	if (texture->format == TEXTURE_GL_RGB ||texture->format == TEXTURE_GL_RGBA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
	else
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, texture->format, texture->width,texture-> height, 0, texture->dataLength, texture->data);
	
	
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, DE_DEFAULT_FILTERING);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, DE_DEFAULT_FILTERING);	

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	SCR_CheckErrors("Loading texture",texture->path);
	
	if (texture->file != NULL)
		FS_CloseFile(texture->file);
	
	free(texture->data);	

}

unsigned int lastId;
void SetTextureF(unsigned int textureId)
{
	if (lastId == textureId)
		return;
	
	glBindTexture(GL_TEXTURE_2D, textureId);
	STATS_AddTexSwitch();
	
	lastId = textureId;
}


void RenderNormalsOBJF(obj_t* obj)
{
	
	float normalsVertex[20000];
	float* normalVertex;
	int j;
	
	float vScale = 3;
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	
	
	normalVertex = normalsVertex;
	for(j=0; j < obj->num_vertices ; j++)
	{
		*normalVertex++ = obj->vertices[j].position[0];
		*normalVertex++ = obj->vertices[j].position[1];
		*normalVertex++ = obj->vertices[j].position[2];
		
		*normalVertex++ = obj->vertices[j].position[0] + obj->vertices[j].normal[0]/DE_SHRT_MAX*vScale;
		*normalVertex++ = obj->vertices[j].position[1] + obj->vertices[j].normal[1]/DE_SHRT_MAX*vScale;
		*normalVertex++ = obj->vertices[j].position[2] + obj->vertices[j].normal[2]/DE_SHRT_MAX*vScale;
	}
	glColor4f(1, 0, 0, 1);
	glVertexPointer (3, GL_FLOAT,0,normalsVertex);
	glDrawArrays(GL_LINES, 0, obj->num_vertices * 2);
	
	
#ifdef TANGENT_ENABLED
	normalVertex = normalsVertex;
	for(j=0; j < obj->num_vertices ; j++)
	{
		*normalVertex++ = obj->vertices[j].position[0];
		*normalVertex++ = obj->vertices[j].position[1];
		*normalVertex++ = obj->vertices[j].position[2];
		
		*normalVertex++ = obj->vertices[j].position[0] + obj->vertices[j].tangent[0]/DE_SHRT_MAX*vScale;
		*normalVertex++ = obj->vertices[j].position[1] + obj->vertices[j].tangent[1]/DE_SHRT_MAX*vScale;
		*normalVertex++ = obj->vertices[j].position[2] + obj->vertices[j].tangent[2]/DE_SHRT_MAX*vScale;
	}
	glColor4f(0, 0, 1, 1);
	glVertexPointer (3, GL_FLOAT,0,normalsVertex);
	glDrawArrays(GL_LINES, 0, obj->num_vertices * 2);
#endif
	
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
}


void RenderNormalsF(md5_mesh_t* currentMesh)
{
	
	float normalsVertex[20000];
	float* normalVertex;
	int j;
	
	float vScale = 3;
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	
	
	normalVertex = normalsVertex;
	for(j=0; j < currentMesh->num_verts ; j++)
	{
		*normalVertex++ = currentMesh->vertexArray[j].pos[0];
		*normalVertex++ = currentMesh->vertexArray[j].pos[1];
		*normalVertex++ = currentMesh->vertexArray[j].pos[2];
		
		*normalVertex++ = currentMesh->vertexArray[j].pos[0] + currentMesh->vertexArray[j].normal[0]/DE_SHRT_MAX*vScale;
		*normalVertex++ = currentMesh->vertexArray[j].pos[1] + currentMesh->vertexArray[j].normal[1]/DE_SHRT_MAX*vScale;
		*normalVertex++ = currentMesh->vertexArray[j].pos[2] + currentMesh->vertexArray[j].normal[2]/DE_SHRT_MAX*vScale;
	}
	glColor4f(1, 0, 0, 1);
	glVertexPointer (3, GL_FLOAT,0,normalsVertex);
	glDrawArrays(GL_LINES, 0, currentMesh->num_verts * 2);
	
	
	#ifdef TANGENT_ENABLED
	normalVertex = normalsVertex;
	for(j=0; j < currentMesh->num_verts ; j++)
	{
		*normalVertex++ = currentMesh->vertexArray[j].pos[0];
		*normalVertex++ = currentMesh->vertexArray[j].pos[1];
		*normalVertex++ = currentMesh->vertexArray[j].pos[2];
		
		*normalVertex++ = currentMesh->vertexArray[j].pos[0] + currentMesh->vertexArray[j].tangent[0]/DE_SHRT_MAX*vScale;
		*normalVertex++ = currentMesh->vertexArray[j].pos[1] + currentMesh->vertexArray[j].tangent[1]/DE_SHRT_MAX*vScale;
		*normalVertex++ = currentMesh->vertexArray[j].pos[2] + currentMesh->vertexArray[j].tangent[2]/DE_SHRT_MAX*vScale;
	}
	glColor4f(0, 0, 1, 1);
	glVertexPointer (3, GL_FLOAT,0,normalsVertex);
	glDrawArrays(GL_LINES, 0, currentMesh->num_verts * 2);
	#endif
	
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

}

void RenderEntitiesMD5F(void* md5Void)
{
		
	
	md5_object_t* md5Object;
	md5_mesh_t* currentMesh;
	int i;
	
	md5Object = (md5_object_t*)md5Void;
	

	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(textureMatrix);
	glMatrixMode(GL_MODELVIEW);
	
	for (i = 0; i < 1/*md5Object.md5Model.num_meshes*/; i++)
    {
		currentMesh = &md5Object->md5Model.meshes[i];
				
		//RenderNormalsF( currentMesh);
		
		glNormalPointer(GL_SHORT, sizeof(vertex_t), currentMesh->vertexArray[0].normal);
		glTexCoordPointer (2, GL_SHORT, sizeof(vertex_t), currentMesh->vertexArray[0].text);	
		glVertexPointer (3, GL_FLOAT, sizeof(vertex_t), currentMesh->vertexArray[0].pos);	
		glDrawElements (GL_TRIANGLES, currentMesh->num_tris * 3, GL_UNSIGNED_SHORT, currentMesh->vertexIndices);
		STATS_AddTriangles(currentMesh->num_tris);
    }
	
}

void RenderEntitiesOBJF(void* objVoid)
{
	obj_t* obj;
	
	obj = (obj_t*)objVoid;
	
	
	
	//RenderNormalsOBJF( obj);
	
	glNormalPointer(GL_SHORT, sizeof(obj_vertex_t), obj->vertices[0].normal);
	glTexCoordPointer (2, GL_FLOAT, sizeof(obj_vertex_t), obj->vertices[0].textCoo);	
	glVertexPointer (3, GL_FLOAT, sizeof(obj_vertex_t), obj->vertices[0].position);	
	glDrawElements (GL_TRIANGLES, obj->num_indices, GL_UNSIGNED_SHORT, obj->indices);
	STATS_AddTriangles(obj->num_indices/3);
}



void RenderEntitiesF(void)
{
	int i;
	entity_t* entity;
	
	
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective(camera.fov, camera.aspect,camera.zNear, camera.zFar, projectionMatrix);
	glLoadMatrixf(projectionMatrix);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	SetupCameraF();
	SetupLightingF();
	
	for(i=0; i < num_map_entities; i++)
	{
		
		entity = &map[i];
		glPushMatrix();
			glMultMatrixf(entity->matrix);
		
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, entity->material->shininess);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, entity->material->specularColor);
			SetTextureF(entity->material->textures[TEXTURE_DIFFUSE]->textureId);
		
			if (entity->material->hasAlpha )
			{
				if (!renderer.isBlending)
				{
					renderer.isBlending = 1;
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					STATS_AddBlendingSwitch();
				}
			}
			else
			{
				if (renderer.isBlending)
				{
					renderer.isBlending = 0;
					glDisable(GL_BLEND);
					STATS_AddBlendingSwitch();
				}
			}
		
			if (entity->type == ENTITY_OBJ)
				RenderEntitiesOBJF(entity->model);
			else
				RenderEntitiesMD5F(entity->model);
		glPopMatrix();
	}

}

void RenderStringF(svertex_t* vertices,ushort* indices, uint numIndices)
{
	

	glVertexPointer (2, GL_SHORT, sizeof(svertex_t), vertices);	
	glTexCoordPointer (2, GL_FLOAT,sizeof(svertex_t), vertices->text);	
	
	//glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glDrawElements (GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,indices);
	STATS_AddTriangles(numIndices/3);
	
	
	
    
   
	
}

void GetColorBufferF(uchar* data)
{
	glReadPixels(0,0,renderWidth,renderHeight,GL_RGBA, GL_UNSIGNED_BYTE,data);
}

void RenderSpriteF(uint textureId, svertex_t* verticesSprite, uint* indicesSprite, uint numIndices)
{
	SetTextureF(textureId);
	glVertexPointer(2, GL_SHORT,  sizeof(svertex_t), verticesSprite->pos);
	glTexCoordPointer( 2, GL_FLOAT,  sizeof(svertex_t), verticesSprite->text);			

	glDrawElements (GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,indicesSprite); //numIndices==6
}

void initFixedRenderer(renderer_t* renderer)
{
	
	
	
	renderer->type = GL_11_RENDERER ;
	
	//renderer->supportBumpMapping = 0;
	renderer->props = 0;
	
	
	
	renderer->Set3D = Set3DF;
	renderer->StopRendition = StopRenditionF;
	renderer->SetTexture = SetTextureF;
	renderer->RenderEntities = RenderEntitiesF;
	renderer->UpLoadTextureToGpu = UpLoadTextureToGPUF;
	renderer->Set2D = Set2DF;
	renderer->RenderString = RenderStringF;
	renderer->GetColorBuffer = GetColorBufferF;
	renderer->RenderSprite = RenderSpriteF;
	
	glViewport(0, 0, renderWidth, renderHeight);
	
	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	

	glEnable(GL_TEXTURE_2D);

	
	
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	
	//glEnable(GL_LIGHTING);
	
	glClearColor(0.0f, 0.0f, 0.0f,1.0f);
	glColor4f(1.0f, 1.0f, 1.0f,1.0f);
	
	glEnable ( GL_COLOR_MATERIAL ) ;
	

	
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Error initing 1.1: glError: 0x%04X", err);
}

