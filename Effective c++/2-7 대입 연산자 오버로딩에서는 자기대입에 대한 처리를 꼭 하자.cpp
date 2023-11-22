//자기대입(Self assignment) : 아떤 객체가 자기 자신에 대해 대입 연산자를 적용하는 것.

class Widget {/*...*/ };

Widget w;
// ...
w = w;

//완전 위의 예시처럼 보이는 코드가 아닌 생각보다 보이지 않게 이런 경우가 생길 수 있다.
a[i] = a[j];	//i와 j가 같은 값을 가지면 자기대입문이 됨.
*px = *py;		//px와 py가 가리키는 대상이 같다면 자기대입문이 됨.

/*
	이런 명확하지 않은 자기 대입문이 생기는 이유는 여러 곳에서 하나의 객체를 참조하는
	중복참조(aliasing) 때문이다.	따라서 같은 타입으로 만들어진 객체 여러개를 참조자나 포인터로 물어놓고
	동작하는 코드를 작성할 때는 자기 대입의 경우를 고려해야 한다.
	이를 방지하기 위해, 같은 타입의 객체라도 다른 타입으로 선언하는 방법도 있다.
	파생 클래스 타입의 객체를 참조하거나 가리키는 용도로 기본 클래스의 참조자나 포인터를 사용하면 되기 때문에.
*/
class Base {};
class Derived : public Base {	};
void DoSomething(const Base& rb, Derived* pd);	//rb와 pd는 원래 같은 객체였을 수도 있다.

/*
	우리는 자원 관리 용도로 항상 객체를 만들어야 할 것이고, 니 자원 관리 객체들이 복사될 때 나름대로 잘동작하도록
	코딩하게 될 것. 바로 이때 대입 연산자를 조심해야한다. 대입 연산자는 신경쓰지 않아도 자기대입에 대해 안전하게 동작해야 한다.
	하지만 아래 예시를 보자
*/

class Bitmap {/*...*/ };

class Widget
{
private:
	Bitmap* pb;	//힙에 할당한 객체를 가리키는 포인터.
	Widget& operator=(const Widget& rhs);
};

Widget& Widget::operator=(const Widget& rhs)	// 안전하지 안게 구현된 operator=
{
	delete pb;						//현재의 비트맵 사용을 중지
	pb = new Bitmap(*rhs.pb);					//이제 rhs의 비트맵을 사용하도록 만듬.
	return *this;				// 대입 연산자이니 *this를 반환
}
/*
	이 코드에서 문제는 *this와 rhs가 같은 객체일 가능성이 있다는 것이다.
	만약 둘이 같은 객체라면 delete가*this뿐만 아니라 rhs에도 적용되어 버린다.
	이전의 해당 에러에 대한 대책으로는 operator=의 첫머리에 일치성 검사를 통해 자기대입을 점검하는 것이였다.

	해결법 1
*/
Widget& Widget::operator=(const Widget& rhs)
{
	if (this == &rhs) return *this;
	//...
	return *this;
}
/*
	하지만 이 방법은 예외 안정성에 대해서는 여전히 문젯거리를 안고 있다. 특히 new Bitmap이라는 표현식에서
	동적 할당 메모리 부족이나 Bitmap 클래스 복사 생성자가 예외를 던진는 등의 예외가 발생하게 되면
	Widget 객체는 결국 삭제된 Bitmap을 가리키는 포인터를 껴안고 남게된다. 만약 이렇게 되면
	delete를 안전하게 적용할 수도 없고, 안전하게 읽는 것조차 불가능하다.

	다만, 예외에 안전하게 구현한다면, 자기 대입에도 안전한 코드가 나오게 된다. 즉 예외 안전성에만 집중하면
	자기 대입문제는 무시하더라도 무사히 넘어갈 확률이 높아진다는 것. (예외 안전성과 관련된 이야기는 뒤에 다시 나온다)
	"많은 경우에 문장 순서를 바꾸는 것만으로도 예외에 안전한 코드가 만들어진다"는 법칙을 보자.

	해결법 2
*/

Widget& Widget::operator=(const Widget& rhs)
{
	Bitmap* pOrig = pb;			// pb를 어딘가에 기억해둔다
	pb = new Bitmap(*rhs.pb);	// pb가 *pb의 사본을 가리키게 만든다.
	delete pOrig;			//원래의 pb를 삭제

	return *this;
}

/*
	위 코드는 이제 예외에 안전하다. new Bitmap에서 예외가 발생하더라도 pb(그리고 이 포인터가 들어있는 widget)는
	변경되지 않은 상태가 유지되기 때문.
	거기다 일치성 검사가 없음에도 자기 대입 현상을 처리하고 있다. 원본 비트맵을 복사해 놓고, 사본을 포인터가 가리키게 만든 후,
	원본을 삭제하는 순서로 실행되기 때문.

	이외에 다른 해결 방법이 한 가지 더 있는데, 복사 후 맞바구기(copy and swap)이라고 알려지 기법.
	이 기법은 예외 안전성과 관련해 밀접한 관련이 있기 떄문에 나중에 자세히 설명될 것.

	해결법 3
*/

class Widget
{
	//...
	void Swap(Widget& rhs);		//*this의 데이터와 rhs의 데이터를 맞바꿈
	Widget& operator=(const Widget& rhs)
	{
		Widget Temp(rhs);		//rhs의 데이터에 대해 사본을 하나 만듧
		Swap(Temp);				//this의 데이터를 그 사본의 것과 맞바꿈.
		return*this;
	}
};
// 이를 조금 다르게 구현한다면
// 1. 클래스의 복사 대입 연산자는 call by value가 가능하다는 것
// 2. call by value수행시 전달된 대상의 사본이 생긴다는 점을 이용하는 것
// 해결법 3-2

Widget& Widget::Operator = (Widget rhs)		//rhs는 원래 객체의 사본
{
	swap(rhs);					// *this의 데이터를 이 사본의 데이터와 맞바꿈.

	return *this;
}
/*
	위 코드는 call  by value에 의해 사본이 생긴다는 점을 이용해 객체를 복하하는 코드를 매개변수로 옮긴 것.
*/