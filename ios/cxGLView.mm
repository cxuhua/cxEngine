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

//#import <AVFoundation/AVFoundation.h>
//#import <MediaPlayer/MediaPlayer.h>
//#import <AssetsLibrary/AssetsLibrary.h>
//#include <core/cxUtil.h>
//
//AVAssetWriterInput *writerInput = nil;
//AVAssetWriter *videoWriter = nil;
//AVAssetWriterInputPixelBufferAdaptor *adaptor = nil;

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
        cxULong i = (cxULong)touch;
        CGPoint p = [touch locationInView:self];
        cxEngine::Instance()->Dispatch(i, type,p.x, p.y);
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
    
    cxEngine::Instance()->Layout(width, height);
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
    [EAGLContext setCurrentContext:context];

    glGenFramebuffers(1, &frameBuffer);
    glGenRenderbuffers(1, &renderBuffer);
    
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    
    IOSDelegate *app = (IOSDelegate *)[[UIApplication sharedApplication] delegate];
    [app InitOK];
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

-(EAGLContext *)GLContext
{
    return context;
}

- (void)glkViewControllerUpdate:(GLKViewController *)controller
{
//    static int i = 0;
//    static int frame = 0;
    cxEngine::Instance()->Run();
//    if(videoWriter == nil){
//        NSError *error = nil;
//        NSString *betaCompressionDirectory = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/Movie.mp4"];
//        videoWriter = [[AVAssetWriter alloc] initWithURL:[NSURL fileURLWithPath:betaCompressionDirectory]
//                                                fileType:AVFileTypeMPEG4
//                                                   error:&error];
//        
//        CGSize size = CGSizeMake(2048, 1536);
//
//        NSURL *url = [NSURL fileURLWithPath:betaCompressionDirectory];
//        MPMoviePlayerController *movie = [[MPMoviePlayerController alloc] initWithContentURL:url];
//        movie.controlStyle = MPMovieControlStyleFullscreen;
//        [movie.view setFrame:self.view.bounds];
//        movie.initialPlaybackTime = -1;
//        [self.view addSubview:movie.view];
//        [movie play];
//        writerInput = nil;
//        
//        return;
//
//        unlink([betaCompressionDirectory UTF8String]);
//        
//        //----initialize compression engine
//
//        NSParameterAssert(videoWriter);
//        if(error)
//            NSLog(@"error = %@", [error localizedDescription]);
//        
//        NSDictionary *videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:AVVideoCodecH264, AVVideoCodecKey,
//                                       [NSNumber numberWithInt:size.width], AVVideoWidthKey,
//                                       [NSNumber numberWithInt:size.height], AVVideoHeightKey, nil];
//        writerInput = [AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeVideo outputSettings:videoSettings];
//        writerInput.transform = CGAffineTransformScale(writerInput.transform, 1.0, -1.0f);
//        
//        NSDictionary *attrs = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA], kCVPixelBufferPixelFormatTypeKey, nil];
//        
//        adaptor = [AVAssetWriterInputPixelBufferAdaptor assetWriterInputPixelBufferAdaptorWithAssetWriterInput:writerInput sourcePixelBufferAttributes:attrs];
//        [adaptor retain];
//        
//        NSParameterAssert(writerInput);
//        NSParameterAssert([videoWriter canAddInput:writerInput]);
//        
//        [videoWriter addInput:writerInput];
//        
//        [videoWriter startWriting];
//        [videoWriter startSessionAtSourceTime:kCMTimeZero];
//    }
//    
//    if(writerInput!=nil && [writerInput isReadyForMoreMediaData])
//    {
//        if(++frame >= 1024)
//        {
//            [writerInput markAsFinished];
//            [videoWriter finishWritingWithCompletionHandler:^{
//                NSLog(@"write finished");
//            }];
//            [videoWriter release];
//            writerInput = nil;
//            return;
//        }
//        CVPixelBufferRef pixel = NULL;
//        CVPixelBufferPoolRef pool = [adaptor pixelBufferPool];
//        CVReturn status = CVPixelBufferPoolCreatePixelBuffer (kCFAllocatorDefault, pool, &pixel);
//        if(status == kCVReturnSuccess){
//            CVPixelBufferLockBaseAddress(pixel, 0);
//            GLubyte *buffer = (GLubyte *)CVPixelBufferGetBaseAddress(pixel);
//            glReadPixels(0, 0, 2048, 1536, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
//            if(videoWriter.status == AVAssetWriterStatusWriting){
//                [adaptor appendPixelBuffer:pixel withPresentationTime:CMTimeMake(frame, 60)];
//            }
//            CVPixelBufferUnlockBaseAddress(pixel, 0);
//            CFRelease(pixel);
//            NSLog(@"write frame %d",frame);
//        }
//    }
//    
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
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
