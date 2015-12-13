//
//  AppDelegate.m
//  cxEngine2D
//
//  Created by xuhua on 5/20/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#import "AppDelegate.h"
#include <engine/cxEngine.h>
#include <core/cxUtil.h>

using namespace cxengine;

@implementation IOSApplication

-(void)sendEvent:(UIEvent *)event
{
    [super sendEvent:event];
}

@end

@interface IOSDelegate ()
{
    
}
@end

@implementation IOSDelegate

-(void)cxDisableDocumentBackup
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docPath = [paths objectAtIndex:0];
    NSURL *url = [NSURL fileURLWithPath:docPath];
    NSError *error = NULL;
    [url setResourceValue:[NSNumber numberWithBool:YES] forKey:NSURLIsExcludedFromBackupKey error:&error];
    if(error != NULL){
        NSLog(@"%@",error);
    }
}

-(void)InitOK
{
    
}

-(EAGLContext *)GLContext
{
    return [self.rootViewController GLContext];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self cxDisableDocumentBackup];
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    self.window.contentScaleFactor = [UIScreen mainScreen].scale;
    
    IOSViewController *glController = [[IOSViewController alloc] init];
    self.rootViewController = glController;
    [self.window setRootViewController:glController];
    [glController release];
    
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    cxEngine::Instance()->Pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    cxEngine::Instance()->Resume();
}

-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    cxEngine::Instance()->MemoryWarning();
}

-(void)dealloc
{
    self.window = nil;
    self.rootViewController = nil;
    [super dealloc];
}

@end
