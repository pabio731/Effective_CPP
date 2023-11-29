/*
	상수 객체 참조를 통한 전달 방식이 많은 부분에서 좋다고해서 값에 의한 전달을 나쁘게 보면 안된다.
	그러다 존재하지 않는 객체의 참조를 전달하는 실수를 할 수 있다.

	다음 코드는 유리수를 나타내는 클래스이고, 두 유리수를 곱하는 멤버함수가 포함되어 있다.
*/

class Rational
{
public:
	Rational(int iNumerator = 0, int iDenomunator = 1);
	// ...

private:
	int n, d;	//분자와 분모

	friend const Rational operator*(const Rational& lhs, const Rational& rhs);	//곱셈 결과를 값으로 반환하도록 선언
};
/*
	하지만 Rational 객체의 생성과 소멸에 들어가는 비용을 생각해야 한다.
	그럼 참조자를 반환한다면? 비용부담은 덜겠지만 다시 한번 생각해볼 문제이다.

	참조자에 대해 생각해보자. 참조자는 이미 존재하는 객체에 대한 "또 다른 이름"이다.
	함수가 객체의 참조차를 반환한다면 이 함수가 반홚는 참조자는 반드시 이미 존재하는
	Rational객체의 참조자여야 한다.

	그럼 위 함수에서 반환될 객체는 어디 있는가?
*/

Rational a(1, 2);	// a = 1/2
Rational b(3, 5);	// b = 3/5
Rational c = a * b;	//c는 3/10?

/*
	c가 호출되고 자동으로 객체가 생기고 그 객체의 데이터가 3과 10으로 초기화 될거라 생각하면 안된다.

	함수에서 새로운 객체를 만드는 방법은 딱 두 가지뿐이다. 스택에 만드는 것과 힙에 만드는 것.

	먼저 스택에 만든다는 생각 -> 지역 변수를 정의
*/
const Rational operator*(const Rational& lhs, const Rational& rhs)
{
	Rational result(lhs.n * rhs.n, lhs.d * rhs.d);
	return result;
}
/*
	이 연산자 함수는 result에 대한 참조자를 반환하는데, result는 지역 객체이니 당연히
	함수가 끝날 때 소멸된다. 따라서 온전한Rational객체를 반환하지 않는다.
	정확하게는 이전에는 Rational이었던 메모리 공간을 가리키게 되는 것.

	그럼 다음으로 힙에 객체를 생성
*/
const Rational operator*(const Rational& lhs, const Rational& rhs)
{
	Rational* result = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);
	return *result;
}
/*
	이번의 문제는 new로 메모리를 할당받은 이 객체를누가 delete해줄것인가?
*/
Rational w, x, y, z;
w = x * y * z;			// operator*(operator*(x,y),z) 와 같음.

/*
	여기서는 한 문장안에서 오퍼레이터가 두 번 호출되기 때문에 new가 두번이니, delete도 두번 필요하다.
	하지만 operator*로부터 반환되는 참조자뒤에 숨겨진 포인터에 대해서는 사용자가 접근할 방법이 없다.

	스택 기반이든, 힙 기반이든 모두operator에서 반환되는 결과는 반드시 생성자를 꼭 한 번 호출했을 것.
	하지만 목표는 쓸데없는 생성자 호출을 피하는 것!

	그렇다면 이를 위해서 Rational 객체를 정적 객체로 함수 안에 정의해놓고 이것의 참조자를 반환하는 식으로 해보자
*/

const Rational operator*(const Rational& lhs, const Rational& rhs)
{
	static Rational result;		//반활할 참조자가 가리킬 정적 객체

	result.n = lhs.n * rhs.n;
	result.d = lhs.d * rhs.d;//...
	return result;
}
/*
	정적 객체를 사용하는 설계는 항상 스레드 안정성 문제가 있다.
	하지만 이보다 더한 약점이 있는데 아래 코드를 보자
*/

bool operator==(const Rational& lhs, const Rational& rhs);
Rational a, b, c, d;
//...
if ((a * b) == (c * d))
{
	//두 유리수 쌍의 곱이 서로 같으면 필요한 처리
}
else
{
	// 다르다면 적절한 처리
}

/*
	위 코드의 조건은 항상  true를 반환하는 문제가 있다.
	각각의 oepator*안에 정의된 정적Rational 객체의 참조자가 반환되고, operator==가 비교하는 피 연산자는 operator*안의
	정적 Rational객체의 값, 그리고 또 operator* 안의 정적 객체의 값이다. 따라서 항상 true

	배열,벡터 모두 비용이나 최적화 면에서 다 문제가 있다.

	새로운 객체를 반환하는 함수를 작성하는 방법에는 정도가 있는데, 바로 "새로운 객체를 반환하게 만드는 것"이다.
	return Rational(....)
	물론 여기에도 생성과 소멸의 비용이 있지만 따지고 보면 동작에 지불되는 작은 비용일 뿐임.

*/