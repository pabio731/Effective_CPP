#include "pch.h"

/*
	private 상속은 is-a를 뜻하지 않는다. 그렇다면 private 상속의 의미는 무엇인가?
	일단 먼저 private 상속이 어떻게 작동하는지 알아보자

	1. public 상속과 대조적으로, 클래스 사이의 상속 관계가 private라면 컴팡이러는 일반적으로 파생 클래스 객체를
	   기본 클래스 객체로 변환하지 않는다.
	2. 기본 클래스로부터 물려받은 멤버는 파생 클래스에서 모조리 private 멤버가 된다.
	   기본 클래스에서 원래 protected 였거나 public이였던 것들도 마찬가지이다.

	즉 private 상속의 의미는 is-implemente-in-terms-of이다.
	Base클래스로부터 private 상속을 통해서 Derived 클래스를 파생 시킨 것은 Base 클래스에서 쓸 수 있는
	기능들 몇개를 활용할 목적으로 하는 것. Base 타입과 Derived 타입간의 어떤 개념적인 관계가 있어서 한 행동이
	아니라는 것이다.

	private 상속은 그 자체로 구현 기법중 하나이다 (기본 클래스는 그저 구현 세부사항이 되는것)
	"구현만 물려받을 수 있다. 인터페이스는 국물도 없다"

	Derived가 Base를 private 상속하면 그냥 Drived객체가 Base객체를 써서 구현되는 것이라고 생각하면 된다.
	private 상속은 소프트웨어 설계 도중에는 아무 의미를 갖지 않으며 단지 소프트웨어 구현 중에만 의미를 가질 뿐이다.

	그럼 똑같이 is-implemente-in-terms-of 의미를 가지는 객체 합성과 둘 중 어떤 것을 이용해서 구현하는 것이 좋을까?
	1. 할 수 있으면 객체 합성을 이용한다.
	2. 꼭 해야만 할 때 private상속을 사용한다.
	그렇다면 꼭 해야만 하는 때는 언제인가?

	비공개 멤버를 접근할 때 혹은 가상 함수를 재정의 할 경우. 또는 공간의 문제로인한 정말 어쩔 수 없는 경우이다.

	예시를 통해 좀 더 알아보자
	Widget 객체를 사용하는 프로그램 하나를 만들고 있다고 가정하자.
	이 객체가 사용되는지 좀 더 이해할 필요가 있어 멤버 함수의 호출 횟수나 호출 비율 등을 알고 싶게 된 것이다.

	그런 이유로 Widget 클래스를 직접 손보기로 한다.
	멤버 함수의 호출 횟수 정보는 프로그램이 실행되는 도중 주기적으로 점검하도록 만들텐데, 이 정보 외에 각Widget객체의 값과
	추가적으로 필요하다고 여겨지는 자료들도 넣을 수 있을 것이다.

	이 작업을 위해 일종의 타이머를 넣자.
	*/

class Timer
{
public:
	explicit Timer(int TickFrequency);
	virtual void Ontick() const;	// 일정 시간이 경과할 때마다 자동으로 호출
};

/*
	Timer 객체는 반복적으로 시간을 경과시킬 주기를 우리가 정해줄 수 있고, 일정 시간이 경과할 때마다
	가상 함수를 호출하도록 되어 있다.

	이렇게 하려면 Widget 클래스에서 Timer의 가상 함수를 재정의할 수 있어야 하므로,
	Widget 클래스는 Timer에서 상속을 받아야 한다. 이 경우 public 상속은 맞지 않고, Widget객체의
	사용자는 Widget 객체를 통해 OnTick 함수를 호출하면 안되기 때문에, private 상속을 하자
*/

class Widget : private Timer
{
private:
	virtual void OnTick() const;	//Widget 사용 자료등을 수집한다.
};

/*
	상속을 private로 한 덕에 Timer의 public 멤버인 OnTick 함수는 Widget에서 private멤버가 된다.
	흠잡을 것 없는 설계이지만 이 OnTick 함수를 꼭 private 상속을 할 필요가 있느냐는 점은 조금 생각해
	볼만 하다. 객체 함성을 써도 충분히 가능한 상태이기 때문에...

	Timer로부터 public 상속을 받은 클래스를 Widget안에 private 중첩 클래스로 선언해 놓고, 이 클래스에서 OnTick을
	재정의한 다음,
	이 타입의 객체 하나를 Widget 안에 데이터 멤버로 넣는 것.
*/

class Widget
{
private:

	class WidgetTimer : public Timer
	{
	public:
		virtual void OnTick() const;
	};
	WidgetTimer timer;
};

/*
	privat 상속보다 상당히 복잡해 보인다. 하지만 위 문제에 대해 다양한 접근 방식이 있다는 점을 배우기엔
	충분하다. 그리고 public 상속에 객체 합성 조합이 더 많이 쓰인다.
	다음의 이점 때문

	1. Widget 클래스를 설계하는 데 있어서 파생은 가능하게 하되, 파생 클래스에서
	   OnTick은 재정의할 수 없도록 설계 차원에서 막고 싶을 때 유용하다.
	   Timer로부터 상속을 받은 WidgetTimer가 Widget 클래스의 private 영역에 있으면 Widget의
	   파생 클래스는 아무리 용을 써도 WidgetTimer에 접근할 수 없다.
	   -> 이점은 이제 final 키워드가 있어서 의미 없음

	2. Widget의 컴파일 의존성을 최소화 하고 싶을 때 좋다.
	   Widget이 Timer에서 파생된 상태라면, Widget이 컴파일될 때 Timer의 정의도 끌어올 수 있어야 하기 때문에
	   Widget의 정의부 파일에서 Timer.h같은 헤더를 포함시켜야 할지도 모른다.
	   반면 위 설계는 WidgetTimer의 정의를 Widget으로부터 빼내고, Widget이 WidgetTimer객체에 대한 포인터만 갖도록
	   만들어 두면 컴파일 의존성을 줄일 수 있다.

	이제 공간 최적화로 인한 어쩔 수 없는 private 상속에 대해 알아보자.
	이 것은 "정말 정말 만에 하나"라는 점을 알아두자

	비정적 데이터 멤버가 없는 공백 클래스(가상 함수도, 가상 기본 클래스도 없어야 함)는 기본적으로 차지하는 메모리 공간이
	없는게 맞다. 하지만 선언 해보면 대개 1byte를 가지게 된다. 이는 C++의 "독립 구조의 객체는 반드시 크기가 0을 넘어야 한다"라는
	금기 사항 때문이다. char 한개를 슬그머니 넣는 것. 심지어 바이트 정렬이 필요하다고 생각되면 int를 담을 수 있는 만큼으로 늘어나기도.

	하지만 이 제약은 파생 클래스의 기본 클래스 부분에는 적용이 되지 않는다.
	따라서 이렇게 공간을 절약할 수 있는데 이를 "공백 기본 클래스 최적화"라고 한다.
	실무적으로 공백 클래스가 진짜로 텅 빈 것은 아니다.
	비정적 데이터 멤버는 갖지 않ㅈ지만 typedef나 enum, 정데이터 멤버는 물론이고 비가상 함수까지 갖는 경우가 많다.

	STL에는 이런 (대부분 typedef타입) 기술적으로 공백 처리된 클래스가 많이 있다.

	하지만 어쨌든 대배분의 상속 관계는 is-a 관계이고, is-implemente-in-terms-of관계는 객체 합성이 낫다.

	private 상속은 다른 모든 대안을 검토한 후에 마지막에 쓸 수 밖에 없을 때 쓰도록 하자
*/