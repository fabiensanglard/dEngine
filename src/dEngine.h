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
 *  main.h
 *  dEngine
 *
 *  Created by fabien sanglard on 09/08/09.
 *
 */


#ifndef DE_MAIN
#define DE_MAIN 1




#include "globals.h"
#include "renderer.h"


void dEngine_Init(uchar rendererType, int viewPortWidth, int viewPortHeight);
void dEngine_HostFrame(void);
void dEngine_WriteScreenshot(char* directory, uchar rotate);



#endif

/*		TODO
	 
 Add precalculator Visibility Set
 Add support for Camera path (md5camera)

 Consolidate shaders into uberShader
 Fix shadowMapping Moire pattern (polyOffset)
 Implement VSM shadowMapping
 Specular support
 Specular map
 Change OBJ to use short texture coo only.
 Fix OBJ texture coordinate (1-y)
 */


/*
 
 October 10th
 
 Multifinger mouvement.
 
 
 */

/*

 October 9th
 
 Add FPS control.
 Fix OBJ loading (reuse vertices when building vertexArray) Done
 
 */

/*
 
 October 5th
 
	Add shadowMapping					Done
 
 */

/*
 
 September 23th
 
  Modelspace bumpmapping				Done
  Doom3 map imported.					Done
 
 */

/*
 September 11th
 Change bumpMapping to use model space	Done (No gain in terms of framerate :( !!! )
 */

/*
 September 8th
 Add bumpMapping						Done
 
*/

/*
	September 1th

	Add shader matrixStack				Done
	Add skeleton interpolated md5 anim	Done
 */


/*
	August 29th
	
	Add md5anim support					Done
	Add lighting support				Done
 
 */

/*
	August 18th
	Add stats and framerate display		Done
*/

/*
	August 16th
 
	Add PVRTC texture support			Done	 
 */

/*
	Project started on August 15th
 
 
	Add 3GS rendering path			Done
	Add md5 support					Done
	Add filesystem					Done
	
 */


