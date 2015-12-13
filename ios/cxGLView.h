//
//  cxViewController.h
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface IOSGLView : GLKView
{
}
@end

@interface IOSViewController : GLKViewController<GLKViewControllerDelegate>
{
    GLuint  frameBuffer;
    GLuint  renderBuffer;
    GLint   width;
    GLint   height;
    EAGLContext *context;
}
-(EAGLContext *)GLContext;
@end
