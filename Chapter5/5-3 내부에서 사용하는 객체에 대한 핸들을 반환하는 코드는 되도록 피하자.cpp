#include"pch.h"
/*
	사각형을 사용하는 어떤 프로그램을 만들고 있다고 가정하자. 사각형은 좌측 상단과 우측 하단의 꼭짓점
	두 개로 나타낼 수 있다. 이것을 추상화 한 Rectangle 클래스를 만드는데, 메모리를 아끼고 싶다는 생각에
	사각형의 영역을 정의하는 꼭짓점을 Rectangle 자체에 넣지 않고, 구조체에 넣어 가리키도록 한다면?
*/

class Point
{
public:
	Point(int _iX, int _iY);
	//...
	void SetX(int _iNewVal);
	void SetY(int _iNewVal);
	//...
};

struct RectData	//Rectangle에 쓰기 위한 점 데이터
{
	Point ulhc;		//"좌측 상단
	Point lrhc;		// 우측 하단
};

class Rectangle
{
public:
	Rectangle(Point Coord1, Point Coord2);
	Point& UpperLeft() const { return pData->lrhc; }
	Point& LowerRight() const { return pData->lrhc; }
private:
	std::tr1::shared_ptr<RectData> pData;
};

/*
	Rectangle  클래스의 사용자는 영역정보를 알아내어 쓸 때가 있을 것으므로, Rectangle 클래스에는
	UpperLeft와 LowerRight 정보가 멤버 함수로 있다. 그런데 Point 가 사용자 정의 타입인 것이 눈에 들어오면
	문득 "사용자 정의 타입은 참조자에 전달이 효과적"이라는 내용이 떠올라 두 멤버 함수는 스마트 포인터로
	물어둔 Point객체에 대한 참조자를 반환하는 형태로 만들어 졌다.

	컴파일에는 문제가 없다. 하지만 문제가 있다. 위 함수들은 멤버를 변경하지 못하도록 상수 함수로
	만들어졌는데, private멤버의 내부 데이터를 참조자로 반환하고 있다.
	즉, 저 함수를 이용해서 데이터를 바꿀 수 있다는 것.
*/
Point coord1(0, 0);
Point coord2(100, 100);
int main()
{
	const Rectangle Rec(coord1, coord2);	//Rec는 (0,0)부터 (100,100)의 영역에 있는 상수 Rectangle 객체
	Rec.UpperLeft().SetX(50);	-// 이제 Rec은 (50,0)부터 (100,100)의 영역에 있게 된다.
}
/*
	우리는 여기서 두 가지 교훈을 얻을 수 있다.
	1. 클래스 데이터 멤버는 아무리 숨겨봤자 그 멤버의 참조자르 반홚하는 함수들의 최대 접근도에 따라 캡슐화 정도가 정해짐
	   ulhc와 lrhc는 private로 선언이 되어 있지만 실질적으로는 public멤버인셈.
	2. 어떤 객체에서 호출한 상수 멤버 함수의 참조자 반환 값의 실제 데이터가 그 객체의 바깥에 저장되어 있다면
	   이 함수의 호출부에서 그 데이터의 수정이 가능하다.

	지금 예시는 참조자이지만 참조자, 포인터, 반복자 모두 동일하다.
	어찌됐건 저 세 가지는 모두 "핸들"이고 이 것 반환하게 만들면 캡슐화가 무너질 우려가 있다
	위의 예시처럼 심지어는 상수 함수에서도 변경이 된다.

	객체의 "내부요소"에는  데이터 멤버뿐만 아니라 일반적인 수단으로 접근이 불가능한 멤버 함수도
	객체의 "내부요소"이다. 따라서 이들 모두의 핸들을 반환하지 않도록 조심해야 한다.

	그렇다면 이들 멤버 함수를 수정하여 문제를 해결 한 코드를 보자
*/

class Rectangle2
{
public:
	Rectangle2(Point Coord1, Point Coord2);
	const Point& UpperLeft() const { return pData->lrhc; }
	const Point& LowerRight() const { return pData->lrhc; }
private:
	std::tr1::shared_ptr<RectData> pData;
};
/*
	간단히 반환 타입에 const만 붙여주면 된다. 호출부에서 객체를 수정하지 못하게 컴파일러가 막아줄 것.

	또한 캡슐화 부분에서도 "의도적인 완화"이기 때문에 느슨한 한편 제한을 두었다는 것에 의미를 둘 수 있다.

	하지만 아직 찝찝한 부분이 있다. 두 함수가 내부 데이터에 대한 핸들을 반환하는 것이기 때문에
	무효참조 핸들 문제가 발생할 수 있다. (핸들을 따라 갔을 때 실제 객체의 데이터가 없는 것)
	이 증상은 함수가 객체를 값으로 반환할 경우에 흔하게 발생한다.

	다음 GUI객체의 사각 테두리 영엉을 Rectangle 객체로 반환하는 함수가 있다고 가정하자
*/

class GUIObject {/*...*/ };
const Rectangle BoundingBox(const GUIObject& obj);	//Rectangle 객체를 값으로 반환

//이 상태에서 사용자가 이 함수를 사용한다고 생각해보자

GUIObject* pgo;		//pgo를 써서 임의의 GUIObject를 가리키도록 함
//...
const Point* pUpperLeft = &(BoundingBox(*pgo).UpperLeft());		// pgo가 가리키는 GUIObject의 사각 테두리 영역으로부터 좌측 상단
																// 꼭짓점의 포인터를 얻음

/*
	마지막 줄 코드를 보면 BoundingBox함수를 호출하면 Rectangle 임시객체가 만들어진다.
	이 임시 객체에 대해 UpperLeft가 호출될텐데, 이 호출로 인해 임시 객체의 Point객체중 하나에 대한 참조자가 나온다

	마지막으로 이 참조자에 &연산 건 결과 값(주소)이 pUpperLeft에 대입이 된다.
	하지만 문제가 있는데 저 마지막 문장이 끝날 무렵이면 임시객체가 사라지게 된다는 것이다. 그럼 그 내부에
	Point객체들도 덩달아 없어지니 pUpperLeft가 가리키는 객체는 날아가고 없게 되는 것.

	이런 문제 때문에 참조자를 반환하는 일은 피해야 하는 것이다. const를 붙이냐 마냐의 문제가 아닌
	참조자를 반환한다는 그 자체가 문제인 것. 함수 밖으로 빠져나간 핸들은 그 자신이 참조하는 객체보다
	오래 살게 될 수 있다는 점을 꼭 기억하자.

	물론 "절대적"인 것은 아니다. 가령 operator[] 연산자는 string 이나 vector등의 클래스에서 개개의 원소를
	참조할 수 있는 용도로 제공되는데, 내부적으로  해당 컨테이너에 들어있는 개개의 원소 데이터에 대한 참조자를 반환한다.
	하지만 이 것이 예외적인 것이라는 점을 기억하자
*/