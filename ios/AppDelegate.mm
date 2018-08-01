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

@implementation AppDelegate

-(void)cxDisableDocumentBackup
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    if(paths.count == 0){
        NSLog(@"user document path error");
        return;
    }
    NSString *docPath = [paths objectAtIndex:0];
    NSURL *url = [NSURL fileURLWithPath:docPath];
    NSError *error = NULL;
    [url setResourceValue:[NSNumber numberWithBool:YES] forKey:NSURLIsExcludedFromBackupKey error:&error];
    if(error != NULL){
        NSLog(@"setResourceValue error:%@",error);
    }
}

-(EAGLContext *)GLContext
{
    IOSViewController *root = (IOSViewController *)self.window.rootViewController;
    return [root GLContext];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    cxengine::cxEngine::Startup(false);
    
    [self cxDisableDocumentBackup];
    
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    self.window.contentScaleFactor = [UIScreen mainScreen].scale;
    
    IOSViewController *glController = [[IOSViewController alloc] init];
    self.window.rootViewController = glController;
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
    cxEngine::Instance()->Warning();
}

-(void)dealloc
{
    self.window = nil;
    [super dealloc];
}

@end
