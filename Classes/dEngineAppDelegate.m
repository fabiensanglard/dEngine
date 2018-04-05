//
//  dEngineAppDelegate.m
//  dEngine
//
//  Created by fabien sanglard on 09/08/09.
//

#import "dEngineAppDelegate.h"
#import "EAGLView.h"
#import "camera.h"

@implementation dEngineAppDelegate

@synthesize window;
@synthesize glView;

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
	[[UIApplication sharedApplication] setStatusBarHidden:YES ];
	[glView startAnimation];
}

- (void) applicationWillResignActive:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
	[glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[glView stopAnimation];
	
	
}

- (void) stopEngineActivity
{
	[glView stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    //No multi-tasking, when you die, you die. Period.
	exit(0);
	//[self stopEngineActivity];
    
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	[glView startAnimation];
}


- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

@end
