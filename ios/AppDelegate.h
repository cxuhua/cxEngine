//
//  AppDelegate.h
//  cxEngine2D
//
//  Created by xuhua on 5/20/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#import <AVFoundation/AVAudioPlayer.h>
#import <UIKit/UIKit.h>
#import "cxGLView.h"

@interface IOSApplication : UIApplication
{
    
}
@end

@interface IOSDelegate : UIResponder <UIApplicationDelegate,AVAudioPlayerDelegate>

@property(retain,nonatomic)UIWindow *window;

@property(retain,nonatomic)IOSViewController *rootViewController;
@end

