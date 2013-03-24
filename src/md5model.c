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
 *  md5mesh.h
 *  dEngine
 *
 *  Created by fabien sanglard on 10/08/09.
 *
 */


#include "globals.h"
#include "md5model.h"
#include "filesystem.h"
#include "texture.h"
#include "renderer.h"
#include "config.h"
#include "material.h"
#include "timer.h"

md5_model_t md5file;
md5_anim_t md5anim;

int animated = 0;

md5_joint_t *skeleton = NULL;


/* vertex array related stuff */
//int max_verts = 0;
//int max_tris = 0;






/**
 * Load an MD5 model from file.
 */
int ReadMD5Model (const char *filename, entity_t* entity )//md5_model_t *mdl)
{
	FILE *fp;
	char buff[512];
	int version;
	int curr_mesh = 0;
	int i,k,j,w;
	char* fullPath;
	md5_mesh_t* mesh;
	md5_model_t *mdl ;
	
	mdl = &((md5_object_t*)entity->model)->md5Model;
	
	fullPath = (char*)malloc((strlen(filename)+1+strlen(FS_Gamedir()) + 1) * sizeof(char));
	fullPath[0] = '\0';
	strcat(fullPath, FS_Gamedir());
	strcat(fullPath, "/");
	strcat(fullPath,filename);
	
	
	fp = fopen (fullPath, "rb");
	if (!fp)
    {
		fprintf (stderr, "Error: couldn't open \"%s\"!\n", fullPath);
		fprintf (stderr, "Error: couldn't open \"%s\"!\n", filename);
		
		return 0;
    }
	
	while (!feof (fp))
    {
		/* Read whole line */
		fgets (buff, sizeof (buff), fp);
		
		if (sscanf (buff, " MD5Version %d", &version) == 1)
		{
			if (version != 10)
			{
				/* Bad version */
				fprintf (stderr, "Error: bad model version\n");
				fclose (fp);
				return 0;
			}
		}
		else if (sscanf (buff, " numJoints %d", &mdl->num_joints) == 1)
		{
			if (mdl->num_joints > 0)
			{
				/* Allocate memory for base skeleton joints */
				mdl->baseSkel = (struct md5_joint_t *)
				calloc (mdl->num_joints, sizeof (struct md5_joint_t));
			}
		}
		else if (sscanf (buff, " numMeshes %d", &mdl->num_meshes) == 1)
		{
			if (mdl->num_meshes > 0)
			{
				/* Allocate memory for meshes */
				mdl->meshes = (struct md5_mesh_t *)
				calloc (mdl->num_meshes, sizeof (struct md5_mesh_t));
			}
		}
		else if (strncmp (buff, "joints {", 8) == 0)
		{
			/* Read each joint */
			for (i = 0; i < mdl->num_joints; ++i)
			{
				struct md5_joint_t *joint = &mdl->baseSkel[i];
				
				/* Read whole line */
				fgets (buff, sizeof (buff), fp);
				
				if (sscanf (buff, "%s %d ( %f %f %f ) ( %f %f %f )",
							joint->name, &joint->parent, &joint->pos[0],
							&joint->pos[1], &joint->pos[2], &joint->orient[0],
							&joint->orient[1], &joint->orient[2]) == 8)
				{
					/* Compute the w component */
					Quat_computeW (joint->orient);
				}
			}
		}
		else if (strncmp (buff, "mesh {", 6) == 0)
		{
			struct md5_mesh_t *mesh = &mdl->meshes[curr_mesh];
			int vert_index = 0;
			int tri_index = 0;
			int weight_index = 0;
			float fdata[4];
			int idata[3];
			
			mesh->bumpShader[0] = '\0';
			
			while ((buff[0] != '}') && !feof (fp))
			{
				/* Read whole line */
				fgets (buff, sizeof (buff), fp);
				
				if (strstr (buff, "shader "))
				{
					int quote = 0, j = 0;
					
					/* Copy the shader name whithout the quote marks */
					for (i = 0; i < sizeof (buff) && (quote < 2); ++i)
					{
						if (buff[i] == '\"')
							quote++;
						
						if ((quote == 1) && (buff[i] != '\"'))
							mesh->shader[j++] = buff[i];
					}
					
					entity->material = MATLIB_Get(mesh->shader);
					if (entity->material == 0)
						printf("[md5Loader] could not laod shader: '%s'\n",mesh->shader);
					
				}
				
				else if (strstr (buff, "bumpShader ") )
				{
					int quote = 0, j = 0;
					
					// Copy the shader name whithout the quote marks 
					for (i = 0; i < sizeof (buff) && (quote < 2); ++i)
					{
						if (buff[i] == '\"')
							quote++;
						
						if ((quote == 1) && (buff[i] != '\"'))
							mesh->bumpShader[j++] = buff[i];

					}
				}
			
				else if (sscanf (buff, " numverts %d", &mesh->num_verts) == 1)
				{
					if (mesh->num_verts > 0)
					{
						/* Allocate memory for vertices */
						mesh->vertices = (struct md5_vertex_t *)
						malloc (sizeof (struct md5_vertex_t) * mesh->num_verts);
					}
					
				}
				else if (sscanf (buff, " numtris %d", &mesh->num_tris) == 1)
				{
					if (mesh->num_tris > 0)
					{
						/* Allocate memory for triangles */
						mesh->triangles = (struct md5_triangle_t *)
						malloc (sizeof (struct md5_triangle_t) * mesh->num_tris);
					}
					
				}
				else if (sscanf (buff, " numweights %d", &mesh->num_weights) == 1)
				{
					if (mesh->num_weights > 0)
					{
						/* Allocate memory for vertex weights */
						mesh->weights = (struct md5_weight_t *)
						malloc (sizeof (struct md5_weight_t) * mesh->num_weights);
					}
				}
				else if (sscanf (buff, " vert %d ( %f %f ) %d %d", &vert_index, &fdata[0], &fdata[1], &idata[0], &idata[1]) == 5)
				{
					/* Copy vertex data */
					mesh->vertices[vert_index].st[0] = fdata[0] * 32767;
					mesh->vertices[vert_index].st[1] = fdata[1] * 32767;
					mesh->vertices[vert_index].start = idata[0];
					mesh->vertices[vert_index].count = idata[1];
				}
				else if (sscanf (buff, " tri %d %d %d %d", &tri_index,&idata[0], &idata[1], &idata[2]) == 4)
				{
					/* Copy triangle data */
					mesh->triangles[tri_index ].index[2] = idata[0];
					mesh->triangles[tri_index ].index[1] = idata[1];
					mesh->triangles[tri_index ].index[0] = idata[2];
				}
				else if (sscanf (buff, " weight %d %d %f ( %f %f %f )",
								 &weight_index, &idata[0], &fdata[3],
								 &fdata[0], &fdata[1], &fdata[2]) == 6)
				{
					/* Copy vertex data */
					mesh->weights[weight_index].joint  = idata[0];
					mesh->weights[weight_index].bias   = fdata[3];
					mesh->weights[weight_index].pos[0] = fdata[0];
					mesh->weights[weight_index].pos[1] = fdata[1];
					mesh->weights[weight_index].pos[2] = fdata[2];
				}
			}
			
			curr_mesh++;
		}
    }
	
	fclose (fp);
	free(fullPath);
	
	//Load all textures
	for(i = 0 ; i < mdl->num_meshes ; i++)
	{
		mesh = &mdl->meshes[i];
		
		mesh->vertexArray = malloc (sizeof (vertex_t) * mesh->num_verts);
		mesh->vertexIndices = malloc (sizeof (ushort) * mesh->num_tris * 3);
		
		
		/* Setup vertex indices */
		for (k = 0, w = 0; w < mesh->num_tris; w++)
		{
			for (j = 0; j < 3; j++,k++)
				mesh->vertexIndices[k] = mesh->triangles[w].index[j];
		}
	}
	
	
	
	
	return 1;
}

/**
 * Free resources allocated for the model.
 */
void FreeModel (md5_model_t *mdl)
{
	int i;
	
	if (mdl->baseSkel)
    {
		free (mdl->baseSkel);
		mdl->baseSkel = NULL;
    }
	
	if (mdl->meshes)
    {
		/* Free mesh data */
		for (i = 0; i < mdl->num_meshes; ++i)
		{
			if (mdl->meshes[i].vertices)
			{
				free (mdl->meshes[i].vertices);
				mdl->meshes[i].vertices = NULL;
			}
			
			if (mdl->meshes[i].triangles)
			{
				free (mdl->meshes[i].triangles);
				mdl->meshes[i].triangles = NULL;
			}
			
			if (mdl->meshes[i].weights)
			{
				free (mdl->meshes[i].weights);
				mdl->meshes[i].weights = NULL;
			}
		}
		
		free (mdl->meshes);
		mdl->meshes = NULL;
    }
}

/**
 * Prepare a mesh for drawing.  Compute mesh's final vertex positions
 * given a skeleton.  Put the vertices in vertex arrays.
 */
void GenerateGPUVertices (md5_mesh_t *mesh, const  md5_joint_t *skeleton)

{
	int i, j;
	const md5_weight_t *weight;
	const md5_joint_t *joint ;
	vec3_t tmpNormal,tmpVertex;
	vec3_t normalAccumulator;
	#ifdef TANGENT_ENABLED
	vec3_t tmpTangent;
	vec3_t tangentAccumulator;
	#endif
	
	/* Setup vertices */
	vertex_t* currentVertex = mesh->vertexArray ;
	for (i = 0; i < mesh->num_verts; ++i)
    {
		vectorClear(currentVertex->pos);
		vectorClear(normalAccumulator);
		
		#ifdef TANGENT_ENABLED
		vectorClear(tangentAccumulator);
		#endif
		
		// Calculate final vertex to draw with weights 
		for (j = 0; j < mesh->vertices[i].count; j++)
		{
			weight= &mesh->weights[mesh->vertices[i].start + j];
			joint = &skeleton[weight->joint];
			
			// Calculate transformed vertex for this weight 
			Quat_rotatePoint (joint->orient, weight->pos, tmpVertex);
			currentVertex->pos[0] += (joint->pos[0] + tmpVertex[0]) * weight->bias;
			currentVertex->pos[1] += (joint->pos[1] + tmpVertex[1]) * weight->bias;
			currentVertex->pos[2] += (joint->pos[2] + tmpVertex[2]) * weight->bias;
			
			// Same thing for normal
			Quat_rotateShortPoint (joint->orient, weight->normal, tmpNormal);
			vectorAdd(normalAccumulator,tmpNormal,normalAccumulator);
			
			#ifdef TANGENT_ENABLED
			Quat_rotateShortPoint (joint->orient, weight->tangent, tmpTangent);
			vectorAdd(tangentAccumulator,tmpTangent,tangentAccumulator);
			#endif
		}

		//Need to normalize normal
		normalize(normalAccumulator);
		vectorScale(normalAccumulator,32767,currentVertex->normal);
		
		#ifdef TANGENT_ENABLED
		normalize(tangentAccumulator);
		vectorScale(tangentAccumulator,32767,currentVertex->tangent);
		#endif
		
		currentVertex++;
    }
}

void GenerateLightingInfo (const md5_mesh_t *mesh, md5_joint_t *skeleton)
{

	int verticesCounter;
	int weightCounter;
	//User for tangent space generation
	vec3_t v1,v2,normal;
	
	vec3_t* normalAccumulator;
	vec3_t* normalWeightAccumulator;
	#ifdef TANGENT_ENABLED
	vec3_t tangent;
	float coef;
	vec3_t jointSpaceTangent;
	vec2_t st1,st2;
	vec3_t* tangentAccumulator;
	vec3_t* tangentWeightAccumulator;
	#endif
	
	vertex_t* currentVertex = NULL;
	md5_vertex_t* md5Vertex;
	int facesCounter;
	
	md5_weight_t* weight;
	md5_joint_t* joint;
	md5_triangle_t* currentFace;
	
	vec3_t jointSpaceNormal;
	
	normalAccumulator = calloc(mesh->num_verts, sizeof(vec3_t));
	normalWeightAccumulator = calloc(mesh->num_weights, sizeof(vec3_t));
 	#ifdef TANGENT_ENABLED
	tangentAccumulator = calloc(mesh->num_verts, sizeof(vec3_t));
	tangentWeightAccumulator  = calloc(mesh->num_weights, sizeof(vec3_t));
	#endif
	
	//Set all textures coordinate once for all.
	currentVertex = mesh->vertexArray;
	for(verticesCounter=0 ; verticesCounter < mesh->num_verts ; verticesCounter++,currentVertex++)
	{
		currentVertex->text[0] = mesh->vertices[verticesCounter].st[0];
		currentVertex->text[1] = mesh->vertices[verticesCounter].st[1];
	}
	
	currentFace = mesh->triangles;
	for(facesCounter = 0; facesCounter < mesh->num_tris ; facesCounter++,currentFace++)
	{
		
		// Normal part
		vectorSubtract(mesh->vertexArray[currentFace->index[2]].pos , mesh->vertexArray[currentFace->index[0]].pos, v1);
		vectorSubtract(mesh->vertexArray[currentFace->index[1]].pos , mesh->vertexArray[currentFace->index[0]].pos, v2);
		vectorCrossProduct(v2,v1,normal);
		
		vectorAdd(normalAccumulator[currentFace->index[0]],normal,normalAccumulator[currentFace->index[0]]);
		vectorAdd(normalAccumulator[currentFace->index[1]],normal,normalAccumulator[currentFace->index[1]]);
		vectorAdd(normalAccumulator[currentFace->index[2]],normal,normalAccumulator[currentFace->index[2]]);
		
		
		// The following part is from "Mathematic for 3D programming" by Eric Lengyel
		// Tangent part
		#ifdef TANGENT_ENABLED
		vector2Subtract(mesh->vertexArray[currentFace->index[2]].text,mesh->vertexArray[currentFace->index[0]].text,st1);
		vector2Subtract(mesh->vertexArray[currentFace->index[1]].text,mesh->vertexArray[currentFace->index[0]].text,st2);
		
		coef = 1/ (st1[0] * st2[1] - st2[0] * st1[1]);
		
		tangent[0] = coef * (v1[0] * st2[1]  + v2[0] * -st1[1]);
		tangent[1] = coef * (v1[1] * st2[1]  + v2[1] * -st1[1]);
		tangent[2] = coef * (v1[2] * st2[1]  + v2[2] * -st1[1]);
		
		vectorAdd(tangentAccumulator[currentFace->index[0]],tangent,tangentAccumulator[currentFace->index[0]]);
		vectorAdd(tangentAccumulator[currentFace->index[1]],tangent,tangentAccumulator[currentFace->index[1]]);
		vectorAdd(tangentAccumulator[currentFace->index[2]],tangent,tangentAccumulator[currentFace->index[2]]);
						
		#endif
	}
	
	
	for(verticesCounter=0 ; verticesCounter < mesh->num_verts ; verticesCounter++,currentVertex++)
	{
		normalize(normalAccumulator[verticesCounter]);
		#ifdef TANGENT_ENABLED			
		normalize(tangentAccumulator[verticesCounter]);
		#endif
	}
	
	//Now we have all the normal for this model, but need to transform them in bone space for re-usage
	// Translating the normal orientation from object to joint space and Store normals inside weights, 
	md5Vertex = mesh->vertices;
	currentVertex = mesh->vertexArray;
	for(verticesCounter=0 ; verticesCounter < mesh->num_verts ; verticesCounter++,md5Vertex++)
	{
		for (weightCounter = 0; weightCounter < md5Vertex->count; weightCounter++)
		{
			weight = &mesh->weights[md5Vertex->start + weightCounter];
			joint  = &skeleton[weight->joint];
			
			multiplyByInvertQuaternion(normalAccumulator[verticesCounter],joint->orient,jointSpaceNormal);
			vectorAdd(normalWeightAccumulator[md5Vertex->start + weightCounter],jointSpaceNormal,normalWeightAccumulator[md5Vertex->start + weightCounter]);
						
			#ifdef TANGENT_ENABLED			
			multiplyByInvertQuaternion(tangentAccumulator[verticesCounter],joint->orient,jointSpaceTangent);
			vectorAdd(tangentWeightAccumulator[md5Vertex->start + weightCounter],jointSpaceTangent,tangentWeightAccumulator[md5Vertex->start + weightCounter]);
			#endif
						
		}
		
	}
	
	weight = mesh->weights;
	for (weightCounter = 0; weightCounter < mesh->num_weights; weightCounter++,weight++)
	{
		normalize(normalWeightAccumulator[weightCounter]);
		vectorScale(normalWeightAccumulator[weightCounter],32767,weight->normal);
		#ifdef TANGENT_ENABLED			
			normalize(tangentWeightAccumulator[weightCounter]);
		vectorScale(tangentWeightAccumulator[weightCounter],32767,weight->tangent);
		#endif			
	}
	
	free(normalAccumulator);
	free(normalWeightAccumulator);
	#ifdef TANGENT_ENABLED	
	free(tangentAccumulator);
	free(tangentWeightAccumulator);
	#endif	
}

void MD5_Update(md5_object_t* md5Object)
{
	int currentFrame ;
	md5_mesh_t* currentMesh;
	float absoluteTimePointerAnim;
	int i;
	
	absoluteTimePointerAnim = simulationTime/1000.0*md5Object->md5Anim.frameRate;
	
	currentFrame = ((int)absoluteTimePointerAnim)%md5Object->md5Anim.num_frames;
	
	InterpolateSkeletons (currentFrame,
						  md5Object->md5Anim.num_frames,
						  md5Object->md5Anim.num_joints,
						  absoluteTimePointerAnim - (int)absoluteTimePointerAnim,
						  md5Object->md5Anim.skelFrames,
						  md5Object->md5Model.baseSkel);
	
	for(i = 0; i < md5Object->md5Model.num_meshes; i++)
	{
		currentMesh = &md5Object->md5Model.meshes[i];
		GenerateGPUVertices (currentMesh,md5Object->md5Model.baseSkel);
	}
}