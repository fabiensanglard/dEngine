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
 *  math.c
 *  dEngine
 *
 *  Created by fabien sanglard on 10/08/09.
 *
 */

#include "math.h"
#include <math.h>

void vectorCrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross ) 
{
	cross[ 0 ] = v1[ 1 ] * v2[ 2 ] - v1[ 2 ] * v2[ 1 ];		// X
	cross[ 1 ] = v1[ 2 ] * v2[ 0 ] - v1[ 0 ] * v2[ 2 ];		// Y
	cross[ 2 ] = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];		// Z
}


void normalize(vec3_t v)
{
	float length, ilength;
	
	length = (float)sqrt( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ] );
	
//	printf("Length = %.3f\n",length);
	
	if( length )
	{
		ilength = 1 / length;
		v[ 0 ] *= ilength;
		v[ 1 ] *= ilength;
		v[ 2 ] *= ilength;
	}
}

void vectorInterpolate(const vec3_t v1,const vec3_t v2,float f,vec3_t dest)
{
	dest[0] = (1-f) *v1[0] + f * v2[0] ;
	dest[1] = (1-f) *v1[1] + f * v2[1] ;
	dest[2] = (1-f) *v1[2] + f * v2[2] ;
}