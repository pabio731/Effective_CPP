#include <iostream>

int main()
{
	/*
		인터페이스는 행여나 잘못 사용된다면 몸부림을 보여주도록 만들어져야하는 것이 의무이다.
		가장 이상적인 것은 잘못 사용 된다면 컴파일이 되지 않도록 만드는 것이다.
		올바르게 사용된다면 아무 문제 없어야 한다는 점도 당연한 것.

		이를 위해서는 사용자가 저지를 수 있는 문제에 대한 예측이 기본으로 필요하다.
		아래 코드는 날짜와 관련된 코드이다
	*/

	//class Date
	//{
//	public:
		//Date(int iMonth, int iDay, int iYear);
		//...
	//};

	//딱 보기에는 간단면서 문제가 없어 보이는 코드이다. 하지만 다음과 같은 문제가 있는데,
	/*
		- 매개변수들이 같은 타입이기 때문에 매개변수의 순서를 착각해 잘못 들어갈 가능성.
		- "월"의 경우 1~12가 되어야 하는데 다른 숫자가 들어가도 문제없이 전달이 되는 문제.(타이핑 실수로 인해)

		이러한 문제들을 다음과 같이 막아 보자
		1. 새로운 타입을 사용
	*/

	struct Day
	{
		explicit Day(int d) : val(d) {}
		int  val;
	};

	struct sMonth
	{
		explicit sMonth(int m) : val(m) {}
		int  val;
	};
	struct Year
	{
		explicit Year(int y) : val(y) {}
		int  val;
	};

	class Date
	{
	public:
		Date(const sMonth& m, const Day& d, const Year& y);
	};

	Date d1(30, 3, 1995);	// 잘못된 타입 사용으로 인한 에러
	Date d2(Day(30), sMonth(3), Year(1995));	// 잘못된 순서로 인한 에러
	Date d3(sMonth(3), Day(30), Year(1995));	// 올바른 타입과 순서 사용으로 통과

	/*
		물론 Day,Month, Year을 클래스에 숨겨 만들어도 괜찮겠지만 위는 인터페이스 사용 에러를 막는
		단순한 예시로 충분하다.
		타입만 잘만들어도 값에 제약을 주는 것도 괜찮은 경우가 많다.
		한가지 예시로 enum을 사용하여 12개의 월을 나타낼 수 있는데, enum은 타입 안정성이 떨어진다는 것이 문제이다.

		타입 안정성을 위해서 유효한 Month의 집합을 미리 정의해 두어도 괜찮다. 다음 코드를 보자
	*/

	class Month
	{
	public:
		static sMonth Jan() { return sMonth(1); }
		static sMonth Feb() { return sMonth(2); }
		// ...
		static sMonth Dec() { return sMonth(12); }

	private:
		explicit Month(int m);		//Month 값이 새로 생성되자 않도록 명시 호출 생성자가 private 멤버
	};
	Date d(Month::Jan(), Day(30), Year(1995));

	/*
		객체를 쓰지 않고 함수를 쓴 것은 비지역 정적 객체들의 초기화는 개별번역 단위에서 순서가 정해진다는 사실 때문에

		또 다른 실수를 막는 방법으로는 어떤 타입이 제약을 부여하려 그 타입을 통해 할 수 있는 일들을 묶어 버리는 방법이 있다.
		아주 쉬운 예로 const를 붙이는 것.  operator*의 반환 타입을 const로 한정함으로써 사용자가 사용자 정의 타입에
		다음과 같은 실수를 저자르지 않도록 하는것
	*/
	if (a* b = c) {}		// 원래는 ==연산자로 비교를 하려했던 것인데 오타로 인해...

	/*
		그렇게 하지 않을 특별한 이유가 없다면 사용자 정의 타입은 기본제공 타입처럼 동작하게 만들어라!
		기본제공 타입과 쓸데 없이 어긋나는 동작을 피하는 이유는 일관성 있는 인터페이스를 제공하기 위해서임.

		"일관성"만큼 제대로 된 인터페이스를 만드는 것에 직관적인 요소는 없다.
		사용자가 외워야만 제대로 쓸 수 있는 인터페이스는 잘못쓰기 아주 쉽다.
		다음 코드는 Investment 클래스 계통에 속해있는 어떤 객체를 동적할당하고, 그 객체의 포인터를 반환하는 함수
	*/

	class Investment {	};
	Investment* CreateInvestment1();		// 팩토리 함수. 매개변수는 생략

	/*
		이 함수는 자원 누출을 피하기 위해서 CreateInvestment에서 얻어낸 포인터를 나중에라도 삭제해야한다.
		하지만 사용자가 그 점을 깜빡할 수도 있고, Delete가 두 번 이상 적용될 수도있다.
		이 부분 때문에 이전에 공부한 스마트 포인터를 사용한다고 해도,

		만약 내가 스마트 포인트를 사용해야 한다는 사실도 사용자가 잊어버린다면??
		따라서 차라리 처음부터 팩토리 함수가 스마트 포인터를 반환하게 만들어버려서 문제가 생길 여지를 모두 해결하자
	*/

	std::tr1::shared_ptr<Investment> CreateInvestment2();

	/*
		위와 같이 해두면, 이 함수의 반환 값은 tr1::shared_ptr 에 넣어둘 수 밖에 없을 뿐더러,
		나중에 Investment객체가 필요 없어졌을 때 이 객체르 삭제하는 것을 깜박하고 넘어가는 일도 없을 것이다.
		이렇게 shared_ptr을 반환하는 구조는 자원해제에 대한 사용자의 실수를 상당수 방지할 수 있기 때문에
		잘 숙지해두면 좋다. 이유는 이 스마트 포인터는 생성시점에 삭제자를 직접 엮을 수 있기 때문.( auto_ptr은 없다)

		추가적으로 이러한 가정도 해보자
		CreateInvestment를 통해 얻은 Investment* 포인터를 직접 삭제하는 것이 아닌 GetRidOfInvestment함수를 통해
		삭제를 하고자 한다면 어떨까?. 더 깔끔해 보이지만 오히려 실수를 더 부를 수 있다.
		해당 함수를 잊고 delete를 사용할 수 있다는 점 등....

		CreateInvestment를 살짝 고쳐 GetRidOfInvestment가 삭제자로 묶인 tr1::shared_ptr을 반환하도록 구현한다면?
		tr1::shared_ptr는 두 개의 인자를 받는 생성자가 있는데,
		1. 이 스마트 포인터가 관리할 실제 포인터
		2. 참조 카운트가 0이 될 때 호출될 삭제자
		이를 이용해 null포인터를 물게함고 동시에 삭제자로 GetRidOfInvestment를 갖게 하는 방법으로코드를 쓴다면?
	*/

	std::tr1::shared_ptr<Investment> pInv(static_cast<Investment*>(0), GetRidOfInvestment);	// 첫 번째 매개변수가 포인터이여여 하기 때문에 0을 그냥 사용하지 않음

	std::tr1::shared_ptr<Investment> CreateInvestment3()
	{
		std::tr1::shared_ptr<Investment> RetVal(static_cast<Investment*>(0), GetRidOfInvestment);
		RetVal = ;//RetVal은 실제 객체를 가리키도록

		return RetVal;
	}

	/*
		RetVal로 관리할 실제 객체의 포인터를 결정하는 시점이 RetVal을 생성하는 시점보다 앞설 수 있으며느 위 코드처럼 RetVal을 널로 초기화 하고,
		나중에 대입하는 방법보다  바로 넘겨버리는 것이 좋다. 이유는 나중에...

		추가적으로 tr1::shared_ptr의 아주 좋은 특징으로 "포인터별 삭제자 자동 사용"이 있다.
		이는 "교차DLL문제"를 방지 하게 해주는 특징인데, 이 문제는 객체 생성시 어떤 동적 링크 라이브러리(dynamically linked library: DLL)
		의 new를 썼는데 그 객체를 삭제할 때는 이전의 DLL과 다른 DLL에 있는 delete를 사용하게되면 생기는 문제이다.
		new/delete 짝이 실행되는 DLL이 달라서 꼬이게 되면 대부분 런타임 에러가 난다.
		하지만 tr1_shared_ptr은 생성된 DLL과 동일한 DLL에서 delete를 사용하도록 만들어져 있기 때문에 이 문제를 걱정하지 않아도 된다.

		부스트라이브러리의 shared_ptr은 크기가 원시 포인터의 두 배이고, 내부 관리 데이터 및 삭제자 매커니즘을 돌릴 데이터를 위해 동적 할당을
		사용하며, 다중 스레드 프로그램이라면 참조 카운트를 변경할 때 스레드 동기화 오버헤드도 일으킨다.
		하지만 이러한 문제는 생각보다 런타임 비용이 많이 늘지는 않을 것이며, 시용자의 실수는 눈에 띄게 줄어드는 모습을 볼 수 있을 것이다.
	*/
}