#include "pch.h"

/*
	앞서 명시적 타입 변환이 되도록 만들기 위해서는 비멤버 함수밖에 방법이 없다고 배운적이 있다.

	여기서 약간의 조미료를 더해서 당시 예시로 썻던 유리수 클래스 Rational과 함수 operator*를
	템플릿으로 만들어 보자
*/

template <typename T>
class Rational
{
public:
	Rational(const T& Numerator = 0, const T& Denominator = 0);
	const T Newmerator() const;
	const T Denominator() const;
};

template <typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {/*...*/ }

/*
	혼합형 수치 연산이 필요하기 때문에 operator를 이전에 배운대로 비멤버 함수로 했고,
	여기서 템플릿화만 했다. 그런데 문제가 있다.
*/

Rational<int> OneHalf(1, 2);	// 템플릿이라는 것만 제외하면 같은 코드
Rational<int> Result = OneHalf * 2;	//그런데 여기서 컴파일 오류가 생긴다. (피연산자 형식 일치 오류)

/*
	템플릿 버전과 비템플릿 버전의 차이는 바로 컴파일러가 호출하려고 하는 함수가 무엇인지 알고 있느냐는 것이다.
	위의 경우는 컴파일러가 어던 함수를 호출하려고 하는지 알 수 있는 방법이 전혀 없다.

	일단 OnHalf는 operator*의 첫 번째 매개변수가 Rational<T> 타입이고 operator에 넘겨지는 매개변수 또한
	Rational<int>이기 떄문에 컴파일러가 쉽게 유추한다. 하지만 두 번재 매개변수가 int타입인데 컴파일러가
	T의 정체를 추론하지 못한다. 위의 코드에서 생성자가 explicit이 아닌데, 그렇다고 생성자를 이용해서
	암시적 타입 변환이 가능하지도 않다. 템플릿 인자 추촌 과정에서는 암시적 타입 변환이 고려되지 않기 때문.

	이 방법을 해결하기 위해서는 위 operator* 함수 템플릿을 friend 함수로 선언 하는 것이다.
*/

template<typename T>
class Rational2
{
public:
	friend const Rational2 operator*(const Rational2& lhs, const Rational2& rhs);
};
template<typename T>
const Rational2<T> operator*(const Rational2<T>& lhs, const Rational2<T>& rhs) {/*...*/ }

/*
	이제 OneHalf 객체가 Rational<int>타입으로 선언되면 Rational<int> 클래스가 인스턴스로 만들어지고
	그 과정의 일부로서 Rational<int> 타입의 매개변수를 받는 프렌드 함수인 operator*도 자동으로 선언된다.

	이전과 다르게 지금은 클래스 내부에 함수 템플릿이 아니라 함수가 선언이 된 것이므로 컴파일러가
	호출문에 대해서 암시적 변화 함수를 적용할 수 있게 되는 것.

	하지만 추가로 해야 할 일이 남았는데 위 코드를 가지고 다시 컴파일을 해보면 링크가 되지 않는다.
	이 문제는 operaotr* 가 템플릿 클래스 내부에 선언만 되어 있는 것이지, 정의 되어 있는 것이 아니다.

	따라서 간단히 선언과 정의를 붙여줘 해결할 수 있다.
*/

template<typename T>
class Rational3
{
public:
	friend const Rational3 operator*(const Rational3& lhs, const Rational3& rhs)
	{
		return Rational3(lhs.Numerator() * rhs.Numerator()),
			lhs.Denominator()* rhs.Denominator());
	}
};

/*
	이렇게 문제가 해결된다.

	여기서 재밌는 부분은 프렌드 함수를 선언하긴 했지만 클래스의 public접근 영역이 아닌
	다른 접근 허용 영역에 접근 하기 위해서 프렌드 함수를 쓴 것이 아니라는 점이다.
	상황에 따라서는 private에 접근하기 위해서 프렌드 함수를 쓰는 것이 아닌
	다른 경우도 있을 수 있다는 것.

	추가로 위의 인라인 함수로 인한 영향을 받지 않기 위해서 쓸 수 있는 방법은
	프렌드 함수만 도우미만 호출하게 만드는 것이다.
*/

template <typename T> class Rational4;

template <typename T>
const Rational4<T> DoMultiply(const Rational<T>& lhs, const Rational<T>* rhs);

template<typename T>
class Rational4
{
public:
	friend Rational4<T> operator* (const Rational<T>& lhs, const Rational<T>* rhs)
	{
		return DoMultiply(lhs, rhs);		// 프렌드 함수가 도우미 함수를 호출하게 만듦
	}
};

/*
	추가로 대다수의 컴파일러에서 템플릿 정의를 헤더 파일에 같이 넣을 것을 강제하니
	그 부분을 잊지 않고 같이 만들어주면된다.
	DoMultiply는 템플릿으로써 혼합형 곱셉을 지원하지 않겠지만
	해당 템플릿은 operator* 만 사용할 것이고, operator*가
	혼합형 연산을 지원하기 때문에 상관 없다.

*/