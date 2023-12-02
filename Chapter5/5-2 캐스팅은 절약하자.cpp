#include"pch.h"

/*
	"C++은 어떤 일이 있어도 타입 에러가 생기지 않도록 보장한다" 라는 철학이 C++의 동작 규칙에 깔려있다.
	즉 일단 컴파일만 깔끔하게 끝난다면 그 이후엔 어떤 객체에 대해서도 불안전한 연산이나 말도 안 되느 연산을
	수행하려 들지 않는다는 것.

	하지만 이 타입 시스템을 가볍게 무시할 수 있는 것이 있는데, 바로 Cast이다.
	cast는 C++에서 굉장히 골칫거리기 때문에 각별히 신경써서 써야 하는 기능이다.
	먼저 캐스팅에 대한 문법을 정리해보자 똑같은 캐스팅이라도 쓰는 방법이 세 가지나 있다.

	// C스타일의 캐스팅
	(T) 표현식	:	표현식 부분을 T타입으로 캐스팅 합니다.
	T(표현식)	:	위와 동일, 함수 방식 캐스트

	어떻게 쓰든지 두 가지의 의미는 같다.
	위 두가지를 "구형 스타일의 캐스트"라고 함.

	C++은 독자적으로 네 가지의 새로운 형태의 캐스트 연산자를 제공한다.

	// C++의 신형 스타일의 캐스트
	const_cast<T>		(표현식)		:	객체의 상수성을 제거
	dynamic_cast<T>		(표현식)		:	안전한 다운 캐스팅을 제공. 주어진 객체가 어떤 클래스 상속계통에 속한 특정 타입인지 아닌지를 결정(런타임 비용이 높음)
	reinterpret_cast<T>	(표현식)		:	포인터를 int로 바꾸는 등의 하부 수준 캐스팅을 위해 만들어짐. 이식성이 없고 하부 수준에만 써야한다.
	static_cast<T>		(표현식)		:	암시적 변환(비상수 객체를 상수 객체로 바꾸거나 int를 double로 바꾸는등)을 강제로 진행. 타입 변환을 거꾸로 수행할 때 사용
										(void*를  일반 타입의 포인터로 바꾸거나 기본 클래스의 포인터를 파생 클래스의 포인터로 바꾸는 등)

	구형 스타일의 캐스트도 물론 적볍하게 쓰일 수 있지만, C++의 캐스트를 쓰는 것이 바람직하다.
	일단 알아보기 쉽기 때문에 소스 코드의 어디서 C++타입 시스템이 망가졌는지 찾아보는 작어이 편하고,
	캐스트를 사용한 목적을 더 좁혀서 지정하기 때문에 컴파일러가 에러를 진단할 수 있다.

	구형 스타일의 캐스트를 사용하는 예제를 보자.
	객체를 인자로 받는 함수에 객체를 넘기기 위해 명시 호출 생성자를 호출하고 싶은 경우이다.
*/

class Monster
{
public:
	explicit Monster(int size);
};
int main()
{
	void DoSomeWork(const Monster & m);

	DoSomeWork(Monster(15));					//함수 방식 캐스트 문법으로 int로부터 Monster를 생성

	DoSomeWork(static_cast<Monster>(15));		// C++방식 캐스트를 써서 int로부터 Monster를 생성
}
/*
	C스타일과 C++스타일의 캐스팅 모두 가능하다.
	하지만 그럴듯한 느낌이 나는 코드는 곧잘 코어 덤프를 잘 일으킨다는 점을 잊지말고
	가능하면 신형스타일의 캐스팅을 쓰자.

	캐스팅을 그냥 어떠한 타입을 다른 타입으로 처리하라고 컴파일러에게 알려 주는 것이라고
	생각한다면 크나큰 오산이다. 일단 타입 변환이 있다면, 이를 통해서 런타임에 실행되는 코드가
	만들어 지는 경우가 정말 많다. 다음 코드를 보자
*/

int iTmp, iDst;
//...
double dSrc = static_cast<double>(iTmp) / iDst;		//iTmp를 iDst로 나눈다. 이 때, 부동소수점 나눗셈을 사용한다.

/*
	int 타입의 iTmp를 double 타입으로 캐스팅한 부분에서 코드가 만들어진다. 그것도 항상.
	왜냐면 컴퓨터 아키텍쳐에서 int와 double의 표현구조가 아예 다르기 때문.
	추가적으로 아래 예시를 보자
*/

class Base {};
class Derived :public Base {};

Derived d;
Base* pb = &d;		//업캐스팅, Derived*->Base*의 암시적 변환이 이루어짐.
/*
	파생 클래스에 대한 기본 클래스 포인터를 만드는 지극히 흔한 코드이다.
	그런데, 두 포인터의 값이 다를 때도 가끔 있다는 사실을 알아두어야 한다.
	이런 경우가 되면, 포인터의 offset을 Derived*포인터에 적용하여 실제의 Base*포인터 값을
	구하는 동작이 바로 런타임에 이루어진다.

	객체 하나가 가질 수 있는 주소가 하나가 아니라 두 개가 될 수 있음을 항상 알아두어야 한다.
	(Derived* 포인터로 가리킬 때의 주소, Base* 포인터로 가리킬 때의 주소가 다른 상황)
	이 상황은 오로지 C++에서만 일어나는 특이한 일임. 자바, C, C# 모두 없는 일.
	즉 C++ 에서는 다중 상속에서는 이런 현상이 항상 생기지만, 심지어 단일 상속에서도 이런 일이 있어나니
	데이터가 어떤 식으로 메모리에 박혀 있을 거라는 섣부른 가정을 피해야 한다.
	즉 어떤 객체의 주소를 char* 포인터로 바꿔서 포인터 상술 연산을 적용하는 등의 코드는 미정의 동작을 낳을 수 있다는 것

	포인터 변위를 써야 하는 떄가 "가끔"인 이유는
	객체의 메모리 배치구조를 결정하는 방법과 객체의 주소를 계산하는 방법은 컴파일러마다 천차만별이기 때문임.
	즉 어떤 플랫폼에서 메모리 배치를 다 꿰고 있어서 캐스팅 했을 때 문제가 없었을지라도 다른 플랫폼에서 그게 또 통하지 않는다는것

	캐스팅이 들어가면 보기엔 맞는 것 같지만 실제로는 틀린 코드를 쓰고도 모르는 경우가 많아진다.
	주변에서 많이 쓰는 프레임 워크 하나를 살펴보면, 가상함수를 파생 클래스에서 재정의해서 구현할 때,
	기본 클래스 버전을 호출하는 문장을 가장 먼저 넣어달라는 요구 사항을 보게된다.
	어떤 프레임 워크에 Window기본 클래스가 있고, specialWindow의 파생 클래스가 있다고 가정하자.

	이들 클래스는 onResize라는 이름의 가상 함수를 모두 정의하고 있음.
	그리고 specialwindow의 onResize를 구현하려면 Window의 onResize를 호출해야 한다.
*/
class Window
{
public:
	virtual void onResize() {/*...*/ }	//기본 클래스 onResize 구현 결과
};

class SpecialWindow : public Window
{
public:
	virtual void onResize()
	{
		static_cast<Window>(*this).onResize();	//*this를 Window로 캐스팅하고, 그것에 대해 onResize를 호출... 동작이 안된다.
		//...
	}
	//...
};

/*
	위 코드를 보면 *this를 Window로 캐스팅 하는 코드이다.
	이에 따라 호출되는 onResize함수는 Window::onResizee가 된다.
	그런데 이제부터 어처구니 없는 일이 벌어지는데, 함수 호출이 이루어지는 객체가
	현재의 객체가 아니게 된다.
	이 코드에서는 캐스팅이 일어나면서 &this의 기본 클래스 부분에 대한 사본이 임시적으로 만들어지는데
	지금의 onResize는 바로 그 임시 객체에서 호출된 것이다. 이에 따라 결국 위 코드는 현재의 객체에 대해
	Window::onResize를 호출하지 않고 지나간다. 그리고 나서 SpecialWindow전용의 동작은 또 현재의 객체에 대해
	수행단다. 다시 말해 SpecialWindow만의 동작을 현재 객체에 대해 수행하기도 전에 기본 클래스 부분의 사본에
	Window::onResize를 호출하는 것. 만약 Window::onResize가 객체를 수정하도록 만들어졌기라도 하면
	현재 객체는 실제로 그 수정이 반영되지 않는 일이 생길 것이다.
	하지만 SpecialWindow::onResize에서 객체를 수정하면 진짜 현재 객체가 수정 될 것이 분명하고...
	분명 의도한대로 프로그램이 돌아가지 않을 것.

	이 경우 일단 캐스팅을 빼버리고 생각해야 한다. 범위 한정 연산자를 쓰자
*/

class SpecialWindow2 :public Window
{
public:
	virtual void onResize()
	{
		Window::onResize();			//*this에서 Window::onResize호출
	}
};

/*
	이 예시는 "캐스트 연산자가 입맛 당기는 상황이라면 뭔가 꼬여가는 징조다"라는 사실을 보여주는 것.
	특히 뭔가 dynamic_cast 연산자인 경우 더 그렇다
	이 연산자는 일단 기본적으로 정말 느리게 구현되어 있다는 점을 알아두자.
	어떤 구현환경들은 클래스 이름에 대한 문자열 비교 연산에 기반을 두어 dynamic_cast가 만들어져 있다(strcmp같은.,,)

	예를 들어 깊이가 4인 단일 상속 계통에 속한 어떤 객체에 대해 이 연산자를 적용할 때,
	그 구현 환경들은 클래스 이름을 비교하기 위해 strcmp가 최대 4번까지도 불릴 수 있다는 것.
	만약 상속의 깊이가 더 깊거나 다중 상속이라면 비용이 더 커질 것임.

	물론 상황에 따라 어쩔 수 없이 이렇게 동작해야 하는 이유가 있긴하다.(동적 링크 지원을 동반해야 해서)

	그럼 dynamic_cast연산자가 쓰고 싶어지는 때는 언제일까?
	파생 클래스 객체임이 분명한 것이 있어서, 이에 대하여 파생 클래스의 함수를 호출하고 싶은데, 그 객체를 조작할 수 있는 수단으로
	기본 클래스의 포인터밖에 없을 경우는 적지 않게 생긴다. 이러한 문제를 피해 가능 일반적인 방법으로는 두 가지를 들 수 있는데
	1. 파생 클래스 객체에 대한 포인터(혹은 스마트 포인터)를 컨테이너에 담아둠으로써 각 객체를 기본 클래스 인터페이스를 통해
	   조작할 필요를 아예 없애기

	Window, SpecialWindow상속 계통에서 깜박 거리기(blink)기능을 SpecialWindow객체만 지원하게 되어 있다면
	아래의 코드는 좋지 않고.
*/

class SpecialWindow3 : public Window
{
public:
	void blink();
	//...
};

int main()
{
	typedef std::vector<std::tr1::shared_ptr<Window>> VPW;
	VPW winPtrs;
	// ...

	for (VPW::iterator iter = winPtrs.begin(); iter != winPtrs.end(); ++iter)
	{
		if (SpecialWindow3* psw = dynamic_cast<SpecialWindow3*>(iter->get()))		//바람직하지 않은 dynamic_cast
			psw->blink();
	}

	//아래 처럼 하는 것이 좋다

	typedef std::vector<std::tr1::shared_ptr<SpecialWindow3> >VPSW;
	VPSW WinPtrs;

	for (VPSW::iterator iter = WinPtrs.begin(); iter != WinPtrs.end(); ++iter)
	{
		(*iter)->blink();
	}
}
/*
	이 방법으로는 Window에서 파생될 수 있는 모든 녀석들에 대한 포인터를 똑같은 컨테이너에 저장할 수 는 없다.
	즉 다른 타입의 포인터를 담으려면 타입 안정성을 갖춘 컨테이너 여려개가 필요할 것.

	Window에서 뻗어 나온 자손들을 전부 기본 클래스 인터페이스를 통해 조작할 수 있는
	다른 방법이 없는 것은 아니다.
	원하는 조작을 가상 함수 집합을 정리해서 기본 클래스에 넣어두면 된다.
*/

class Window4
{
public:
	virtual void blink() {}		// 아무 동작도 하지 않는 기본 구현
};

class SpecialWindow4 : public Window
{
public:
	virtual void blink() {/*...*/ }	//특정한 동작을 수행하는 blink함수
};

typedef std::vector<std::tr1::shared_ptr<Window4>> VPW;
VPW winPtrs;
// ...

{
	for (VPW::itertor iter = winPtrs.begin(); iter != winPtrs.end(); ++iter)
	{
		(*iter)->blink();			//dynamic_cast가 없음.
	}
}

/*
	위 두 상황 두 가지 다 모든 상황에서 쓸 수는 없지만 dynamic_cast를 쓰는 것 보다는 낫다.

	그리고 정말 꼭 피해야 하는 설계가 있는데, 바로 폭포식(cascading) dynamic_cast라고 불리는 구조이다.

	for(VPW::itertor iter = winPtrs.begin(); iter != winPtrs.end(); ++iter)
	{
	if(SpecialWindow1 *psw1 = dynamic_cast<SpecialWindow1*>(iter->get())){...}
	else if (SpecialWindow2 *psw1 = dynamic_cast<SpecialWindow1*>(iter->get())){...}
	else if (SpecialWindow3 *psw1 = dynamic_cast<SpecialWindow1*>(iter->get())){...}
	...

	크기만 크고 속도도 둔하고 망가지기도 쉬운 코드이다..
	Window클래스 계통이 바뀌면 무조건 보면서 또 수정 해야하나? 하는 코드.
	이런 형태가 아닌 가상 함수 호출에 기반을 둔 어떤 방법이든 써서 바꿔야 한다.

	정말 잘 작성된 C++코드는 캐스팅을 거의 쓰지 않는다. 하지만 캐스팅을 아예 쓰지 않는 것도
	현장의 사정을 무시한 것일 수 있다.
	따라서 안전하게 함수안에 두는 등 최대한 격리 시키는 것이 좋다.
*/