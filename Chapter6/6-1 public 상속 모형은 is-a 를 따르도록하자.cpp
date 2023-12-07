#include "pch.h"

/*
	만약 클래스 Base(B)로부터 Derived(D)클래스를 파생시켰다면, 우리는 컴파일러에게 이렇게 말한 것과 같다.
	"D타입으로 만들어진 모든 객체는 또한 B타임의 객체이지만, 그 반대는 되지 않는다."
	즉 B는 D보다 더 일반적인 개념이며, D는 B보다 더 특수한 개념이다.  또 B타입이 쓰일 수 있는 곳에는
	D타입도 쓰일 수 있다고 "단정"하는 것이다. D타입의 모든 객체는 B타입의 객체도 되니까...

	반면 D타입이 필요한 부분에 B타입 객체를 쓰는 것은 불가능하다.  D는 B의 일종이지만
	B는 D의 일종이 아니기 때문에....

	Person타입의 인자를 기대하는 함수는 Student 객체도 받아들일 수 있다.
	다음 코드를 보자
*/
class Person {};
class Student :Person {};

void Eat(const Person& p);			// 먹는 것은 누군든 한다
void Study(const Student& s);		// 공부는 학생만 한다

Person P;
Student S;

Eat(P);						// 가능
Eat(S);						// 가능 S는 Person의 일종이니

Study(S);					//가능
Study(P);					//에러! P는 Student가 아님!

/*
	이 이야기는 public 상속에서만 통한다.  private상속은 의미 자체가 다르고, protected 상속은 애매하다.

	public 상속과 is-a 관계가 똑같은 뜻이라는 말은 직관적이지만, 이 직관 떄문에
	판단을 잘못 하는 경우도 있다.
	펭귄은 새의 일종이고,
	새라는 개념만 생각한다면 새가 날 수 있다는 점도 사실이다.
	이를 코드로 표현한다면?
*/

class Bird1
{
public:
	virtual void Fly();		//새는 날 수 있다.
};

class Penguin1 : public Bird {};	// 펭귄은 새의 일종이다.

/*
	위 코드를 보면 뭔가 이상하다. 펭귄은 새는 맞지만 날지는 못하기 때문이다.
	즉 "모든 종류의 새가 날지는 않는다"라는 점을 간과한 것. 이 때문에 기본 클래스의 성질에 대해 잘 생각해야 한다
	다음과 같은 코드가 올바른 것
*/

class Bird2 {};
class FlyBird : public Bird2
{
public:
	virtual void Fly();
};
class Penguin2 :public Bird2 {};		//Fly함수가 선언 X

/*
	그렇다고 이 것으로 끝난 것이 아니다. 어떤 소프트웨어에서는 비행능력이 있는 새와 없는 새를 구분할 필요가 없기 때문
	이런 점이 소프트웨어 설계엔 이상적인 것 따위는 없다는 사실을 반증하는 것.
	최고의 설계는 제작하려는 소프트웨어 시스템이 기대하는 바에 따라서 만들어지는 것이다.
	결국 비행에 대한 지식을 전혀 쓰지 않고, 나중에도 쓸 일이 없다면, 날 수 있는 새와
	날지 못하는 새를 구분할 필요가 전혀 없는 것이다.

	그럼 위와 같은 방법이 아니라 또 다른 방법을 생각하는 사람도 있을 수 있다. 바로
	펭귄의 Fly함수를 재정의해서 런타임 에러를 내자는 것.
*/

void Error(const std::string& Msg);		// 어딘가에 정의된 에러 함수
class Bird3
{
public:
	virtual	void Fly();
};
class Penguin3 : public Bird3
{
public:
	virtual void Fly()
	{
		Error("Attempt to make a Penguin Fly");
	}
};
/*
	이렇게 설계하는 것은 "펭귄은 날지 못한다"가 아니다
	"펭귀는 날 수 있으나 날려고 하면 에러가 난다"이다.

	"펭귄을 날 수 없다"라는 지령은 컴파일러가 내릴 수 잇지만,"펜귄이 실제로 날려고 하면 에러가 난다"라는
	규칙을 위반하는 것은 프로그램이 실행될 때만 발견할 수 있다.

	그러니 그냥 Bird 클래스에서 Fly함수를 선언하지 않고, Penguin 클래스에서도 Fly선언하지 않도록 해서
	"펭귄을 날 수 없다"라는 확실한 정의를  내려주도록 하자. "유효하지 않은 코드는 컴파일 단계에서
	막아주는 인터페이스가 좋은 인터페이스이다"라는 것을 잘 기억하자.

	다음으로 정사각형과 직사각형에 대해 생각해보자.
	정사각형(Square)클래스는 직사각형(Rectangle)클래스로부터 상속을 받아야할까?
	우리가 학교에서 배운 개념으로는 당연한 사실이다.

	하지만 아래 코드를 보자
*/

class Rectangle
{
public:
	virtual void SetHeught(int NewHeight);
	virtual void SetWidth(int NewWidth);
	virtual int Height() const;		//현재의 값 반환
	virtual int Width() const;
};

void MakeBigger(Rectangle& r)
{
	int iOldHeight = r.Height();
	r.SetWidth(r.Width() + 10);		//r의 가로 길이에 10을 더함.
	assert(r.Height() == iOldHeight);	// r의 세로 길이가 변하지 않는다는 조건에 단정문을 걸어둠
}

/*
	여기서 위의 단정문이 실패할 일이 없다는 것은 확실하다 MakeBigger함수는 r의 가로 길이만 변결할 뿐이고, 세로 길이는 바뀌지 않는다.
	이제 다음 코드를 보자. 상속을 해서 정사각형을 직사각형처럶 처리하게끔 허용하는 코드이다
*/

class Square :public Rectangle {};
int main()
{
	Square square;
	assert(square.Width() == square.Height());	// 이 단정문은 모든 정사각형에 대해 참이여야 한다.

	MakeBigger(square);							//상속한 것이므로, square는 Rectangle의 일종임. 즉 square의 넓이를 늘릴 수 있다.
	assert(square.Width() == square.Height());
}
/*
	당연히 두 번째 단정문도 실패해서는 안된다. 정사각형의 정의가 그렇듯 정사각형의 가로 길이는 세로 길이와 같으니
	그런데 위의 코드는 문제를 일으키지 않는다.

	이렇게 이론적인 것괴 실제로 우리가 프로그래밍으로 구현하는 것에는 차이가 있는 것이다.
	직사각형의 성질 중 어떤 것은 정사각형에 적용할 수 없다는 것.
	컴파일이 잘된다는 것이 문제가 없느다는 말이 안난 것쯤은 당연한 일이다.

	추가적으로 클래스들 사이에 맺을 수 있는 관계로 is-a 외에도 has-a 관계도 있고, is-implementted-in-terms-of(...는...를  써서 구현됨)도 있다.
*/