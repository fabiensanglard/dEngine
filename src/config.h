/*
 *  config.h
 *  dEngine
 *
 *  Created by fabien sanglard on 13/09/09.
 *
 */


#ifndef DE_CONFIG
#define DE_CONFIG
	

#ifdef _ARM_ARCH_7
	#define TANGENT_ENABLED 1
#else
	#ifdef TARGET_IPHONE_SIMULATOR
		#define TANGENT_ENABLED 1
	#else
		#define TANGENT_ENABLED 0
	#endif
#endif




#endif
