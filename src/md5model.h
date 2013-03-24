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
 *  md5model.h
 *  dEngine
 *
 *  Created by fabien sanglard on 10/08/09.
 *
 */

#ifndef __MD5MODEL_H__
#define __MD5MODEL_H__


#include "globals.h"
#include "math.h"
#include "matrix.h"
#include "quaternion.h"
#include "texture.h"
#include "material.h"
#include "entities.h"

typedef struct md5_joint_t
{
	char name[64];
	int parent;
	vec3_t pos;
	quat4_t orient;
	
} md5_joint_t;


typedef struct md5_vertex_t
{
	vec2short_t st;

	int start; /* start weight */
	int count; /* weight count */
	
} md5_vertex_t;


typedef struct md5_triangle_t
{
	int index[3];
	
} md5_triangle_t;


typedef struct md5_weight_t
{
	int joint;
	float bias;
	
	vec3_t pos;
	
	vec3short_t normal;
	
	// Only 3GS and shader capable need tangent for bumpMapping
	#ifdef TANGENT_ENABLED
	vec3short_t tangent;
	#endif
} md5_weight_t;


typedef struct md5_bbox_t
{
	vec3_t min;
	vec3_t max;
	
} md5_bbox_t;


typedef struct md5_mesh_t
{
	md5_vertex_t *vertices;
	md5_triangle_t *triangles;
	md5_weight_t *weights;
	
	int num_verts;
	int num_tris;
	int num_weights;
	
	char shader[256];
	char bumpShader[256];
	
	vertex_t* vertexArray ;
	ushort* vertexIndices ;
	
	
	
} md5_mesh_t;


typedef struct md5_model_t
{
	md5_joint_t *baseSkel;
	md5_mesh_t *meshes;
	
	
	int num_joints;
	int num_meshes;
} md5_model_t;


typedef struct md5_anim_t
{
	int num_frames;
	int num_joints;
	int frameRate;
	
	md5_joint_t **skelFrames;
	md5_bbox_t *bboxes;
	
} md5_anim_t;



typedef struct md5_object_t
{
	md5_model_t md5Model;
	md5_anim_t  md5Anim;
	
	
	uchar animated;
	

	
} md5_object_t;





int ReadMD5Model (const char *filename, entity_t* entity );
void FreeModel (md5_model_t *mdl);
void GenerateGPUVertices (md5_mesh_t *mesh, const md5_joint_t *skeleton);
void GenerateLightingInfo (const md5_mesh_t *mesh,  md5_joint_t *skeleton);
void DrawSkeleton (const md5_joint_t *skeleton, int num_joints);


int CheckAnimValidity (const md5_model_t *mdl,const md5_anim_t *anim);
int ReadMD5Anim (const char *filename, md5_anim_t *anim);
void FreeAnim (struct md5_anim_t *anim);
void InterpolateSkeletons (int frameIndex,int numFrames, int num_joints, float interp,  md5_joint_t **joints, md5_joint_t *targetJoints);

void MD5_Update(md5_object_t* md5object);

#endif /* __MD5MODEL_H__ */