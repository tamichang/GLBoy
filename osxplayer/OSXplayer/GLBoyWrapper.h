#import <Foundation/Foundation.h>

@interface GLBoyWrapper : NSObject
{
	void* _cppObj;    // C++クラスのインスタンスを参照するポインタ
}

-(void)test;    // このメソッドをSwiftから呼べて、内部でC++コードが動けばOK、とする

@end