//
//  cxEngine.m
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreText/CoreText.h>
#include <engine/cxEngine.h>
#include <engine/cxLabel.h>

CX_CPP_BEGIN

#define ToUIColor(c) [UIColor colorWithRed:(c).r green:(c).g blue:(c).b alpha:(c).a]

static CTFrameRef frameRefCreate(NSString *str,UIFont *font,const cxTextAttr &attr,CGSize *size)
{
    UIColor *txtColor = ToUIColor(attr.color);
    CGRect rect = CGRectMake(0, 0, size->width, size->height);
    NSTextAlignment textAlign = NSTextAlignmentLeft;
    if(attr.align == cxTextAlignCenter){
        textAlign = NSTextAlignmentCenter;
    }else if(attr.align == cxTextAlignLeft){
        textAlign = NSTextAlignmentLeft;
    }else if(attr.align == cxTextAlignRight){
        textAlign = NSTextAlignmentRight;
    }
    NSMutableParagraphStyle * paraStyle = [[NSMutableParagraphStyle alloc] init];
    paraStyle.alignment = textAlign;
    paraStyle.lineBreakMode = NSLineBreakByCharWrapping;
    //
    NSMutableDictionary *attrs = [[NSMutableDictionary alloc] init];
    [attrs setObject:font forKey:NSFontAttributeName];
    [attrs setObject:paraStyle forKey:NSParagraphStyleAttributeName];
    [attrs setObject:txtColor forKey:NSForegroundColorAttributeName];
    NSMutableAttributedString *attrsString = [[NSMutableAttributedString alloc] initWithString:str attributes:attrs];
    // Set up frame.
    CTFramesetterRef framesetterRef = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)attrsString);
    //
    CFDictionaryRef attributes = (__bridge CFDictionaryRef)attrs;
    rect.size = CTFramesetterSuggestFrameSizeWithConstraints(framesetterRef,CFRangeMake(0,str.length),attributes,rect.size,NULL);
    rect.size.width  += (fabsf(attr.strokeOffset.x) + 2);
    rect.size.height += (fabsf(attr.strokeOffset.y) + 2);
    //
    CGPathRef pathRef = CGPathCreateWithRect(rect, nil);
    CTFrameRef frameRef = CTFramesetterCreateFrame(framesetterRef, CFRangeMake(0, str.length), pathRef, NULL);
    //release
    CFRelease(framesetterRef);
    CGPathRelease(pathRef);
    
    [attrsString release];
    [attrs release];
    [paraStyle release];
    *size = CGSizeMake(ceilf(rect.size.width), ceilf(rect.size.height));
    return frameRef;
}

static CGImageRef strokeImageCreate(CGRect rect,CTFrameRef frameRef,CGFloat strokeSize,UIColor *strokeColor)
{
    // Create context.
    UIGraphicsBeginImageContextWithOptions(rect.size, NO, 0.0);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetTextDrawingMode(context, kCGTextStroke);
    // Draw clipping mask.
    CGContextSetLineWidth(context, strokeSize);
    CGContextSetLineJoin(context, kCGLineJoinRound);
    [strokeColor setStroke];
    CTFrameDraw(frameRef, context);
    // Save clipping mask.
    CGImageRef clippingMask = CGBitmapContextCreateImage(context);
    // Clear the content.
    CGContextClearRect(context, rect);
    // Draw stroke.
    CGContextClipToMask(context, rect, clippingMask);
    CGContextTranslateCTM(context, 0.0, CGRectGetHeight(rect));
    CGContextScaleCTM(context, 1.0, -1.0);
    [strokeColor setFill];
    UIRectFill(rect);
    // Clean up and return image.
    CGImageRelease(clippingMask);
    CGImageRef image = CGBitmapContextCreateImage(context);
    UIGraphicsEndImageContext();
    return image;
}

static void drawString(CGContextRef ctx,CTFrameRef frameRef,CGRect rect,UIFont *font,const cxTextAttr &attr)
{
    CGContextTranslateCTM(ctx, 0.0, CGRectGetHeight(rect));
    CGContextScaleCTM(ctx, 1.0, -1.0);
    CGContextSaveGState(ctx);
    CGContextSetTextDrawingMode(ctx, kCGTextFill);
    CTFrameDraw(frameRef, ctx);
    CGContextRestoreGState(ctx);
    if(attr.strokeWidth > 0){
        UIGraphicsPushContext(ctx);
        CGRect srect = rect;
        srect.origin.x += attr.strokeOffset.x;
        srect.origin.y += attr.strokeOffset.y;
        CGContextSaveGState(ctx);
        CGContextSetTextDrawingMode(ctx, kCGTextStroke);
        CGImageRef image = CGBitmapContextCreateImage(ctx);
        CGImageRef strokeImage = strokeImageCreate(srect,frameRef,attr.strokeWidth,ToUIColor(attr.strokeColor));
        CGContextDrawImage(ctx, srect, strokeImage);
        CGContextDrawImage(ctx, srect, image);
        CGImageRelease(strokeImage);
        CGImageRelease(image);
        CGContextRestoreGState(ctx);
        UIGraphicsPopContext();
    }
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    [image drawInRect:rect];
}

static NSString *formatString(NSString *str,cxTextNumFormat format)
{
    NSNumberFormatter *formatter = [[[NSNumberFormatter alloc] init] autorelease];
    formatter.locale = [NSLocale currentLocale];
    if(format == cxTextNumFormatDecimal){
        formatter.numberStyle = NSNumberFormatterDecimalStyle;
    }else if(format == cxTextNumFormatCurrency){
        formatter.numberStyle = NSNumberFormatterCurrencyStyle;
    }
    return [formatter stringFromNumber:[NSNumber numberWithInt:[str intValue]]];
}

void cxEngine::Exit()
{
    cxEngine::Destroy();
    exit(0);
}

const cxStr *cxEngine::TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size)
{
    if(!cxStr::IsOK(txt)){
        return NULL;
    }
    NSString *str = [NSString stringWithUTF8String:txt->Data()];
    if(str == nil){
        return NULL;
    }
    if(attr.format != cxTextNumFormatNone){
        str = formatString(str, attr.format);
    }
    UIFont *font = nil;
    if(attr.boldFont){
        font = [UIFont boldSystemFontOfSize:attr.size];
    }else {
        font = [UIFont systemFontOfSize:attr.size];
    }
    cxInt max = cxOpenGL::Instance()->maxTextureSize;
    CGSize dim = (attr.fixWidth > 0)?CGSizeMake(attr.fixWidth, max):CGSizeMake(max, max);
    CTFrameRef frameRef = frameRefCreate(str, font, attr, &dim);
    if(frameRef == nil){
        return NULL;
    }
    //RGBA
    cxInt bufsiz = (int)(dim.width * dim.height) * 4;
    cxStr *rv = cxStr::Create()->Init(bufsiz);
    chars buffer = rv->Buffer();
    CGBitmapInfo bitMapInfo = kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault;
    CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(buffer,dim.width,dim.height,8,(int)(dim.width * 4),colorSpace,bitMapInfo);
    CX_ASSERT(context != NULL, "CGBitmapContextCreate create failed");
    CGContextTranslateCTM(context, 0.0f, dim.height);
    CGContextScaleCTM(context, 1.0f, -1.0f);
    UIGraphicsPushContext(context);
    drawString(context,frameRef, CGRectMake(0, 0, dim.width, dim.height), font, attr);
    UIGraphicsPopContext();
    CGContextRelease(context);
    CFRelease(frameRef);
    CGColorSpaceRelease(colorSpace);
    size.w = (int)dim.width;
    size.h = (int)dim.height;
    return rv;
}

CX_CPP_END
