#include<iostream>

int main()
{
	std::string* StringArray = new std::string[100];
	//...
	delete StringArray;
}
/*
위 코드는 미정의 동작을 보일 수 밖에 없게 된다. 100개의 string 객체들 가운데 99개는 정상적인 소멸 과정을 거치지
못할 가능성이 큼.

만약 new 연산자를 사용하게 된다면 두 가지의 내부 동작이 진행된다.
1. 일단 메모리가 할당이 된다(이때 operator new 함수가 쓰임.)
2. 할당된 메모리에 대해 한 개 이상의 생성자가 호출이됨.

delete 연산자를 사용할 때도 두 가지의 내부 동작이 진행되는데,
1. 우선 기존에 할당된 메모리에 대해 한 개 이상의 소멸자가 호출되고
2. 메모리가 해제됨(이때 operator delete 함수가 쓰임.)

이 때, delete 연산자가 적용되느 객체의 수는 "소멸자가 호출되는 수"만큼이다.
삭제되는 포인터는 객체 하나만 가리키는지 객체의 배열을 가리키는지 생각해보자.

단일 객체의 메모리 구조는 객체 배열에 대한 메모리 배치구조와 다르다.
특히, 배열을 위해 만들어지는 힙 메모리에는 대개 배열원소의 개수가 박혀 들어간다는 점이 결정적인데,
이 때문에 delete 연산자는 소멸자가 몇 번 호출될지 쉽게 알 수 있다.

반면, 단일 객체용 힙 메모리는 이런 정보가 없다.

한 개의 객체     :		object
객체의 배열		:		n	object	object	object...

어떤 포인터에 대해 delete를 적용할 때, delete연산자에게 "배열 크기 정보가 있다" 라는 사실을 알려줄 수 있는 것은
프로그래머에게 있다.
*/

std::string* StringPtr1 = new std::string;
std::string* StringPtr2 = new std::string;
//...
delete StringPtr1;		// 객체 한 개를 삭제합니다
delete[] StringPtr2;	// 객체의 배열을 삭제합니다.

/*
	동적 할당된 메모리에 대한 포인터를 멤버 데이터로 갖고 있는 클래스를 만든다면 이 사실을
	아주 조심해야 할 것.

	또한 typedef를 많이 쓴다면 또 알아둬야 한다.
	typedef로 정의된 어떤 타입의 객체를 메모리에 생성하려고 new를 썼을 때, 나중에 어떤 형태의 delete를 적어줘야 하는가에
	대한 언급을 달아주는 책임이 있기 때문이다.
*/

typedef std::string AddressLines[4];	//주소는 네줄로 되어있고, 각각은 string
std::string* pal = new AddressLines;

delete pal;		// 미정의 동작행
delete[] pal	// 올바른 마무리

/*
	가장 좋은 방법은 배열을 typedef로 하지 않는 것.
	vector 같이 좋은 것이 있으니 가급적 vector<string> 같은 것을 쓰자.
*/