#import "GLBoyWrapper.h"

#include <vector>
#include <iostream>

class MyCppClass
{
public:
	MyCppClass& add(int i) {vct.push_back(i); return *this;}
	void trace() {std::for_each(vct.begin(), vct.end(), [](int i){std::cout << i << std::endl;});}
	
private:
	std::vector<int> vct;
};

@implementation GLBoyWrapper

-(id)init {
	self = [super init];
	
	if (self) {
		_cppObj = new MyCppClass();
		MyCppClass& obj = *static_cast<MyCppClass*>(_cppObj);
		obj.add(10).add(20).add(30);
	}
	
	return self;
}

-(void)dealloc {
	delete static_cast<MyCppClass*>(_cppObj);
}

-(void)test {
	MyCppClass& obj = *static_cast<MyCppClass*>(_cppObj);
	obj.trace();
}

@end