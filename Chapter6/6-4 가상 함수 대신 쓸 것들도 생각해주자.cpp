#include"pch.h"
#include<functional>
/*
	게임의 각종 캐릭터를 클래스로 설계하는 작업을 하고 있다고 가정하자.
	캐릭터의 체력이 얼마나 남았는지를 나타내는 정수값을 반환하는 HealthValue라는 멤버함수를 만들고,
	이를 각 캐릭터마다 다르게 정의 해주는 것이 마땅해 보이니 가상함수로 선언하며, 기본적인 체력에 관한
	동작 원리는 제공한다고 생각하자
*/

class GameCharacter
{
public:
	virtual int HealthValue() const;	// 캐릭터의 체력치를 반환하는 함수. 파생 클래스는 이 함수를 재정의 할 수 있음.
};
/*
	너무나 당연한 설계이지만 또 다른 의미로는 이것이 약점이다.
	따라서 이외의 다른 설계 방법을 보도록 하자

	1. 비가상 인터페이스 관용구를 통한 템플릿 메서스 패턴
		"가상 함수는 반드시 private멤버로 두어야 한다"라는 '가상 함수 은폐론'에 따르는 코드를 보자
		HealthValue을 public 멤버 함수로 그대로 두되, 비가상 함수로 선언하고, 내부적으로는 실제 동작을 맡은 private가상 함수를 호출하는 것
*/

class GameCharacter2
{
private:
	virtual int DoHealthValue() const
	{
		int RetVal(0);
		//캐릭터의 능력치 계산을 위한 기본 알고리즘 구현
		return RetVal;
	}
public:
	int HealthValue() const
	{
		// "사전"동작을 수행
		int RetVal = DoHealthValue();
		// "사후"동작을 수행

		return RetVal;
	}
};

/*
		이런 방식의 설계이다. public 비가상 멤버 함수를 통해 private가상 함수를 간접적으로 호출하게 만드는 방법으로,
		비가상 함수 인터페이스(non-virtual interface:NVI)관용구라고 한다.
		이 관용구는 템플릿 메서드(Template Method)라 불리는 고전 디자인 패턴을 C++식으로 구현한 것

		이 방식의 이점은 "사전"동작과 "사후"동작을 수행할 수 있다는 것에 있다. 가상 함수를 호출하기 전에 어떤 동작을 하고,
		(예를 들면 뮤텍스 잠금이나 로그 남기기 같은)
		가상 함수를 호출한 후 그 상태를 없애는 작업이 가능하다
		(예를 들면 뮤텍스 잠금 해제나 함수의 사후 조건의 점검 같은)

		private 가상 함수를 파생 클래스에서 재정의 할 수는 있지만 호출은 할 수 없다.
		설계상의 모순이 있는 것은 아니다. 가상 함수를 재정의 하는 것은 동작을 어떻게 구현할 것인가를 지정하는 것이고,
		가상함수를 호출하는 일은 그 동작이 수행될 시점을 지정하는 것. 즉 서로 관심사가 무관한 것.

		NVI관용구에서는 파생 클래스의 가상함수 재정의를 허용하기 때문에 어떤 기능을 어떻게 구현할 것인가는 파생 클래스의
		권한이지만 함수를 언제 호출할 것인지를 정하는 것은 기본 클래스의 고유 권한이다 (해야하는 사전 동작이 끝난 후 함수를 호출하도록 기본 클래스에서 만들어졌으니)
		이 관용구에서 가상 함수가 엄격하게 private일 필요는 없다. 어떤 클래스 계통의 경우 파생 클래스에서 재정의되는 가상 함수가 기본 클래스의
		대응 함수를 호출할 것을 예상하고 설계된 것도 있는데 이 경우 적법한 함수 호출이 되려면 protected멤버여야 한다.

	2. 함수 포인터로 구현한 전략패턴
		위 NVI관용구는 가상 함수를 대신할 수 있는 나쁘지 않은 방법이지만, 클래스 설계의 관점에서 보면 눈속임이나 다름없다.
		어쨌든 가상함수를 사용하기 때문에.
		조금 더 극적으로, 캐릭터의 체력치를 계산하는 작업은 캐릭터의 타입과 별개로 놓는 것이 맞을 것이다.
		한 예로, 각 캐릭터의 생성자에 체력 계산용 함수의 포인터를 넘기게 만들고, 이 함수를 호출해서 계산을 수행하도록 해보자
*/

class GameCharacter3;	//전방선언

int DefaultHealthCalc(const GameCharacter3& gc);

class GameCharacter3
{
public:
	typedef int (*HealthCalcFunc)(const GameCharacter3&);

	explicit GameCharacter3(HealthCalcFunc Hcf = DefaultHealthCalc)
		: HealthFunc(Hcf) {}

	int HealthValue() const { return HealthFunc(*this); }

private:
	HealthCalcFunc HealthFunc;
};

/*
		이 방법은 전략(strategy)패턴을 응용한 예이다.
		가상 함수를 쓰는 것보다 재밌는 융통성을 가지는데, 같은 캐릭터타입으로 만들어진 객체들도 체력치 계싼 함수를 각각 다르게 가질 수 있다는 것
		다음 코드를 보자
*/

class EvilBadGuy :public GameCharacter3
{
public:
	explicit EvilBadGuy(HealthCalcFunc Hcf = DefaultHealthCalc)
		: GameCharacter3(Hcf) {}
};

int LoseHealthQuickly(const GameCharacter3&);
int LoseHealthSlowly(const GameCharacter3&);	//각각 다른 동작 원리로 구현된 체력치 계산 함수들

EvilBadGuy edg1(LoseHealthQuickly);
EvilBadGuy edg2(LoseHealthSlowly);		//같은 타입이지만 체력치가 다르게 나오는 캐릭터들 (다른 함수의 주소를 넘겨주기 때문에)

/*
		게임이 진행되는 도중에 특정 캐릭터에 대한 능령치 계산 함수를 바꿀 수 있다.
		예를 들어 GameCharacter클래스에서 다른 체력 계산 멤버 함수를 들고 있다면 이를 통해서
		현재 사용중인 체력 계산 함수의 교체가 가능한 것.

		하지만 단점도 존재한다. 체력치 계산 함수가 멤버 함수가 아니기 때문에 계산되는 객체의 비공개 데이터에 접근이 안된다.
		정확한 계산을 위해 public멤버가 아닌 정보를 써야하는 경우 문제가 된다.
		public영역에 없는 부분을 비멤버 함수도 접근할 수 있게 하려면 그 클래스의 캡슐화를 약화 시키는 방법밖에는 없다는 것이 일반적 법칙이다.
		프렌드의 구현이든, 세부 구현사항에 대한 접근자를 public 멤버로 제공하든 말이다.
		무엇이 더 이득인가는 상황에 따라 판단해야한다.

	3. tr1::function으로 구현한 전략 패턴

		그렇다면 체력치 계산을 함수가 하지 않도록 하는 것에 대해 생각해보자. 함수로 구현한다면 왜 멤버 함수는 안되냐는 의문도 생길 수 있다.
		tr1::function타입의 객체를 써서 기존의 함수 포인터를 대신하게 만들면 된다.
		tr1::function타입의 객체는 함수호출성 개체(callable entity)(함수 포인터, 함수 객체 혹은 멤버 함수 포인터)를 가질 수 있고,
		이들 개체는 주어진 시점에서 예상되는 시그너쳐와 호환되는 시그너쳐를 가지고 있다.
*/

class GameCharacter4;
int DefaultHealthCalc2(const GameCharacter4& gc);

class GameCharacter4
{
public:
	typedef std::tr1::function<int(const GameCharacter4&)> HealthCalcFunc;

	explicit GameCharacter4(HealthCalcFunc Hcf = DefaultHealthCalc2)
		:HealthFunc(Hcf) {}

	int HealthValue() const { return HealthFunc(*this); }
private:
	HealthCalcFunc HealthFunc;
};

/*
		HealthCalcFunc는 tr1::function테 템플릿을 인스턴스화 한 것에 대한 typedef타입이다
		즉, 일반화된 포인터 타입처럼 동작한다는 것.

		typedef std::tr1::function<int(const GameCharacter4&)> 는 HealthCalcFunc를 typedef 한 것이고,
		int(const GameCharacter4&)가 tr1::function을 인스턴스화 하기 위해 매개변수로 쓰인 대상 시그니쳐이다.
		이 시그니쳐를 해석하면 const GameCharacter4에 대한 참조자를 받고 int로 반환하는 함수이다.
		이렇게 정의된 tr1::function타입의 객체는 앞으로 대상 시그니처와 호환되는 함수 호출성 개체를 어떤 것도 가질 수 있다.

		여기서 "호환된다"라고 말하는 것은 함수객체의 매개 변수 타입이 const GameCharacter4& 이거나 const GameCharacter4&로
		암시적 변환이 가능한 타입이며, 반환 타입도 암시적으로 int 로 변환될 수 있다는 뜻이다.

		이전 설계와 비교해서 크게 다른 것은 없지만 이제 GameCharacter4가 이제 좀 더 일반화된 함수 포인터를 물게 됐다는 것이다.
		하지만 이 작은 변화로인해 융통성은 엄청나게 불었났다
*/

short CalcHealth(const GameCharacter4&);	// 반환타입이 int가 아닌 체력 계산 함수

struct HealthCalculator
{
	int operator()(const GameCharacter4&) const {}	//체력 계산용 함수 객체를 만들기 위한 클래스
};

class GameLevel
{
public:
	float Health(const GameCharacter4&) const;		// 체력치 계산에 쓰일 멤버 함수. 반환타입이 int가 아님
};

class EvilBadGuy2 : public GameCharacter4
{
	explicit EvilBadGuy2(HealthCalcFunc Hcf = DefaultHealthCalc2)
		: GameCharacter4(Hcf) {}
};

class EyeCandyCharacter : public GameCharacter4
{
	explicit EyeCandyCharacter(HealthCalcFunc Hcf = DefaultHealthCalc2)
		:GameCharacter4(Hcf) {}
};

int main()
{
	EvilBadGuy2 ebg1(CalcHealth);		// 체력 계산을 위한 함수를 사용하는 캐릭터

	EyeCandyCharacter ecc1(HealthCalculator()); //체력치 계산을 위해 함수 객체를 사용하는 캐릭터

	GameLevel CurrentLevel;

	EvilBadGuy2 ebg2(std::tr1::bind(&GameLevel::Health, CurrentLevel, _1));
}

/*
		위 정의문이 말하는 바는 ebg2의 체력을 계산하기 위해 GameLevel 클래스의 Health 멤버 함수를 써야한다는 것이다.  그리고 그
		함수는 매개변수 하나를 받는 것으로 선언되어 있지만 실제로는 GameLevel 객체를 암시적으로 받아들이기 때문에 두 개이다.
		하지만 GameCharacter4 객체에 쓰는 체력 계산 함수가 받는 매개변수는 체력이 계산되는 GameCharacter4 객체 하나뿐이다.
		ebg2의 체력 계산에 GameLevel::Health 함수를 쓰려고 한다면 어떻게든 매개변수 하나만 함수로 바꿔야 한다는 것.
		지금의 코드에서는 ebg2 의 체력 계산에 쓸 GameLevel 객체로 CurrentLevel만을 쓸 생각이므로, GameLevel::Health 함수가
		호출될 때마다 CurrentLevel이 사용되도록 묶어준 것이다. 다시말해 ebg2의 체력 계산 함수는 함상 CurrentLevel 만을
		GameLevel객체로 쓴다고 지정한 것.

	4. 고전적인 전략패턴

		체력 계산 함수를 나타내는 클래스 계통을 아예 따로 만들고, 실제 체력 계산 함수는
		이 클래스 계통의 가상 멤버 함수로 만드는 것.
		GameCharacter가 상속 계통의 최상위 클래스이고 EvilBadGuy와 EyeCandyCharacter는 여기서 갈라져 나온 파생 클래스이다.
		한편 HealthCalcFunc는 SlowHealthLoser 와 FastHealthLoser등을 파생 클래스로 거느린 최상위 클래스이다.
		추가로 GameCharacter타입을 따르는 모든 객체는 HealthCalcFunc타입의 객체에 대한 초인터를 포함하고 있다.
*/

class GameCharacter5;
class HealthCalcFunc
{
public:
	//...
	virtual int Calc(const GameCharacter5& gc) const {}
};

HealthCalcFunc DefaulthealthCalc;

class GameCharacter5
{
public:
	explicit GameCharacter5(HealthCalcFunc* phcf = &DefaulthealthCalc)
		:pHealthCalc(phcf) {}

	int healthValue() const { return pHealthCalc->Calc(*this); }
private:
	HealthCalcFunc* pHealthCalc;
};
/*
	이 방법은 "표준적인" 전략 패턴 구현 방법에 친숙한 경우에 빨리 이해할 수 있다는 점에서 매력적이다.
	게다가 HealthCalcFunc클래스 계통에 파생 클래스를 추가함으로써 기존의 체력 계산 알고리즘을 조정/개조할 수 있는 가능성도 열어놓았다.
*/