#include<iostream>

/*
	기본적으로 C++도 함수로부터 객체를 전달받거나 함수에 객체를 전달할 때 call by value를 사용한다.
	특별히 다른 방식을 지정하지 않는다면 함수의 매개변수는 실제 인자의 '사본'을 통해 초기화되며, 어떤 함수를
	호출한 쪽은 그 함수가 반환한 값의 사본을 돌려받는다. 아들 사본을 만들어내는 원천이 바로 복사 생성자인데,
	이 점 때문에 call by value가 고비용의 연산이 되기도 한다. 아래 예시를 보자
*/

class CPerson
{
public:
	CPerson();
	virtual ~CPerson();

private:
	std::string m_sName;
	std::string m_sAddress;
};

class CStudent : public CPerson
{
public:
	CStudent();
	~CStudent();		//자동으로 가상 소멸자
	//...
private:
	std::string m_sSchoolName;
	std::string m_sSchoolAddress;
};

//다음 함수는 CStudent인자를 전달 받고(값으로) 이 인자가 유효화 됐는가를 알려 주는 값을 반환

bool ValidateStudent(CStudent s);
CStudent Plato;
bool PlatoIsOK = ValidateStudent(Plato);

/*
	위 함수가 호출되면 어떤 과정이 이루어지는지 생각해보자.

	1. Plato로부터 매개변수s를 초기화 시키기 위해서 CStudent의 복사 생성자가 호출될 것
	2. s는 함수가 끝날 떄 소멸될 것.				-> 1,2로 전달비용은 복사생성자 1번 소멸자 호출 한 번
	3. CStudent객체는 string 객체 두 개가 멤버로 들어가기 때문에 같이 생성되어야 함.
	4. CStudent객체는 CPerson 객체로부터 파생되었기 때문에 CPeron 객체도 같이 생성되어야함.
	5. 또 다시 CPerson객체는 string 객체 두 개가 멤버로 들어 있기 때문에 각각의 생성자 호출
	최종적으로 CStudent 객체 하나만 값으로 전달했을 뿐인데 복사 생성자 호출만 6번, 소멸자 6번이 일어남

	이를 해결하기 위한 방법이 바로 "상수 객체 참조자에 의한 전달"이다.
	위 코드를 아래와 같이 바꿔보자
*/

bool ValidateStudent(const CStudent& s);

/*
	새로 만들어지는 객체 같은 것이 없기 때문에 생성자와 소멸자가 전혀 호출되지 않으며, const 키워드 덕분에
	원본이 전달되어도 변화로부터 안전하다!

	또한 참조에 의한 전달 방식으로 매개변수를 넘기면 복사손실 문제(Slicing Problem)가 없어지는 장점도 있다.
	파생 클래스 객체가 기본 클래스 객체로서 전달되는 경우가 드물지 않게 나오는데,
	이때 객첵가 값으로 전달되면 기본 크랠스의 복사 생성자가 호출되고, 파생 클래스 객체로 동작하게 해 주는
	특징들이 잘려나가게 되는데 이것을 방지해주는 것!
	아래 예시를 보자
*/

class CWindow
{
public:
	//...
	std::string Name() const;		//윈도우의 이름 반환
	virtual void Display() const;	//윈도우 테두리 및 내부를 그림.
};

class CWindowWithScrollBars : public CWindow
{
public:
	//...
	virtual void Display() const;
};

/*
	이 CWindow 클래스로 만들어지는 객체는 이름을 갖고 있으며, 이 이름은 Name멤버 함술로 얻어낸다.
	화면 표시도 가능한데, Display멤버함수를 호출하면 윈도우를 화면에 뿌릴 수 있다.
	그리고 Display가 가상 함수로 선언되어 있기 때문에 파생 클래스에서 더 나은 모습으로 화면 표시를 할 수 있을 것

	이제 이것을 가지고 어떤 윈도우의 이름을 출력하고 그 윈도우를 화면에 표시하는 함수를 하나 만들어보자
*/

void PrintNameAndDisplay(CWindow w)	//매개변수가 복사 손실에 당해버림!
{
	std::cout << w.Name();
	w.Display();
}
/*
	이 함수에 파생클래스인 CWindowWithScrollBars의 객페를 넘긴다면 어떤 일이 생기는지 생각해보자
	매개변수 w가 생성이 되기는 하는데, CWindowWithScrollBars객체의 구실을 할 수 있는 정보가 전부 잘려 나간다.
	함수 안에서 w는 어떤 타입의 객체가 넘겨지든 CWindow 클래스 객체의 면모만을 갖게 될 것.

	특히 함수 안에서 호출되는 Display는 무조건 CWindow의 Display다.
	이 문제를 해결하기 위해서 위에서 말했듯 w를 상수 객체에 대한 참조자로 전달하면 된다
	void PrintNameAndDisplay(const CWindow& w)

	이렇게 해서 w는 어떤 종류의 윈도우가 넘겨지더라도 그 윈도우의 성질을 잘 유지하게된다.

	참조자는 기본적으로 포인터를 써서 구현되기 때문에 객체의 타입이 기본 제공 타입일 경우에는
	그냥 값으로 넘기는 편이 더 효율적일 때가 많다.
	즉 "값에 의한 전달"과 "상수 객체의 참조에 의한 전달"중 하나를 골라야 한다면
	기본 제공 타입의 경우 값에 의한 전달을 하더라도 잘못된 것이 아니라는 것.

	이 점은 STL의 반복자와 함수 객체에도 마찬가지임! 기본적으로 이들은 값에 의한 전달이 되도록 만들어졌기 때문에!
	참고로, 반복자와 함수 객체를 구현할 때에는
	1. 복사 효율을 높일 것
	2. 복사 손실 문제에 노출되지 않도록 할 것.

	기본제공 타입은 크기가 작은데, 이점에 착안해 크기가 작은 타입은 모두 값에 의한 전달이 낫다고 생각하면 안된다!
	크기가 작더라도, 복사 생성자도 값이 싸더라도 컴파일러가 기본제공 타입과 사용자 정의 타입을 다르게
	취급하기도 하기 때문에 수행 성능에서 문제가 생길 수 있다는 점을 알아두어야 한다.

	추가적으로 사용자 정의 타입은 항상 변화에 노출되어 있기 때문에 언제 커질지 모르는 법이다!

	정리하자면 값에 의한 전달이 괜찮은 타입은
	1. 기본제공 타입
	2. STL반복자
	3. 함수 객체 타입

	이렇게 세 가지뿐이다!
*/