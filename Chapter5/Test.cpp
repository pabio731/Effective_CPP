#include"pch.h"

class Base1
{
private:
	int iTest1;
	int iTest2;

public:
	Base1();
};

Base1::Base1()
	:iTest1(0), iTest2(0) {}

class Derived2 : public Base1
{
private:
	int iD1;
	int iD2;

public:
	Derived2();
};

Derived2::Derived2()
	:iD1(0), iD2(0) {}

int main()
{
	Base1* pB;
	Derived2* pD;
	Derived2 d;

	pB = &d;
	pD = &d;

	std::cout << "pB : " << pB << std::endl;
	std::cout << "pD : " << pD << std::endl;
}