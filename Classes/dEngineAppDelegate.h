//
//  dEngineAppDelegate.h
//  dEngine
//
//  Created by fabien sanglard on 09/08/09.
//

#import <UIKit/UIKit.h>

#define MAX_FPS 1.0f/45.f
#define IDLE_FPS 1.0f/5.f

@class EAGLView;

@interface dEngineAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

