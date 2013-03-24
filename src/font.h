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
