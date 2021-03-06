//
//  GLView.swift
//  OSXplayer
//
//  Created by Kanaya Tamihiro on 2016/02/08.
//  Copyright © 2016年 Kanaya Tamihiro. All rights reserved.
//

import Cocoa
import AppKit

class GLView: NSOpenGLView {
	
	var displayLink:CVDisplayLink?	//UnsafeMutablePointer<CVDisplayLink?>.alloc(1)
	
	var glboyWrapper: GLBoyWrapper?
	
	override func awakeFromNib() {
		let attributes: [NSOpenGLPixelFormatAttribute] = [
			// Must specify the 3.2 Core Profile to use OpenGL 3.2
			NSOpenGLPixelFormatAttribute(NSOpenGLPFAOpenGLProfile),
			NSOpenGLPixelFormatAttribute(NSOpenGLProfileVersion3_2Core),
			NSOpenGLPixelFormatAttribute(NSOpenGLPFADoubleBuffer),
			NSOpenGLPixelFormatAttribute(NSOpenGLPFAColorSize),
			NSOpenGLPixelFormatAttribute(32),
			NSOpenGLPixelFormatAttribute(NSOpenGLPFADepthSize),
			NSOpenGLPixelFormatAttribute(24),
			NSOpenGLPixelFormatAttribute(NSOpenGLPFAStencilSize),
			NSOpenGLPixelFormatAttribute(8),
			NSOpenGLPixelFormatAttribute(NSOpenGLPFAMultisample),
			NSOpenGLPixelFormatAttribute(NSOpenGLPFASampleBuffers),
			NSOpenGLPixelFormatAttribute(1),
			NSOpenGLPixelFormatAttribute(NSOpenGLPFASamples),
			NSOpenGLPixelFormatAttribute(4),
			NSOpenGLPixelFormatAttribute(0)
		]
		
		let pf = NSOpenGLPixelFormat(attributes: attributes)
		if pf == nil {
			NSLog("No OpenGL pixel format")
		}
		
		let context = NSOpenGLContext(format: pf!, shareContext: nil)
		CGLEnable(context!.CGLContextObj, kCGLCECrashOnRemovedFunctions)
		
		pixelFormat = pf
		openGLContext = context
		
		
	}
	
	
	override func prepareOpenGL() {
		super.prepareOpenGL()
		
		// Make all the OpenGL calls to setup rendering
		//  and build the necessary rendering objects
		initGL()
		
		// Create a display link capable of being used with all active displays
		CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
		
		func displayLinkOutputCallback(displayLink: CVDisplayLink,
			_ inNow: UnsafePointer<CVTimeStamp>,
			_ inOutputTime: UnsafePointer<CVTimeStamp>,
			_ flagsIn: CVOptionFlags,
			_ flagsOut: UnsafeMutablePointer<CVOptionFlags>,
			_ displayLinkContext: UnsafeMutablePointer<Void>) -> CVReturn {
			unsafeBitCast(displayLinkContext, GLView.self).getFrameForTime(inOutputTime)
			
			return kCVReturnSuccess
		}
		
		// Set the renderer output callback function
		CVDisplayLinkSetOutputCallback(displayLink!, displayLinkOutputCallback, UnsafeMutablePointer<Void>(unsafeAddressOf(self)));
		
		// Set the display link for the current renderer
		let cglContext: CGLContextObj = openGLContext!.CGLContextObj;
		let cglPixelFormat: CGLPixelFormatObj = pixelFormat!.CGLPixelFormatObj;
		CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink!, cglContext, cglPixelFormat);
		
		// Activate the display link
		CVDisplayLinkStart(displayLink!);
		
		// Register to be notified when the window closes so we can stop the displaylink
		NSNotificationCenter.defaultCenter().addObserver(self,
			selector: "windowWillClose:",
			name: NSWindowWillCloseNotification,
			object: self.window)
	}
	
	
	func initGL() {
		// The reshape function may have changed the thread to which our OpenGL
		// context is attached before prepareOpenGL and initGL are called.  So call
		// makeCurrentContext to ensure that our OpenGL context current to this
		// thread (i.e. makeCurrentContext directs all OpenGL calls on this thread
		// to [self openGLContext])
		openGLContext!.makeCurrentContext()
		
		// Synchronize buffer swaps with vertical refresh rate
		var swapInt: GLint = 1
		openGLContext!.setValues(&swapInt, forParameter:NSOpenGLContextParameter.GLCPSwapInterval)
		
		// Init our renderer.  Use 0 for the defaultFBO which is appropriate for
		// OSX (but not iOS since iOS apps must create their own FBO)
//		_renderer = [[OpenGLRenderer alloc] initWithDefaultFBO:0];
		
		glboyWrapper = GLBoyWrapper()
		glboyWrapper!.initGLBoyWidth(Int32(bounds.size.width), height: Int32(bounds.size.height))
		glboyWrapper!.setup()
		
	}
	
	
	func getFrameForTime(outputTime: UnsafePointer<CVTimeStamp>) -> CVReturn {
//		NSLog("width: %f, height: %f", window!.frame.origin.x, window!.frame.origin.y)
		autoreleasepool {
			drawView()
		}
		return kCVReturnSuccess;
	}
	
	
	func drawView() {
		openGLContext!.makeCurrentContext()
		
		// We draw on a secondary thread through the display link
		// When resizing the view, -reshape is called automatically on the main
		// thread. Add a mutex around to avoid the threads accessing the context
		// simultaneously when resizing
		CGLLockContext(openGLContext!.CGLContextObj);
		
		glboyWrapper!.render()
		
		CGLFlushDrawable(openGLContext!.CGLContextObj)
		CGLUnlockContext(openGLContext!.CGLContextObj)
	}
	
	
	func windowWillClose(notificaiton: NSNotification) {
	// Stop the display link when the window is closing because default
	// OpenGL render buffers will be destroyed.  If display link continues to
	// fire without renderbuffers, OpenGL draw calls will set errors.
	
		CVDisplayLinkStop(displayLink!);
	}
	
	
	deinit {
		// Stop the display link BEFORE releasing anything in the view
		// otherwise the display link thread may call into the view and crash
		// when it encounters something that has been release
		CVDisplayLinkStop(displayLink!);
//		CVDisplayLinkRelease(displayLink!);
	}
	
	
	override func reshape()
	{
		super.reshape()
	
		// We draw on a secondary thread through the display link. However, when
		// resizing the view, -drawRect is called on the main thread.
		// Add a mutex around to avoid the threads accessing the context
		// simultaneously when resizing.
		CGLLockContext(openGLContext!.CGLContextObj)
		glboyWrapper!.sizeWidth(Int32(bounds.size.width), height: Int32(bounds.size.height))
		CGLUnlockContext(openGLContext!.CGLContextObj)
	}
	
	
	override func drawRect(theRect: NSRect)
	{
		// Called during resize operations
		
		// Avoid flickering during resize by drawiing
		drawView()
	}



	//https://www.ekreative.com/blog/using-c-code-and-libraries-in-applications-written-in-swift

	
}