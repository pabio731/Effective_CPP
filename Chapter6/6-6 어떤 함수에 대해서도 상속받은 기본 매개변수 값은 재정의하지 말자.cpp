#include "pch.h"

/*
	C++에서 상속받을 수 있는 함수의 종류는 가상 함수와 비가상 함수 두 가지 밖에 없다.
	그리고 비가상 함수는 절대 재정의 해서는 안된다는 점을 알았으니 다음으로 디폴트 매개변수를 가진 가상 함수를
	상속하는 경우에 대해 생각해보자.

	핵심은 가상 함수는 동적으로 바인딩되지만, 디폴트 매개변수 값은 정적으로 바인딩 된다는 것이다.

	객체의 정적 타입은 우리가 놓는 선언문을 통해서 그 객체가 갖는 타입이다.
	다음 코드를 보자
*/
class Shape
{
public:
	enum ShapeColor { Red, Green, Blue };

	// 모든 도형이 갖는 자기 자신을 그리는 함수
	virtual void Draw(ShapeColor Color = Red) const = 0;
};

class Rectangle : public Shape
{
	// 디폴트 매개변수가 달라졌다
	virtual void Draw(ShapeColor Color = Green) const;
};

class Circle :public Shape
{
public:
	virtual void Draw(ShapeColor Color) const;
};

int main()
{
	Shape* ps;							// 정적 타입 = Shape*      동적 타입  = 없음 (가리키는 대상이 없으니)
	Shape* pc = new Circle;				// 정적 타입 = Shape*	  동적 타입  =  Circle*
	Shape* pr = new Rectangle;			// 정적 타입 = Shape*	  동적 타입  =  Rectangle*

/*
	ps,pc,pr 모두 Shape에 대한 포인터로 선언되어 있기 때문에 각각의 정적 타입도 모두 Shape이다.
	정적 타입만 그렇다는 거지 진짜 가리키는 대상이 달라지는 것은 없음.

	객체의 동적 타입은 현재 그 객체가 진짜로 무엇이냐에 따라 결정되는 타입이다.
	즉 "이 객체가 어떻게 동작할 것이냐"를 가리키는 타입이 동적 타입이다.
	그리고 동적 타입은 이름에서 느껴지듯 프로그램 실행되는 도중에 바뀔 수 있다.
	대개 대입문을 통해서 바뀐다
	위의 예를 이용하자면
	ps = pc		// ps의 동적 타입이 이제 Circle*로.
	ps = pr		// ps의 동적 타입이 이제 Rectangle*로

	가상 함수는 동적으로 바인딩된다. 즉 호출이 일어난 객체의 동적 타입에 따라 어떤 함수가 호출될지가 결정된다는 뜻.
*/
	pc->Draw(Shape::Red);		// Circle::Draw(Shape::Red)를 호출
	pr->Draw(Shape::Red);		// Rectangle::Draw(Shape::Red)를 호출

/*
	이제 디폴트 매개변수 값이 설정된 가상 함수로 오게되면 뭔가 꼬이기 시작한다.
	이유는 디폴트 매개변수는 정적으로 바인딩되어 있기 때문이다.
	즉, 파생 클래스에 정의된 가상 함수를 호출하면서 기본 클래스에 정의된 디폴트 매개변수 값을 사용해버릴 수 있다는 것.
*/
	pr->Draw();	//Rectangle::Draw(Shape::Red)를 호출해 버린다.
}
/*
	이 경우 pr의 동적 타입이 Rectangle* 이므로 호출되는 함수는 Rectangle의 것이다. 그리고 Rectangle::Draw함수에서는
	디폴트 매개변수값이 Green으로 되어있다. 하지만 pr의 정적 타입은 Shape* 이기 때문에 지금 호출되는 가상 함수에 쓰이는
	디폴트 매개변수 값을 Shape클래스에서 가져온다. 양쪽에서 선언된 것이 한데 섞이는 이상한 일이 벌어지는 것!

	포인터가 아닌 참조자도 마찬가지이다. 중요한 것은 디폴트 매개변수 값들 중 하나가 파생 클래스에서 재정의 되면
	여지없이 문제가 생긴다는 것이다.

	이 방식은 컴파일의 속도와 효휼 때문에 이렇게 구현된 것.
	그럼 기본 클래스와 파생 클래스의 사용자에게 디폴트 매개변수 값을 똑같이 제공해보려 하면?

	코드 중복에다가... 의존성까지 더해진다. 기본 클래스가 바뀌면 나머지도 다 바꿔줘야해...

	이 문제를 해결하기 위해서는 다른 방법을 택하는 것이 낫다.
	이전에 공부한 비가상 인터페이스(NVI관용구)를 쓰자.

	파생 클래스에서 재정의 할 수 있는 가상함수를 private멤버로 만들고. 해당 가상 함수를 호출하는
	public 비가상 함수를 기본 클래스에 만들어두자.
*/

class Shape2
{
public:
	enum ShapeColor { Red, Green, Blue };
	void Draw(ShapeColor Color = Red) const		//이제 비가상 함수
	{
		DoDraw(Color);
	}

private:
	virtual void DoDraw(ShapeColor Color) const = 0;	// 진짜작업이 이루어지는 함수
};

class Rectangle2 : public Shape2
{
public:
	//...
private:
	virtual void DoDraw(ShapeColor Color) const;
};

// 비가상 함순는 파생 클래스에서 오버라이드 되면 안되기 때문에 위와 같이 설계하면 Draw함수의 Color 매개변수에 대한 기본값을 Red로 고정 시킬 수 있다.