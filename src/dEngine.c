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
 *  main.c
 *  dEngine
 *
 *  Created by fabien sanglard on 09/08/09.
 *
 */

#include "dEngine.h"

#include "globals.h"
#include "math.h"
#include "camera.h"
#include "renderer_fixed.h"
#include "renderer_progr.h"
#include "filesystem.h"
#include "timer.h"
#include "world.h"
#include "material.h"
#include "commands.h"




void dEngine_Init(uchar rendererType,int viewPortWidth,int viewPortHeight)
{
	FS_InitFilesystem();
	SCR_Init(rendererType,viewPortWidth,viewPortHeight);
	
	TEXT_InitTextureLibrary();

	
	FM_Init();
	

	
	
	MATLIB_LoadLibraries();
	World_Init();
	CAM_Init();
	
	
	
	Timer_resetTime();
	Timer_Resume();
}



void dEngine_HostFrame(void)
{
	
	Timer_tick();

	
	Comm_Update();
	CAM_Update();
	
	World_Update();
	
	SCR_RenderFrame();

}

uint num_screenshot=0;

void dEngine_WriteScreenshot(char* directory, uchar rotate)
{
	uchar* data;
	int i;//,j;
	FILE* pScreenshot;
	char fullPath[256];
	char num[5];
	uchar tga_header[18];
	uchar* pixel;
	uchar tmpChannel;
	
	sprintf(num, "%03d", num_screenshot++);
	//itoa(,num,3);
	
	memset(fullPath, 256, sizeof(char));
	strcat(fullPath,directory);
	strcat(fullPath,num);
	strcat(fullPath,".tga");
	
	data = calloc(renderHeight*renderWidth, 4);
	
	SCR_GetColorBuffer(data);
	
	pScreenshot = fopen(fullPath, "wb");
		
	memset(tga_header, 0, 18);
	tga_header[2] = 2;
	tga_header[12] = (renderWidth & 0x00FF);
	tga_header[13] = (renderWidth  & 0xFF00) / 256;
	tga_header[14] = (renderHeight  & 0x00FF) ;
	tga_header[15] =(renderHeight & 0xFF00) / 256;
	tga_header[16] = 32 ;
	
	
	/*
	uchar  identsize;          // size of ID field that follows 18 byte header (0 usually)
    uchar  colourmaptype;      // type of colour map 0=none, 1=has palette
    uchar  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	
    short colourmapstart;     // first colour map entry in palette
    short colourmaplength;    // number of colours in palette
    uchar  colourmapbits;      // number of bits per palette entry 15,16,24,32
	
    short xstart;             // image x origin
    short ystart;             // image y origin
    short width;              // image width in pixels
    short height;             // image height in pixels
    uchar  bits;               // image bits per pixel 8,16,24,32
    uchar  descriptor;         // image descriptor bits (vh flip bits)
    */
	
	fwrite(&tga_header, 18, sizeof(uchar), pScreenshot);
	
	//BGR > RGB
	pixel = data;
	for(i=0 ; i < renderWidth * renderHeight ; i++)
	{
		tmpChannel = pixel[0];
		pixel[0] = pixel[2];
		pixel[2] = tmpChannel;
		
		pixel += 4;
	}
	
	
	fwrite(data, renderWidth * renderHeight, 4 * sizeof(uchar), pScreenshot);
	
	fclose(pScreenshot);
	free(data);
}