#include<iostream>

/*
	아주 설계가 잘 된 시스템을 보면 객체를 복사하는 함수가 딱 두 개만 있다.
	두 가지는 바로 복사 생성자와 복사 대입 연산자이다.(둘이 합쳐 복사 함수)
	일단 우리가 이 것들을 만들어 주지 않아도 컴파일러가 만들어 주기 마련인데, 상황에 따라서는
	우리가 직접 만들어야 하는 경우도 있다. 하지만 문제는 우리가 의도한대로 만들어 지지 않더라도
	에러를 뱉지 않는 다는 점이다. 아래 예시를 보자. 고객을 나타내는 클래스가 있고,
	개발자가 직접 구현한 복사 함수가 있다. 복사 함수을 호출할 때 마다 로그를 남기도록 작성.
*/

void LogCall(const std::string& FuncName);	// 로그 기록을 만듦

class Customer
{
public:
	//...
	Customer(const Customer& rhs);
	Customer& operator=(const Customer& rhs);
	//...
private:
	std::string Name;
};

Customer::Customer(const Customer& rhs) : Name(rhs.Name) //rhs의 데이터를 복사
{
	LogCall("Customer copy constructor");
}

Customer& Customer::operator=(const Customer& rhs)
{
	LogCall("Customer copy assignement operator");
	Name = rhs.Name;		//rhs의 데이터를 복사
	return *this;
}
//여기까진 문제 없어 보인다.
class Date {/*...*/ };	// 날짜 정보를 위한 클래스

class Customer2
{
public:
	//...			//위와 동일
private:
	std::string name;
	Date LastTransaction;		//데이터 멤버가 추가
};

/*
	이렇게 되고 나면,(데이터 멤버가 추가되면서) 복사 함수의 동작은 완전 복사가 아니라 부분 복사가 된다. 고객의 Name은 복사하지만
	LastTransaction은 복사하지x, 여기서 주의 해야할 점이 나오는데, 이 상황에 대해 컴파일러가 말해주지 않는다.
	따라서 데이터 멤버가 추가 됐다면, 우리가 해야 할 일은
	1. 복사 함수를 다시 작성
	2. 생성자 갱신
	3. 비표준형operator=함수도 모두 바꿔줘야함.

	아래는 이러한 문제에서 가장 프로그래머를 사악하게 괴롭히는 경우의 예다 (클래스 상속)
*/

class PriorityCustomer : public Customer2
{
public:
	PriorityCustomer(const PriorityCustomer& rhs);
	PriorityCustomer& operator=(const PriorityCustomer& rhs);
	//...

private:
	int iPriority;
};

PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs) : iPriority(rhs.iPriority)
{
	LogCall("PriorityCustomer copy construction");
}
PriorityCustomer& PriorityCustomer::operator=(const PriorityCustomer& rhs)
{
	LogCall("PriorityCustomer copy assignment operator");
	iPriority = rhs.iPriority;
	return *this;
}

/*
	PriorityCustomer 클래스의 복사함수는 언뜻 보기엔 PriorityCustomer의 모든 것을 복사하고 있는 것처럼 보이지만,
	Customer로부터 상속한 데이터 멤버들의 사본도 PriorityCustomer클래스에 들어가 있는데 이들은 복사가 안되고 있다.
	PriorityCustomer의 복사 생성자에는 기본 클래스 생성자에 넘길 인자들도 명시되어 있지 않아서 Customer의 부분은
	기본 생성자에 의해 초기화된다. 당연히 이 생성자는 Name, LastTransaction에 대해 '기본적인' 초기화를 해줄것.

	심지어 PriorityCustomer의 복사 대입 연산자는 기본 클래스 부분을 건드리지도 않는다.
	어쨌든 만약 복사함수를 직접 만들게 된다면 기본 클래스 부분을 빠뜨리지 않도록 각별히 주의해야 한다.
	물론 기본 클래스 부분은 private 멤버일 가능성이 높기 때문에 직접 건드리긴 어렵고, 파생 클래스의 복사 함수 안에서
	기본 클래스의 복사 함수를 호출하도록 만들면 된다.
*/

PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs) : Customer2(rhs), iPriority(rhs.iPriority)
{		//초기화 리스트를 통해 기본 클래스의 복사 생성자 호출.
	LogCall("PriorityCustomer copy construction");
}
PriorityCustomer& PriorityCustomer::operator=(const PriorityCustomer& rhs)
{
	LogCall("PriorityCustomer copy assignment operator");
	Customer2::operator=(rhs);		//기본 클래스 부분을 대입
	iPriority = rhs.iPriority;
	return *this;
}

/*
	추가로 복사함수에서 한쪽이 다른 한쪽을 호출하는 방식은 절대로 피할것. 만약 코드의 중복이 있어서
	이를 피하고 싶다면 중복되는 부분을 함수로 따로 만들어서 각각 이를 호출하도록 할것.
	이런 용도의 함수는 private 접근 지정인 경우가 많고, init어쩌구 하는 이름을 많이 가짐.
*/