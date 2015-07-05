//
//  cxViewController.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#import <OpenGLES/ES2/glext.h>
#include "AppDelegate.h"
#import "cxGLView.h"

using namespace cxengine;

@implementation IOSGLView

-(void)postTouchesEvent:(NSSet *)touches
{
    for (UITouch *touch in touches) {
        cxUInt type = 0;
        if(touch.phase == UITouchPhaseBegan){
            type = cxTouchPoint::Began;
        }else if(touch.phase == UITouchPhaseMoved){
            type = cxTouchPoint::Moved;
        }else if(touch.phase == UITouchPhaseEnded){
            type = cxTouchPoint::Ended;
        }else if(touch.phase == UITouchPhaseCancelled){
            type = cxTouchPoint::Ended;
        }
        if(type == 0){
            CX_WARN("type not process");
            continue;
        }
        cxUInt  t = (cxUInt)touch.tapCount;
        cxULong i = (cxULong)touch;
        CGPoint p = [touch locationInView:self];
        cxEngine::Instance()->Dispatch(i, type, t, p.x, p.y);
    }
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self postTouchesEvent:touches];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self postTouchesEvent:touches];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self postTouchesEvent:touches];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self postTouchesEvent:touches];
}

-(void)dealloc
{
    [super dealloc];
}

@end

@implementation IOSViewController

-(void)viewDidLayoutSubviews
{
    CAEAGLLayer *layer = (CAEAGLLayer*)self.view.layer;
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    cxengine:cxEngine::Instance()->Layout(width, height);
}

-(void)viewDidLoad
{
    IOSGLView *glview = [[IOSGLView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.view = glview;
    [glview release];
    
    [self.view setMultipleTouchEnabled:YES];
    self.delegate = self;
    self.preferredFramesPerSecond = 60;

    [super viewDidLoad];
    
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    context.multiThreaded = YES;
    [EAGLContext setCurrentContext:context];

    glGenFramebuffers(1, &frameBuffer);
    glGenRenderbuffers(1, &renderBuffer);
    
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
}

- (void)viewDidUnload
{
    cxEngine::Destroy();
    [EAGLContext setCurrentContext:nil];
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteRenderbuffers(1, &renderBuffer);
    [context release];
    [super viewDidUnload];
}

- (void)glkViewControllerUpdate:(GLKViewController *)controller
{
    cxEngine::Instance()->Run();
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAll;
}

- (BOOL) shouldAutorotate
{
    return YES;
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}


@end
