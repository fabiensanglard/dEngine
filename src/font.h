/*
 *  font.h
 *  dEngine
 *
 *  Created by fabien sanglard on 17/08/09.
 *
 */


#ifndef FONT0
#define FONT0 

#include "globals.h"
#include "texture.h"
#include "math.h"


typedef struct{
	uchar nCharWidth[ 128 ];		// width of each character 
	uchar nMaxWidth;				// box width 
	uchar nMaxHeight;				// box height 
	//SW32 spacing;				// space between characters 
	uint nSize;					
	float wFrac;
	float hFrac;
	
	texture_t* texfont;
} font_t;

extern font_t font;


void FM_Init( void );

void FM_createFont(  char *filename );
ubyte Font_GenerateStringVertexArray(svertex_t* vertices,char* string, int pos_x, int pos_y);

#endif
