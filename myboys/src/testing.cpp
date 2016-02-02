
#include "testing.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
//#include <cstdlib>
//#include <time.h>

class Aaa {
	int id;
public:
	Aaa(int _id) {
		id = _id;
		std::cout << "Aaa:" << id << std::endl;
	}
	~Aaa() {
		std::cout << "~Aaa:" << id << std::endl;
	}
};

class Bbb :public Aaa {
	int id;
public:
	Bbb(int _id) : Aaa(_id) {
		id = _id;
		std::cout << "Bbb:" << id << std::endl;
	}
	~Bbb() {
		std::cout << "~Bbb:" << id << std::endl;
	}
};




int main()
{
//	std::shared_ptr<Aaa> aaa1 = std::make_shared<Aaa>(1);
//	aaa1 = std::make_shared<Aaa>(2);
//	std::unique_ptr<Bbb> bbb(new Bbb(1));
//	aaa1 = std::unique_ptr<Aaa>(new Aaa(2));
	
	std::srand (std::time(NULL));
//	std::cout << RAND_MAX << std::endl;
	for (int i = 0; i < 1000; i++) {
		std::cout << (float) std::rand() / RAND_MAX << std::endl;
	}
	
	
	std::cout << "main end" << std::endl;
}