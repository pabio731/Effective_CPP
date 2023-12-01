/*
"클래스에서 암시적 타입변환을 지원하는 것은 일반적으로 못된 생각이다"라는 것에도 예외가 있는데
바로 숫자 타입을 만들 때이다. 만약 유리수를 나타내는 클래스가 있다면, 정수에서 유리수로
암시적 타입 변환을 허용하자고 충분히 생각할 수 있고, C++의 기본 타입 변환도 그렇게 한다.
그렇다면 아래 코드를 보자
*/

class Rational
{
public:
	Rational(int iNumerator = 0, int iDenominatir = 1);		//암시적 변환을 허용하기 위해 explicit을 붙이지 않음
	int Numerator() const;		//분자 및 분모에 대한 접근용 함수
	int iDonominator() const;
};

/*
	유리수를 나타내는 클래스이니 사칙연산을 기본적으로  지원하고 싶을 것이다.
	operator* 함수를 유리수의 곱셈은 유리수와 관련 있으니 멤버 함수로 하는게 좋겠다는 생각을 해(4-6 의 내용을 고려하지 않고)
	코드를 짰다고 가정하자
*/

class Rational
{
public:
	//...
	const Rational operator*(const Rational& rhs) const;
};
// 이제 유리수의 곱셈이 잘 될것이다.

Rational OneEighth(1, 8);
Rational OneHalf(1, 2);

Rational Result = OneEighth * OneHalf;	// 문제 없음
Rational Result = Result * OneEighth;	// 문제 없음.

//그럼 이제 혼합형 연산은?? int * double 같은...

Rational Result = OneHalf * 2;