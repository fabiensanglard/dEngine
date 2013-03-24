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
 *  renderer.c
 *  dEngine
 *
 *  Created by fabien sanglard on 10/08/09.
 *
 */

#include "renderer.h"
#include <math.h>
#include "renderer_fixed.h"
#include "renderer_progr.h"
#include "stats.h"
#include "timer.h"

int renderWidth;
int renderHeight;
float renderAspect;

renderer_t renderer;



//2D text and stats
char* textSwitchStr = "Texture Switches: %d";
char* shadSwitchStr = "Shader Switches: %d";
char* blendSwitchStr = "Blending Switches: %d";
char* polyCount = "Poly Count: %d";
char* msCount = "ms: %d";

char fpsText[80]; 
char teSwText[80]; 
char shSwText[80]; 
char bldwText[80]; 
char polCnText[80]; 
char msText[80]; 


ushort indicesString[256]  ;


svertex_t verticesSprite[4];
ushort indicesSprite[6] =  {1,2,3,1,3,4};

void SCR_Init(int rendererType,int viewPortWidth,int viewPortHeight)
{
	int i;
	svertex_t* v;
	
	//renderWidth =  768;
	//renderHeight = 1024;
	
	//renderWidth =  320;
	//renderHeight = 480;
	
    renderWidth =  viewPortWidth;
    renderHeight = viewPortHeight;
    
	if (rendererType == GL_11_RENDERER)
	{
		printf("Running as openGL ES 1.1\n");
		initFixedRenderer(&renderer);
	}
	
	if (rendererType == GL_20_RENDERER)
	{
		printf("Running as openGL ES 2.0\n"); 
		initProgrRenderer(&renderer);
	}
	
	//Generate indices for text rendition
	for(i=0; i < 42 ; i++)
	{
		indicesString[i*6] = i*4;
		indicesString[i*6+1] = i*4+1;
		indicesString[i*6+2] = i*4+2;
		indicesString[i*6+3] = i*4;
		indicesString[i*6+4] = i*4+2;
		indicesString[i*6+5] = i*4+3;
		
	}
	
	
	//Sprite stuff
	v = verticesSprite;
	//	4 3
	//	1 2
	/*1*/v->text[0] = 0 ; 	v->text[1] = 0; 	v->pos[0] = 0;				v->pos[1] = 0;				v++;
	/*2*/v->text[0] = 1;	v->text[1] = 0;		v->pos[0] = renderWidth ;	v->pos[1] = 0;				v++;
	/*3*/v->text[0] = 1;	v->text[1] = 1;		v->pos[0] = renderWidth ;	v->pos[1] = renderHeight;	v++;
	/*4*/v->text[0] = 0;	v->text[1] = 1;		v->pos[0] =  0;				v->pos[1] = renderHeight;	v++;
	
}



void DrawStats()
{
	static svertex_t vertexString[256];
	uint numVertices;
	
	if (fps > 99)
		sprintf( fpsText, "fps:%d",fps );
	else if (fps > 9) 
		sprintf( fpsText, "fps: %d",fps );
	else
		sprintf( fpsText, "fps:  %d",fps );
	
	sprintf( teSwText, textSwitchStr,STATS_GetTexSwitchCount() );
	sprintf( polCnText, polyCount,STATS_GetTrianglesCount() );
	sprintf(msText, msCount,simulationTime);
	sprintf(shSwText, shadSwitchStr, STATS_GetShaderSwitchCount());
	sprintf(bldwText, blendSwitchStr, STATS_GetBlendingSwitchCount());
	
	renderer.SetTexture(font.texfont->textureId);
	
	numVertices = Font_GenerateStringVertexArray(vertexString,fpsText,300,400);
	renderer.RenderString(vertexString,indicesString,numVertices/4*6);
	
	numVertices = Font_GenerateStringVertexArray(vertexString,teSwText,300, 360);
	renderer.RenderString(vertexString,indicesString,numVertices/4*6);
	
	numVertices = Font_GenerateStringVertexArray(vertexString,polCnText,300, 320);
	renderer.RenderString(vertexString,indicesString,numVertices/4*6);
	
	numVertices = Font_GenerateStringVertexArray(vertexString,msText,300, 280);
	renderer.RenderString(vertexString,indicesString,numVertices/4*6);
	
	numVertices = Font_GenerateStringVertexArray(vertexString,shSwText,300, 240);
	renderer.RenderString(vertexString,indicesString,numVertices/4*6);
	
	numVertices = Font_GenerateStringVertexArray(vertexString,bldwText,300, 200);
	renderer.RenderString(vertexString,indicesString,numVertices/4*6);
	
}

void SCR_RenderFrame(void)
{
	STATS_Begin();
		
	renderer.Set3D();
	
	renderer.RenderEntities();
	
	
	
	if (renderer.statsEnabled)
	{
		renderer.Set2D();
		DrawStats();
	}
	
	renderer.StopRendition();
}


inline void gluPerspective(float fovy, float aspect, float zNear, float zFar,matrix_t projectionMatrix)
{
	float f  = (float)(1 / tan(fovy*DEG_TO_RAD/2));	
	
	
	projectionMatrix[0]= f/aspect;	projectionMatrix[4]= 0;	projectionMatrix[ 8]= 0;								projectionMatrix[12]= 0;
	projectionMatrix[1]= 0; 		projectionMatrix[5]= f;	projectionMatrix[ 9]= 0;								projectionMatrix[13]= 0;
	projectionMatrix[2]= 0;			projectionMatrix[6]= 0;	projectionMatrix[10]=(zFar+zNear)/(zNear-zFar) ;		projectionMatrix[14]= 2*(zFar*zNear)/(zNear-zFar);
	projectionMatrix[3]= 0;			projectionMatrix[7]=0;	projectionMatrix[11]=-1;								projectionMatrix[15]= 0;
}





inline  void gluLookAt(  vec3_t vEye,  vec3_t vLookat, vec3_t vUp ,matrix_t fModelView)
{
	vec3_t vN,vU,vV;
	
    // determine the new n
    vectorSubtract(vEye,vLookat,vN);
	
    // determine the new u by crossing with the up vector
    vectorCrossProduct(vUp, vN, vU) ;
	
    // normalize both the u and n vectors
    normalize(vU) ; 
		normalize(vN);
	
    // determine v by crossing n and u
    vectorCrossProduct(vN,vU,vV);
	
    // create a model view matrix
	fModelView[0] = vU[0];					fModelView[4] = vU[1];					fModelView[8] = vU[2];					fModelView[12] = - DotProduct(vEye,vU); 
	fModelView[1] = vV[0];					fModelView[5] = vV[1];					fModelView[9] = vV[2];					fModelView[13] = - DotProduct(vEye,vV);
	fModelView[2] = vN[0];					fModelView[6] = vN[1];					fModelView[10]= vN[2];					fModelView[14]=  - DotProduct(vEye,vN);
	fModelView[3]=	0.0f;					fModelView[7]= 0.0f;					fModelView[11]= 0.0f;					fModelView[15]= 1.0f;

}


void SCR_GetColorBuffer(uchar* data)
{
	renderer.GetColorBuffer(data);
}