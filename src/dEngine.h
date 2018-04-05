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


