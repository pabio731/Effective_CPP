#include"pch.h"

/*
	인라인 함수에는 기본적으로 매크로보다 훨씬 안전하고 쓰기 좋으며 함수 호출시 발생하는 오버해드에 대한
	걱정도 없다. 그런데 그 외의 숨겨진 이점이 한가지 더 있는데, 바로 최적화이다.

	대체적으로 컴파일러 최적화는 함수 호출이 없는 코드가 연속적으로 이어지는 구간에 적용되도록 설계 되기 때문에
	인라인 함수를 사용하면 컴파일러가 함수 본문에 대해 문맥별 최적화를 걸기가 용이해진다.

	실제로 대부분의 컴파일러는 '아웃라인 함수 호출(일반적인 함수 호출)'에 대해 최적화를 적용하지 않는다.
	하지만 이 역시 공짜는 아니다. 인라인 함수는 함수의 호출부를 함수의 본문으로 바꿔치기 하는 것이니
	목적 코드의 크기가 커질 것이 뻔하다. 따라서 메모리가 제한된 컴퓨터에서 인라인을 남발하면
	그 기계에서 쓸 수 있는 공간을 넘어버릴 수도 있다. 가상 메모리일지라도 코드가
	부풀려지면 성능의 걸림돌이 될 수 있다.

	반대의 경우도 있는데, 본문의 길이가 굉장히 짧은 인라인 함수를 이용하면, 함수 본문에 대해
	만들어지는 코드의 크기가 함수 호출문에 대해 만들어지는 코드보다 작아질 수 있다.
	이런 경우에는 목적코드의 크기가 작아지고, 명령어 캐시의 적중률도 높아질것

	아주 기본적으로, inline은 컴파일러에게 "요청"하는 것이지 "명령"하는 것이 아니다.
	inline을 붙이지 않더라도 자동으로 되는 경우도 있거, 명시적으로 할 수도 있다.
*/

class Person
{
public:
	//...
	int iAge() const { return iTheAge; }	//암시적 인라인 요청
private:
	int iTheAge;
};

/*
	이런 함수는 대게 멤버 함수이지만, 프렌드 함수도 클래스 내부에서 정의될 수도 있다.
	아무튼 이 경우 암시적으로 인라인 함수로 선언된다.

	명시적 방법은 inline 키워드를 붙이는 것.
	다음은 표준 라이브러리 mx 템플릿의 구현이다
*/
template <typename T>
inline const T& std::max(const T& a, const T& b) { return a < b ? b : a; }	// 명시적인 inline요청

/*
	기본적으로 인라인 함수는 대체로 헤더 파일에 들어있어야 하는 게 맞다. 왜냐하면 대부분
	빌드 환경에서 인라인을 컴파일 도중에 수행하기 때문이다.
	인라인 함수 호출을 그 함수의 본문으로 바꿔치기 하려면, 일단 컴파일러는 그 함수가 어떤 형태인지
	알고 있어야 함.

	템플릿 역시 대체적으로 헤더 파일에 들어있어야 맞다. 템플릿이 사용되는 부분에서 해당 템플릿을
	인스턴스로 만드려면 그것이 어떻게 생겼는지를 컴파일러가 알아야 하기 때문에.

	그런데 템플릿 인스턴스화는 인라인과 전혀 관련이 없다. 어떤 템플릿을 만들고 있는데,
	해당 템플릿으로 만들어지는 모든 함수가 인라인 함수였으면 싶은 경우에 그 템플릿에
	inline을 선언하는 것일뿐. 그러니 모든 템플릿은 인라인으로 만들어야 한다는 것은 잘못된 생각이다.

	컴파일러는 자신이 보기에 복잡한 함수는 절대로 인라인 요청을 받아주지 않는다.
	가장 대표적인 것이 루프와 재귀함수이며, 가상함수는 함수 호출을 실행중에 결정하는 것인데,
	인라인은 프로그램 실행전에 호출된 함수를 넣는 것이니 애초에 성립이 안돼서 요청을 받아주지 않는다.

	또한 완벽하게 인라인의 조건을 만족해도 함수의 본문을 만드는 경우가 있는데,
	어떤 인라인 함수의 주소를 취하는 코드가 있다면 함수 본문을 만들 수 밖에 없다.
	있지도 않은 함수의 주소를 취할 수는 없기 때문에.
*/

inline void f() {}		//이 f의 호출은 컴파일러가 반드시 인라인 해준다고 가정하자.
void (*pf)() = f;		//pf는 f를 가리키는 함수 포인터.
f();					//이 호출은 인라인 될 것.
pf();					//이 호출은 인라인 되지 않을 것. 함수 포인터를 통해 호출되고 있기 때문에

/*
	함수 포인터가 아니더라도 인라인 되지 않는 인라인 함수가 괴롭게 할 수 있다.
	컴파일러가 ㅇ생성자와 소멸자에 대해 아웃라인 함수 본문을 만들 수도 있는 것.
	어떤 배열의 원소가 객체일 경우가 대표적인데, 해당 객체들을 생성, 소멸시
	생성자/소멸자의 함수 포인터를 얻어내려면 함수 본문이 필요하다.

	기본적으로 생성자와 소멸자는 인라인하기에 그리 좋지 않은 함수이다.
*/
class Base
{
public:

private:
	string bm1, bm2;	//Base 클래스의 멤버
};

class Derived : public Base
{
public:
	Derived() {}

private:
	string dm1, dm2, dm3;
};

/*
	C++은 객체의 생성과 소멸시 일어나는 일들에 대해서 여러가지 보장을 준비해 놓았다.
	예를들면 new를 쓰면 동적으로 만들어지는 객체를 생성자가 자동으로 초기화 해주는 것,
	delete를 쓰면 대응 되는 소멸자가 호출되는 것도 C++이 깔아둔 보장이다.

	다만 C++은 "무엇을" 해야할지는 정해두었지만 그것을 어떻게 해야할지는 정하지 않았다는 점.
	이 부분은 전적으로 컴파일러 구현자에게 달려있다.

	어쨌든 이런 일들이 스스로 어나는 것이 아니고, 우리 눈에 보이지 않지만 이런 일을 가능하게 만드는
	코드가 프로그램에 포함되어야 하고, 이 코드가 소스코드 어딘가에 들어가야 한다. 그리고 그것이
	소멸자와 생성자일 수도 있는것.
	다시 위 비어있는 것으로 보이는 Derived의 생성자를 다음과 같이 만들어질 것이라고 예상해볼 수 있다.
*/

Derived::Derived()
{
	Base::Base();							// Base부분 초기화
	try { dm1.string::basic_string(); }		// dm1의 생성 시도
	catch (/*...*/)							// 생성 도중 예외를 던지면
	{										//
		Base::~Base();						// 기본 클래스 부분을 소멸하고
		throw;								// 그 예외를 던짐
	}										//
	try { dm2.string::basic_string(); }		// dm2의 생성 시도
	catch (/*...*/)							// 생성도중 dm2에서 예외를 던지면
	{										//
		dm1.string::~basic_string();		// dm1을 소멸시키고
		Base::~Base();						// 기본 클래스를 소멸
		throw;								// 예외를 전파
	}										//
	try { dm3.string::basic_string(); }		// dm3의 생성시도
	catch (/*...*/)							// 생성도중 dm3에서 예외를 던지면
	{										//
		dm2.string::~basic_string();		// dm2를 소멸시키고
		dm1.string::~basic_string();		// dm1을 소멸시키고
		Base::~Base();						// 기본 클래스를 소멸
		throw;								// 예외를 전파
	}
}

/*
	실제로 컴파일러가 만드는 코드도 이렇다고 보기는 힘들다. 컴파일러가 예외처리를
	위와 같이 무식하게 하지는 않기 때문... 하지만 위 코드는 Derived의 텅빈 생성자가
	하는 일을 그대로 보여주는 것은 맞다.
	Derived 클래스의 생성자는 최소한 자신의 데이터 멤버와 기본 클래스 부분에 대해
	생성자를 호출해 주어야 하고, 이들 생성자를 호출해야 하기 때문에 인라인이 남감해진다...

	또 가장 중요한 점은 인라인 함수는 디버거가 가장 난감해 하는 것중 하나라는 것.
	그러니 인라인 함수를 만들고자 한다면 정말 단순함 함수에 한해서만
	인라인 함수로 선언하자. 디버깅이 중요한 위치에서는 디버거가 잘 작동하도록 만들어주고,
	정말 필요한 위치에서만 인라인 함수를 놓자.
*/