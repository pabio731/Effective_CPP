﻿#include "pch.h"

/*
	객체 지향 프로그래밍의 세계를 회전 시키는 축은 명시적 인터페이스와 런타임 다형성이다.
	예를 들어서 다음과 같은 클래스와 함수가 있을 때,
*/

class Widget
{
public:
	Widget();
	virtual ~Widget();
	virtual std::size_t size() const;
	virtual void Normalize();
	void Swap(Widget& Other);
};

void DoProcessing(Widget& w)
{
	if (w.size() > 10 && w != SomeNastyWidget)
	{
		Widget Temp(w);
		Temp.Normalize();
		Temp.Swap(w);
	}
}
/*
	DoProcessing 함수 안에 있는 w에 대해 말할 수 있는 부분은 다음과 같다.
	1. w는 Widget 타입으로 선언되었기 때문에, w는 Widget 인터페이스를 지원해야 한다.
	   이 인터페이스를 소스 코드에서 찾을면 이것이 어떤 형태인지를 확인할 수 있으므로,
	   이런 인터페이스를 가리켜 명시적 인터페이스라고 한다. 다시말해 소스 코드에 명시적으로
	   드러나는 인터페이스를 말하는 것.

	2. Widget의 멤버 함수 중 몇 개는 가상 함수이므로, 이 가상 함수에 대한 호출은 런타임
	   다형성에 의해 이루어진다. 다시 말해, 특정한 함수에 대한 실제 호출은 w의 동적 타입을
	   기반으로 런타임에 의해 결정된다.

	템플릿과 일반화 프로그래밍 세계에는 뿌리부터 뭔가 다른 부분이 있다. 명시적 인터페이스 및
	런타임 다형성은 그대로 존재하긴 하지만 중요도는 사뭇 떨어진다.

	템플릿과 일반화 프로그래밍에서 중요한 것은 암시적 인터페이스와 컴파일 타임 다형성이다.

	이를 알기 위해 DoDoProcessing 함수를 함수 템플릿으로 바꿔보자
*/

template <typename T>
void DoProcessing(T& w)
{
	if (w.size() > 10 && w != SomeNastyWidget)
	{
		T Temp(w);
		Temp.Normalize();
		Temp.Swap(w);
	}
}

/*
	이번에는 DoProcessing 템플릿 안의 w에 대해서 어떻게 말할 수 있는가?

	1. w가 지원하는 인터페이스는 이 템플릿 안에서 w에 대해 실행되는 연산이 결정한다.
	   지금의 경우 size,Normalize,Swap 멤버 함수를 지원해야 하는 쪽은 w의 타입,
	   즉 T이다. 그뿐 아니라
	   T는 Temp를 만들기 위해 복사 생성자도 지원해야하고
	   SomeNastyWidget와 비교를 위해서 부등 비교를 위한 연산도 지원해야 한다.
	   이 템플릿이 제대로 컴파일 되려면 몇 개의 표현식이 "유효"해야 하는데
	   이렇게 T가 지원해야 하는 인터페이스들을 암시적 인터페이스라고 한다.

	2. w가 수반되는 함수 호출이 일어날 때, 이를테면 operator> 와 operator!=함수가
	   호출될 때는 해당 호출을 성공시키기 위해 템플릿의 인스턴스화가 어난다.
	   근데 이 인스턴스화가 일어나는 시점이 컴파일 도중이라는 것.
	   템플릿에 어떤 매개변수가 들어가느냐에 따라 호출되는 함수가 달라지기 때문에
	   이를 가리켜 컴파일 타임 다형성이라고 한다.

	명시적 인터페이스와 암시적 인터페이스에 대해 감을 잡기 힘드므로 좀 더 자세히 알아보자
	명시적 인터페이스는 대개 함수 시그너처로 이루어진다.
	그리고 함수 시그너처는 함수의 이름, 매개변수 타입, 반환 타입 등을 통틀어 부르는 것
	데이터 멤버의 경우 접근지정과 상관없니 시그너처에 들어가지 않는다.

	반면 암시적 인터페이스는 함수 시그너쳐에 기반하지 않는다.
	암시적 인터페이스를 이루는 요소는 유효 표현식이다.

	위 함수의 조건문을 보면 T에서 제공될 암시적 인터페이스에는 다음과 같은 제약이 걸릴 것(&&편의상 제외)
	1. 정수 계열의 값을 반환하고 이름이 size인 함수를 지원해야 한다.
	2. T타입의 객체 둘을 비교하는 operator != 함수를 지원해야 한다.

	사실 실제로는 연산자 오버로딩 때문에 두가지 제약 중 어떤 것도 만족시킬 필요는 없다.
	size함수를 지원해야 하는 것은 맞지만 수치 타입을 반환할 필요까지는 없다.
	그저 어떤 X타입의 객체와 int가 함꼐 호출될 수 있는 operator>가 성립될 수 있도록, X타입의
	객체만 반환하면 된다.
	또 operator>함수는 Y타입의 매개변수를 받도록 정의되어 있고 X타입에서 Y타입으로
	암시적이 변환이 가능하다면 X타입의 매개변수를 받아들일 필요가 없다.

	아무튼 암시적 인터페이스는 그저 유효 표현식의 집합으로 구성되어 있을 뿐이다.

	*/