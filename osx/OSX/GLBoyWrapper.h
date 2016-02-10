#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

//@class OSXPlayer;

@interface GLBoyWrapper : NSObject
{
}

-(void) initGLBoyWidth:(int) width Height:(int) height;
-(void) setup;
-(void) render;
-(void) sizeWidth:(int) width Height:(int) height;

@end