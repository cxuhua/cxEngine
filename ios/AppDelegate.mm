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
#include <engine/cxSound.h>

using namespace cxengine;

@implementation IOSApplication

-(void)sendEvent:(UIEvent *)event
{
    [super sendEvent:event];
}

@end

@interface IOSDelegate ()
{
    AVAudioPlayer *currPlayer;
    cxStr *currFile;
}

-(cxFloat)LoadMusic:(cchars)file;
-(void)PlayMusic:(cxFloat)volume loop:(cxBool)loop;
-(void)SetMusicVolume:(float)volume;
-(void)StopMusic;
-(void)PauseMusic;
-(void)ResumeMusic;

@end

cxFloat cxSound::Load(cchars file)
{
    IOSDelegate *app = (IOSDelegate *)[[UIApplication sharedApplication] delegate];
    return [app LoadMusic:file];
}

void cxSound::Play(cxFloat volume,cxBool loop)
{
    IOSDelegate *app = (IOSDelegate *)[[UIApplication sharedApplication] delegate];
    return [app PlayMusic:volume loop:loop];
}
void cxSound::StopMusic()
{
    IOSDelegate *app = (IOSDelegate *)[[UIApplication sharedApplication] delegate];
    [app StopMusic];
}

void cxSound::SetVolume(cxFloat volume)
{
    IOSDelegate *app = (IOSDelegate *)[[UIApplication sharedApplication] delegate];
    [app SetMusicVolume:volume];
}

void cxSound::Pause()
{
    IOSDelegate *app = (IOSDelegate *)[[UIApplication sharedApplication] delegate];
    [app PauseMusic];
}

void cxSound::Resume()
{
    IOSDelegate *app = (IOSDelegate *)[[UIApplication sharedApplication] delegate];
    [app ResumeMusic];
}

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

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    currFile = nullptr;
    currPlayer = nil;
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

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    CX_LOGGER("%s play finished",currFile->ToString());
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error
{
    CX_ERROR("pay music file %s decode error",currFile->ToString());
}

-(void)PlayMusic:(cxFloat)volume loop:(cxBool)loop
{
    if(currPlayer != nil){
        currPlayer.volume = volume;
        [currPlayer setNumberOfLoops:loop ? -1 : 1];
        [currPlayer play];
    }
}

-(cxFloat)LoadMusic:(cchars)file
{
    CX_ASSERT(cxStr::IsOK(file), "music file arg error");
    [currPlayer stop];
    [currPlayer release];
    currPlayer = nil;
    cxObject::swap(&currFile, cxStr::UTF8(file));
    const cxStr *path = cxUtil::Instance()->AssetsPath(file);
    if(path == NULL){
        CX_ERROR("music path error");
        return 0;
    }
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path->ToString()]];
    NSError *error = nil;
    currPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
    currPlayer.delegate = self;
    if(error != nil ) {
        CX_ERROR("init audio player error %s,file=%s",[[error localizedDescription] UTF8String],file);
        return 0;
    }
    return currPlayer.duration;
}

-(void)SetMusicVolume:(float)volume
{
    if(currPlayer != nil){
        currPlayer.volume = volume;
    }
}

-(void)StopMusic
{
    if(currPlayer != nil){
        [currPlayer stop];
        [currPlayer release];
        currPlayer = nil;
    }
    cxObject::release(&currFile);
}

-(void)PauseMusic
{
    if(currPlayer != nil){
        [currPlayer pause];
    }
}

-(void)ResumeMusic
{
    if(currPlayer != nil){
        [currPlayer play];
    }
}


- (void)applicationDidEnterBackground:(UIApplication *)application
{
    [self PauseMusic];
    cxEngine::Instance()->Pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [self ResumeMusic];
    cxEngine::Instance()->Resume();
}

-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    cxEngine::Instance()->Memory();
}

-(void)dealloc
{
    self.window = nil;
    self.rootViewController = nil;
    [super dealloc];
}

@end
