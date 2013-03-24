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
 *  camera.c
 *  dEngine
 *
 *  Created by fabien sanglard on 15/08/09.
 *
 */

#include "camera.h"
#include "renderer.h"
#include "timer.h"
#include "filesystem.h"
#include "lexer.h"


uint cameraAccTime;
#define REC_FREQUENCY 33

camera_t camera;

matrix3x3_t orientationMatrix;

void CAM_Init()
{
	
	float fov,zFar,zNear;
	vec3_t pos ;
	
	pos[0] = 40;
	pos[1] = 80;
	pos[2] = -40;
	

	
	fov = 45;
	
	zFar = 2000;
	zNear = 10;
	
	camera.recording = 0;
	camera.playing = 0;
	
	CAM_SetCamera(pos, 0, -0.225, renderWidth*1.0f/renderHeight,fov, zNear, zFar);
}

void CAM_GenerateVectors(matrix3x3_t orientationMatrix)
{
	vec3_t right = {0,-1,0};	
	vec3_t up = {1,0,0};
	vec3_t forward = {0,0,1};
	
	//vec3_t right = {1,0,0};	
	//vec3_t up = {0,1,0};
	//vec3_t forward = {0,0,1};
	
	
	matrix_transform_vec3t(orientationMatrix,right,camera.right);
	matrix_transform_vec3t(orientationMatrix,up,camera.up);
	matrix_transform_vec3t(orientationMatrix,forward,camera.forward);
}



camera_frame_t* CAM_RecordCurrentFrame(void)
{
	camera_frame_t* newFrame;
	
	newFrame = calloc(1,sizeof(camera_frame_t));
	newFrame->next = 0;
	newFrame->time = simulationTime;
	
	vectorCopy(camera.position,newFrame->position);
	Quat_CreateFromMat3x3(orientationMatrix, newFrame->orientation);
	
	return  newFrame;
}

void CAM_UpdateOrientationMatrix(void)
{
	
	matrix3x3_t rotHead,rotPitch;
	
	
	//Rotation head (y)
	rotHead[0] = cos(camera.head)	;  rotHead[3] = 0	;			rotHead[6] =  -sin(camera.head);  
	rotHead[1] = 0					;  rotHead[4] = 1	;			rotHead[7] = 0;  
	rotHead[2] = sin(camera.head)	;  rotHead[5] = 0	;			rotHead[8] = cos(camera.head);  	
	
	
	//Rotation pitch (x)
	rotPitch[0] = 1;  rotPitch[3] = 0					;  rotPitch[6] = 0;  
	rotPitch[1] = 0;  rotPitch[4] = cos(camera.pitch)	;  rotPitch[7] = -sin(camera.pitch);  
	rotPitch[2] = 0;  rotPitch[5] = sin(camera.pitch)	;  rotPitch[8] = cos(camera.pitch); 	
	
	
	matrix_multiply3x3(rotHead,rotPitch,orientationMatrix);
	
	//matrix_print3x3(concatenationMatrix);
	
	CAM_GenerateVectors(orientationMatrix);
	
	
	if (camera.recording)
	{
		cameraAccTime += timediff;
		if (cameraAccTime > REC_FREQUENCY)
		{
			cameraAccTime=0;
			
			camera.path.currentFrame->next = CAM_RecordCurrentFrame();
			camera.path.currentFrame = camera.path.currentFrame->next;
			
			camera.path.num_frames++;

		}
	}
	
}


void CAM_Update(void)
{
	float			interpolationFactor;
	quat4_t			interpolatedQuaterion;
	matrix3x3_t		interpolatedOrientationMatrix;
	camera_frame_t* nextFrame = 0;
	
	if (camera.playing)
	{
		
		while (camera.path.currentFrame->next != 0 && camera.path.currentFrame->next->time < simulationTime) {
			camera.path.currentFrame = camera.path.currentFrame->next;
		}
		
		if (camera.path.currentFrame->next == 0)
			return;
		
		nextFrame = camera.path.currentFrame->next;
		
		interpolationFactor = (simulationTime - camera.path.currentFrame->time) * 1.0 / (nextFrame->time - camera.path.currentFrame->time);
				
		if (interpolationFactor >= 1)
			return;
		
		//Interpolate position
		vectorInterpolate(camera.path.currentFrame->position,nextFrame->position,interpolationFactor,camera.position);
		
		//Interpolate quaternion
		Quat_slerp(camera.path.currentFrame->orientation, nextFrame->orientation, interpolationFactor, interpolatedQuaterion);
		
		//Transforme quat to matrix and set it as orientation
		Quat_ConvertToMat3x3(interpolatedOrientationMatrix, interpolatedQuaterion);
		
		
		CAM_GenerateVectors(interpolatedOrientationMatrix);		
		
	}
	else 
	{
		CAM_UpdateOrientationMatrix();
	}
	
	
}

void CAM_SetCamera(vec3_t position,float head,float pitch,float aspect , float fov , float zNearl , float zFarl)
{
	vectorCopy(position,camera.position);

	camera.position[3] = 1;
	
	camera.head = head;
	camera.pitch=pitch;
	
	CAM_UpdateOrientationMatrix();
	
	camera.aspect = aspect;
	camera.fov = fov;
	camera.zNear = zNearl;
	camera.zFar = zFarl;
	
}



void CAM_StartRecording()
{
	
	printf("[Camera] Start recording.\n");
	
	
	camera.recording=1;
	
	cameraAccTime = 0;
	
	camera.path.num_frames=0;
	camera.path.frames = CAM_RecordCurrentFrame();
	camera.path.currentFrame = camera.path.frames;
	
}

void CAM_StopRecording()
{
	camera_frame_t* frame;
	
	printf("[Camera] Stop recording.\n");
	
	camera.recording=0;
	
	//Convert history for whaterver format and 
	printf("cp1\n");
	printf("num_frames %u\n",camera.path.num_frames);
	
	frame = camera.path.frames;
	
	while (frame != 0) 
	{
		printf("time %u: (%.4f,%.4f,%.4f) q (%.4f,%.4f,%.4f,%.4f)\n",
			   frame->time,
			   frame->position[0],
			   frame->position[1],
			   frame->position[2],
			   frame->orientation[0],
			   frame->orientation[1],
			   frame->orientation[2],
			   frame->orientation[3]			   
			   );
		
		frame = frame->next;
	}
	
	
	cameraAccTime = 0 ;
	camera.path.num_frames = 0;
}

camera_frame_t* CAM_ReadFrameFromFile(void)
{
	camera_frame_t* frame ;
	matrix3x3_t rotMatrix;
	int i;
	
	frame = calloc(1,sizeof(camera_frame_t));
	
	
	LE_readToken(); //time
	frame->time = LE_readReal();
	
	frame->position[0] = LE_readReal();
	frame->position[1] = LE_readReal();
	frame->position[2] = LE_readReal();
	
	LE_readToken(); //q or m
	if (!strcmp("q", LE_getCurrentToken()))
	{
		for( i=0; i < 4 ; i++)
		frame->orientation[i]= LE_readReal();
	}
	else if (!strcmp("m", LE_getCurrentToken()))
	{
		for( i=0; i < 9 ; i++)
			rotMatrix[i] = LE_readReal();
		
		Quat_CreateFromMat3x3(rotMatrix,frame->orientation);
		
	}
	
	return frame;
}

void CAM_StartPlaying(char* filename)
{
	uint i;	
	filehandle_t* file ;
	uchar isCP1 = 0;
	
	file = FS_OpenFile(filename, "rt");
	
	if (!file)
		return;
	
	isCP1 = (*file->ptrStart == 'c' && *(file->ptrStart+1) == 'p' && *(file->ptrStart+2) == '1') ;
	
	if (!isCP1)
	{
		printf("Camera path is not a valid file:");
		return;
	}
	
	LE_init(file);
	
	LE_readToken(); //CP1
	LE_readToken(); //num_frames
	
	
	
	camera.path.num_frames = LE_readReal();
	
	camera.path.frames = CAM_ReadFrameFromFile();
	camera.path.frames->next = 0;
	camera.path.currentFrame = camera.path.frames;
	
	for(i=1 ; i < camera.path.num_frames ; i++)
	{
		camera.path.currentFrame->next = CAM_ReadFrameFromFile();
		camera.path.currentFrame = camera.path.currentFrame->next;
		camera.path.currentFrame->next = 0;
	}
							  
	camera.playing = 1;
	camera.path.currentFrame = camera.path.frames;
	strcpy(camera.recordFilename, filename);
	simulationTime = camera.path.frames[0].time;
	
	FS_CloseFile(file);
}
