#include"pch.h"

/*
	C++은 인터페이스와 구현을 깔끔하게 분리하는 일에 별로 일가견이 없다. C++ 클래스 정의는 클래스 인터페이스만
	지정하는 것이 아니라 구현 세부사항까지 상당히 많이 지정하고 있기 때문.
*/

//class Date {};
//class Address {};

class Person
{
public:
	Person(const string& strNam, const Date& Birthday, const Address& Addr);
	string Name() const();
	string Birthday() const();
	string Address_() const;

private:
	string strTheName;		//구현 세부 사항
	Date tTheBirthDate;		// 구현 세부 사항
	Address tTheAddress;	// 구현 세부 사항
};

/*
	위 코드만 가지고 Person 클래스가 컴파일 될 수 있는가?
	string, Date, Address가 어떻게 정의됐는지를 모르면 컴파일 자체가 불가능 하다.
	결국 이들이 정의된 정보를 가져와야 하고, 이때 쓰는 것이 #include 지시자임.
	따라서 아마 Person 클래스를 정의하는 파일을 보면 아마 아래의 코드가 있을 것이다.
	#include <string>
	#include <Date.h>
	#include <Address.h>

	하지만 문제는 이것들이 골칫덩이들이라는 것이다. #include문은 Person을 정의한 파일과 위의 헤더 파일들 사이에
	컴파일 의존성이란 것을 엮어버린다. 그러면 위 헤더 셋 중 하나라도 바뀌는 것은 물론, 이들과 엮인 다른 헤더 파일이
	바뀌기만 해도, Person 클래스를 정의한 파일은 컴파일러에가 끌려가게 되어 있다. 아마 또 Person을 사용하는
	다른 파일들도 끌려가게 될 것은 말할 것도 없다.

	그럼 Person 클래스를 정의할 때, 구현 세부 사항을 따로 떼어서 지정하는 식으로 하면 안되는가?
*/

namespace std
{
	class string;		// 전방선언, 틀린 것임.
}

class Date;
class Address;

class Person2
{
public:
	Person(const std::string& strName, const Date& Birthday, const Address& Addr);
	std::string Name() const;
	std::string Birthdate() const;
	std::string Address() const;
};
/*
	이렇게 놓고 보면 Person 클래스의 인터페이스만 바뀌었을 때만 컴파일을 다시 하면 되니 좋아보인다.
	아이디어는 좋지만 되지 않는 것이 문제이다.
	1. string은 사실 클래스가 아니라 basic_string<char>을 typedef한 것. 그러니 string 에 대한 전방 선언이 맞을 리 없다.
	   제대로 전방 선언을 하려면 템플릿을 추가로 끌어와야하기 때문에 더 복잡하다. 사실 표준 라이브러리 헤더는
	   어지간하면 병목요인이 되지 않으니 그냥 헤더를 추가시키면 된다.
	   특히 pch를 쓴다면 더더욱 그렇다. 만약 표준 헤더가 문제가 된다면 인터페이스 설계를 고치는 방법밖에 없다.

	2. 컴파일러가 컴파일 도중에 객체들의 크기를 전부 알아야 한다는 데 있다.
*/
int main()
{
	int iTmp;					// int 하나를 정의
	Person2 p(/*params*/)		// Person 하나를 정의
}
/*
	여기서 문제는 컴파일러가 int 를 담을 공간을 만드는 데에는 어느정도가 필요한지 알지만, p 를 담을 공간이
	얼마나 필요한지 알지 못한다는 것이다. 다른 언어의 경우 고민이 안된다. 이들 언어는 객체가
	정의될 때 컴파일러가 그 객체의 포인터를 담을 공간만 할당하기 때문이다.
*/
int main()
{
	int iSrc;
	Person2 P();		//Person 하나에 대한 포인터 하나를 정의
}
/*
	위와 같이 된다는 것!
	그럼 C++에서는? 당연히 적법하다. 포인터 뒤에 실제 객체 구현부 숨기기 놀이를 C++에서도 할 수 있다.
	우선 주어진 클래스를 두 클래스로 쪼갠다. 한쪽은 인터페이스만 제공하고, 한쪽은 인터페이스의 구현을 맡도록 만드는 것.
	구현을 맡은 클래스의 이름이 PersonImp3 이라고 하면 Person3클래스는 다음과 같이 정의할 수 있다.
*/

#include<memory>	//tr1::shared_ptr을 위한 헤더

class PersonImp3;		//Person3의 구현 클래스에 대한 전반 선언

class Date;
class Address;			//Person안에서 사용하는 것들에 대한 전방 선언

class Person3
{
public:
	Person3(const string& strName, const Date& Birthday, const Address& Addr);
	string Name() const;
	string BirthDate() const;
	string Address() const;

private:
	tr1::shared_ptr<PersonImp3> pImpl;		//구현 클래스 객체에 대한 포인터
};

/*
	위 코드를 보면 주 클래스(Person3)에 들어 있는 데이터 멤버라고는 구현 클래스(PersonImp3)에 대한 스마트 포인터뿐이다.
	이런 설계는 거의 패턴으로 굳어져 있을 정도여서, Pimpl 관용구(Pointer to Implementation)라는 이름도 있는데,
	이때 포인터의 이름은 대개 pImpl이라고 붙이는 것이 일반적이다.

	이렇게 설계해두면 Person의 사용자는 생일, 주소 이름 등의 자질구레한 세부사항과 완전히 갈라서게 된다.
	Person클래스에 대한 구현 부분은 마음대로 고칠 수 있는데, Person의 사용자 쪽에서는 컴파일을 다시 할 필요가 없는 것.
	이렇게 인터페이스와 구현을 둘로 나누는 열쇠는 "정의부에 대한 의존성"을 "선언부에 대한 의존성"으로
	바꾸어 놓는데 있음. 이것이 컴파일 의존성을 최소화 하는 핵심 원리임.
	즉 헤더 파일을 만들 때는 실용적으로 의미를 갖는 한 자체 조달 형태로 만들고, 정 안되면 정의부가 아닌
	선언부에 의존성을 갖도록 해야한다. 이외의 나머지 전략들고 이 것을 축으로 해서 흘러가게 되어 있음.

	1. 객체 잠조자 및 포인터로 충분한 경우에는 객체를 직접 쓰지 않는다.
	   어떤 타입의 참조자 및 포인터를 정의할 때는 그 타입의 선언부만 필요함.
	   반면 객체를 정의할 때는 그 타입의 정의가 필요해진다.

	2. 할 수 있으면 클래스 정의 대신 클래스 선언에 최대한 의존하도록 만든다.
	   어떤 클래스를 사용하는 함수를 선언할 때는 그 클래스의 정의를 가져오지 않아도 된다.
	   심지어 그 클래스 객체를 값으로 전달하거나, 반환하더라도 정의가 필요없다.

	3. 선언부와 정의부에 대해 별도의 헤더파일을 제공한다.
	   "클래스를 둘로 쪼개자"라는 지침을 제대로 쓸 수 있도록 하려면 헤더 파일이 짝으로 있어야 한다.
	   하나는 선언부를 위한 헤더 파일이고, 또 하나는 정의부를 위한 헤더 파일
	   당연히 이 두개는 짝으로 관리를 해야한다. 한쪽에서 어떤 선언이 바뀌면
	   다른 쪽도 똑같이 바꿔야 한다는 것.

	   Date의 사용자가 Today함수와 ClearAppointments 함수를 선언하고 싶다고 해서,
	   Date를 직접 전방선언하면 안된다는 것. Date 클래스에 대한 선언부 헤더를
	   #include해야한다.

	표준 C++라이브러리의 <iosfwd>헤더는 C++에서 지원하는 iostream관련 함수와 클래스의
	선언부만으로 구성 된 헤더이다. 각각의 정의부는 일정한 분류에 따라 여러개의 헤더로 나눠져 있는데,
	<sstream>, <streambuf>,<fstream>,<iostream> 등이 바로 그것.

	iosfwd에 관해 잘 알아두는 것을 통해 위의 내용이 템플릿이 아닌 함수 뿐만 아니라
	템플릿에도 잘 들어 맞는 다는 사실을 확인하기 딱 좋다는 점을 확인할 수 있다.
	5-5에서 대다수의 빌드 환경에서 템플릿 정의가 대개 헤더 파일에 있다고 했지만,
	몇몇 빌드 환경에서는 헤더가 아닌 파일에 템플릿 정의가 잇는 것도 사실임.
	그러니 선언부 전용 헤더를 제공하자는 이야기의 대표적인 헤더가 <iosfwd>인셈.

	사실 C++에서는 템플릿 선언과 템플릿 정의를 분리할 수 있도록 하는 기능을 export라는 키워드로 제공하고 있다.
	하지만 현장에서 export를 쓰는지는 의문...?

	위에서 본 pImpl 관용구를 사용하는 Person 같은 클래스를 가리켜 핸들 클래스(handle class)라고 함.
	이번 항목의 person 클래스가 아닌 추가적인 응용을 보도록 하자. 핸들 클래스에서 어떤
	함수를 호출하게 되어 있다면, 핸들 클래스에 대응되는 구현 클래스 쪽으로 그 함수 호출을
	전달해서 구현 클래스가 실제 작업을 수행하게 만드는 것.
*/

#include "Person.h"			//Person 클래스를 구현하는 중이기 때문에 이 Person 클래스 정의를 include해야함
#include "PersonImp3.h"		//이와 동시에 PersonImpl의 클래스 정의도 Include해야 하는데,
//이렇게 하지 않으면 멤버 함수를 호출할 수 없다.

Person::Person(const std::string& strName, const Date& Birthday, const Address& Addr)
	:pImpl(new PersonImp3(Name, Birthday, Addr)) {}
std::string Person::Name() const { return pImpl->Name(); }
/*
	Person 생성자가 어떻게 PersonImpl 생성자를 호출하는지,
	Person::Name이 PersonImp3::Name()을 어떤 식으로 호출하는지를 볼 것.

	Person클래스는 핸들 클래스이지만, Person의 동작이 바뀐 것은 아님. 동작을 수행하는 방법이 달라진 것이지.
	핸들 클래스 방법 대신 다른 방법을 쓴다면 Person을 특수 형태의 추상 기본 클래스, 인터페이스 클래스로
	만드는 방법도 있다. 어떤 기능을 나타내는 인터페이스를 추상 기본 클래스를 통해 마련해 놓고,
	이 클래스로부터 파생 클래스를 만들 수 있게 하는 것.
	파생이 목적이기 때문에 이 클래스에는 데이터 멤버도, 생성자도 없고, 하나의 가상 소멸자와 인터페이스를 구성하는
	순수 가상 함수만 들어있다.

	C++은 자바 및 닷넷이 인터페이스에 대해 제약을 가하는 것처럼 인터페이스 클래스에 제약을 가하지 않는다.
	자바나 닷넷의 인터페이스는 언어 차원에서 데이터 멤버나 함수 구현을 아예 가질 수 없지만, C++에는 제약이 없다.

	이점이 유용하게 쓰이기도 하는데, 비가상 함수의 구현은 주어진 클래스 계통 내의 모든 클래스에 대해 똑같아야 한다.
	따라서 비가상 함수는 인터페이스 클래스의 일부로서 구현해 두는 편이 이치에 딱 맞는 것.
*/

class Person
{
public:
	virtual ~Person();

	virtual	std::string Name() const = 0;
	virtual std::string BirthDate() const = 0;
	virtual std::string Address() const = 0;
};
/*
	이 클래스를 코드에 써먹으려면 Person에 대한 포인터 혹은 참조자로 프로그래밍 하는 방법밖에 없다
	인터페이스 클래스를 사용하기 위해서는 객체 생성 수단이 최소한 하나는 있어야 한다. 대개
	이 문제는 파생 클래스의 생성자 역할을 대신하는 어떤 함수르 만들어 놓고, 이것을 호출해 해결하곤 한다.

	이런 함수를 가리켜 팩토리 함수 혹은 가상 생성자라고 부른다. 주어진 인터페이스 클래스의
	인터페이스를 지원하는 객체를 동적으로 할당한 후 그 객체의 포인터(가능하면 스마트 포인트)를 반환하는 것.
	항상은 아니지만, 이런 함수는 인터페이스 클래스 내부에 정적멤버로 선언되는 경우가 많다.
*/

class Person
{
public:
	//...
	static std::tr1::shared_ptr<Person> create(const std::string& Name, const Date& Birthday, const Address& Addr);
	// 주어진 매개변수로 초기화 되는 Person에 대한 객체를 새로 생성하고, 그것에 대한 tr1::shared_ptr을 반환함.
};

// 사용자쪽에서는
std::string Name;
Date DateOfBirth;
Address address;

//Persom 인터페이스를 지원하는 객체 한 개를 생성
std::tr1::shared_ptr<Person> pp(Person::create(Name, DateOfBirth, Address));

std::cout << pp->Name() << "was born on" << pp->BirthDate() << "and now lives at" << pp->address(); // Person인터페이스를 통해 그 객체를 사용
//스마트 포인터 사용으로, pp는 유효범위를 벗어날 때 자동으로 삭제됨.

/*
	해당 인터페이스 클래스의 인터페이스를 지원하는 구체 클래스가 어딘엔가 정의되어야 할 것이고, 정말로 실행되는
	생성자(구체 클래스의 생서자)가 호출되어야 하는 것은 당연하다. 실제로 이부분은 보이지 않는 가상 생서자의
	구현부를 갖고있는 파일 안에서 이루어진다. 예를들어Person 클래스로부터 파생된RealPerson이라는 구체 클래스가 있다면, 이 클래스는
	자신이 상속받는 가상 함수의 구현부를 제공하는 식으로 만들어졌을 것.
*/

class RealPerson :public Person
{
public:
	RealPerson(const std::string& Name, const Date& Birthday, const Address& Addr)
		:strTheName(Name), TheBirthDate(Birthday), TheAddress(Addr) {}
	virtual ~RealPerson();

	std::string Name() const;
	std::string BirthDate() const;
	std::string Address()const;

private:
	std::string strTheName;
	Date TheBirthDate;
	Address TheAddress;
};

// 남은 것은 Person::create함수
std::tr1::shared_ptr<Person>Person::create(const std::string& Name, const Date& Birthday, const Address& Addr)
{
	return std::tr1::shared_ptr<Person>(new RealPerson(Name, Birthday, Addr));
}

/*
	인터페이스 클래스를 구현하는 용도로 가장 많이 쓰이는 매커니즘이 두가지인데
	위의RealPerson같은 인터페이스 클래스로부터 인터페이스 명세를 물려받게 만든 후에 그 인터페이스에
	들어있는 함수(가상함수)를 구현하는 것.
	두 번째 방법은 다중 상속을 하는 것.

	결론적으로 핸들 클래스와 인터페이스 클래스를 구현부로부터 인터페이스를 뚝 떼어 놓아
	파일들 사이의 컴파일 의존성을 완화 시켰는데 비용은?

	실행시간에 대한 비용과 객체 한개당 필요한 저장 공간이 추가로 늘어난다.

	핸들클래스의 멤버함수를 호출하면 알맹이 객체의 데이터까지 가기 위해 포인터를 타야하니
	간접화 연산이 한 단계 더 증가하고, 객체 하나씩을 저장하는데 필요한 메모리 크기에 구현부 포인터의
	크기도 추가된다. 그리고 구현부 포인터가 동적 할당된 구현부 객체를 가리키도록 어디선가 그 구현부 포인터의
	초기화가 일어나야 한다(핸들 클래스의 생성자 안에서)  결국 동적할단 연산에 따른 오버헤드, bad_alloc 예외와 부딪힐 수도 있다.

	인터페이스 클래스는 함수가 전부 가상함수라 가상 테이블에 들어가는 비용이 든다. 파생된 객체들도 모두 가상함수 테이블을
	갖게 되고, 이는 메모리 크기를 늘리는 요인이 된다.
	그리고 인라인 함수를 쓸 수 없다

	그럼에도 불구하고 미래를 대비한다는 측면에서 핸들 클래스 혹은 인터페이스 클래스를 사용하는 것이 좋다.
*/