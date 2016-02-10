#import "GLBoyWrapper.h"
#import "OSX-Swift.h"


#include <GLBoy/GLBoy.hpp>
#include <vector>
#include <iostream>

#include "TriangleBackground.hpp"

using namespace glboy;

class Sample01 : public glboy::GLBoy {
public:
	Object::ptr box;
	
	void setup() {
		box = Object::box(200);
		box->bindVertexData();
	}
	
	void draw() {
		camera_to_mouse();
		box->draw();
	}
};

class OSXPlayer : public glboy::Player {
public:
	
	CGPoint screenCenter;
	
	OSXPlayer() {
		NSRect screenRect = [[[NSScreen screens] objectAtIndex:0] visibleFrame];
		screenCenter = {screenRect.size.width/2, screenRect.size.height/2};
	}
	~OSXPlayer() {}
	
	int run() { return 0; }
	
	void mouse_position(float& xpos, float& ypos) {
		NSPoint mouseLoc;
		mouseLoc = [NSEvent mouseLocation]; //get current mouse position
		xpos = mouseLoc.x - screenCenter.x;
		ypos = mouseLoc.y - screenCenter.y;
//		NSLog(@"Mouse location: %f %f", mouseLoc.x, mouseLoc.y);
//		NSWindow* window = [[NSApplication sharedApplication] mainWindow];
//		NSLog(@"window location: %f %f", window.frame.origin.x, window.frame.origin.y);
//		NSPoint localPoint = [window.contentView convertPoint:mouseLoc fromView:nil];
//		NSLog(@"local mouse location: %f %f", mouseLoc.x, mouseLoc.y);
	}
	void frame_rate(int rate) {}
};

@interface GLBoyWrapper ()
{
	OSXPlayer* _player;
	glboy::GLBoy* _glboy;
}
@end

@implementation GLBoyWrapper

-(id)init {
	self = [super init];
	
	if (self) {
		_glboy = new TriangleBackground(800, 640);
		_player = new OSXPlayer();
	}
	
	return self;
}

-(void)dealloc {
	delete _glboy;
	delete _player;
}

-(void)initGLBoyWidth:(int) width Height:(int) height {
	_glboy->init(_player, width, height);
}

-(void)setup {
	_glboy->setup();
}

-(void)render {
//	_glboy->frame_count++;
	_glboy->clear_background();
	_glboy->render();
}

-(void) sizeWidth:(int) width Height:(int) height {
	_glboy->size(width, height);
}

@end