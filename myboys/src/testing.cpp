

#include "testing.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>

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
	std::unique_ptr<Bbb> bbb(new Bbb(1));
//	aaa1 = std::unique_ptr<Aaa>(new Aaa(2));
	std::cout << "main end" << std::endl;
}