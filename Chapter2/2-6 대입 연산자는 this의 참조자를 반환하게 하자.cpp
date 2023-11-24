/*
	c++대입 연산은 여러개가 시슬처럼 엮을 수 있다.
	ex) x=y=z=15;

	우측 연관(right-associative)연산
	위 코드가 x = (y=(z=15)); 로 분석할 수 있다.
	이렇게 대입 연산이 사슬처럼 엮이려면 대입 연산자가 좌변 인자에 대한 참조자를 반환하도록 구현해야 한다.
	해당 구현은 일종의 관례.
	*/
class Widget
{
public:
	Widget& operator=(const Widget& rhs)
	{
		//...
		return *this;	// 좌변 객체의 참조자를 반환
	}
};

//	이는 단순 대입형 연산자 말고도 모든 형태의 대입 연산자에서 지켜져야 한다.

class Widget
{
public:
	Widget& operator+=(const Widget& rhs)
	{
		//...
		return *this;
	}

	Widget& operator=(int rhs)
	{
		//...
		return *this;	//대입 연산자의 매개변수 타입이 일반적이지 않은 경우에도 마찬가지
	}
};

/*
	관례이기 때문에 컴파일이 되지 않는 등의 문제는 없다. 하지만 모두 따르고 있고, 표준 라이브러리 속한 모든 타입에사도
	따르고 있다는 점은 무시 못할 것.

*/