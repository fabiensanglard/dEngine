/*
 *  camera.h
 *  dEngine
 *
 *  Created by fabien sanglard on 15/08/09.
 *
 */

#ifndef DE_CAMERA
#define DE_CAMERA

#include "globals.h"
#include "math.h"
#include "quaternion.h"

typedef struct visibility_set_t
{
} visibility_set_t;

typedef struct camera_frame_t
{
	uint time;
	vec3_t position;
	quat4_t orientation;
	visibility_set_t* vs;
	struct camera_frame_t* next;
} camera_frame_t;

typedef struct frames_t
{
	uint num_frames;
	camera_frame_t* frames;
	camera_frame_t* currentFrame;
} cam_path_t;

typedef struct 
{
		//vec3_t vEye ;
		//vec3_t vLookat ;
		//vec3_t vUp ;
	
	vec4_t position ;
	
	vec3_t forward ;
	vec3_t right ;
	vec3_t up;
	
	float pitch;
	float head;
		
	float aspect;
	float fov;
	float zNear;
	float zFar;
	
	uchar recording;
	cam_path_t path;
	
	uchar playing;
	char  recordFilename[256];
} camera_t;





extern camera_t camera;

void CAM_Init();
void CAM_SetCamera(vec3_t position,float head,float pitch,float aspect , float fov , float zNearl , float zFarl);
void CAM_Update(void);

void CAM_StartRecording();
void CAM_StopRecording();
void CAM_StartPlaying(char* filename);

#endif