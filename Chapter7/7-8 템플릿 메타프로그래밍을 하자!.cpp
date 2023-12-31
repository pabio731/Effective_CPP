#include "pch.h"

/*
	템플릿 메타프로그래밍(template metaprogramming : TMP)은 컴파일 도중에 실행되는
	템플릿 기반의 프로그램을 작성하는 일을 말한다.
	템플릿 메타 프로그래밍은 C++컴파일러가 실행시키는 C++로 만들어진 프로그램이다.
	TMP프로그램이 실행을 마친 후에 그 결과로 나온 출력물이 다시 컴파일 과정을 거치기 때문.

	TMP에는 엄청난 강점이 두 가지가 있다.
	1. TMP를 쓰면 다른 방법으로는 까다롭거나 불가능한 일을 아주 쉽게 할 수 있다.
	2. TMP은 C++ 컴파일이 진행되는 동안에 실행되기 때문에, 기존 작업을 런타임에서 컴파일 타음으로 옮길 수 있다.

	위 강점으로 얻는 이점들이 있는데,
	먼저 에러의 발견을 런타임 도중이 아니라, 컴파일 도중으로 바꿀 수 있다는 것이다.
	다음은 모든 면에럿 효율적일 여지가 많다는 것이다.
	컴파일 타임에 동작을 다 하기 때문에 실행 코드가 작아지고, 실행시간도 짧아지며,
	메모리도 적게 잡아먹는 것.
*/

template <typename IterT, typename DistT>
void Advance(IterT& iter, DistT d)
{
	if (/*iter가 임의 접근 반복자*/)
		iter += d;							//임의 접근 반복자에 대해서는 반복자 산술 연산을 한다.
	else
	{
		if (d > 0) { while (d--) ++iter; }	// 그 외의 반복자에 대해서는 ++혹은 --연산의 반복 호출을 사용한다.
		else { while (d++) --iter; }
	}
}

// 이 유사 코드를 진짜 코드로 만드려면 typeid를 쓸 수 있다.
// 타입 정보를 꺼내는 작업을 런타임에 하겠다는 것

template<typename IterT, typename DistT>
void Advance2(IterT& iter, DistT d)
{
	if (typeid(typename std::iterator_traits<IterT>::iterator_category) ==
		typeid(std::random_access_iterator_tag)
		iter += d;//임의 접근 반복자에 대해서는 반복자 산술 연산을 한다.
	else
	{
		if (d > 0) { while (d--) ++iter; }	// 그 외의 반복자에 대해서는 ++혹은 --연산의 반복 호출을 사용한다.
		else { while (d++) --iter; }
	}
}

/*
	이전에 지적했듯, typeid를 쓰는 방법은 iterator_traits를 쓰는 방법보다 효율이 떨어진다.
	런타임에 타입점검이 이루어지고, 런타임에 타입 점검을 수행하는 코드는 어쩔 수 없이 실행 파일에 들어가야 하기 때문.
	특성정보 클래스를 이용하는 방법은 TMP를 이용하는 방법이다. if.. else문 처리를 타입에 따라서 컴파일 타임에 하기 때문에

	TMP는 그 자체로 튜링 완전성을 가진 것으로 알려져있다. 즉 범용 언어처럼 어떤 것이든 계산 할 수 있다는 것.
	TMP에서 if...else 조건문을 나타내는 데는 통사으이 if문이 아닌 템플릿 및 템플릿 특수화 버전을 사용한다.
	프로그래밍 언어 수준으로보면 이런 방법은 TMP의 어셈블리라고 할 수 있다.

	TMP의 동작원리를 엿볼 수 있는 부분의 예로 루프를 들 수 있다.
	사실TMP에는 반복 의미의 진정한 루프는 없기 떄문에 재귀를 사용해서 루프의 효과를 나타낸다.
	그런데 이 여기서 말하는 재귀는 우리거 원래 알던 그 재귀와는 다르다.
	TMP의 루프는 재귀 함수 호출을 만들지 않고 재귀식 템플릿 인스턴스화를 하기 때문.

	재귀식 템플릿 인스턴스화를 통한 루프 효과를 볼 수 있는 대표적인 예시인
	팩토리얼의 구현을 보자
*/

template <unsigned U>
struct Fatorial
{
	enum { value = n * Fatorial<n - 1>::value };
};

template<>
struct Fatorial<0>
{
	enum { value = 1 };
};

/*
	이렇게 FActorial<n>::value를 참조함으로써 n factorial을 바로 얻을 수 있다.

	이 코드에서 루프를 도는 위치는 Fatorial<n>의 내부에서 또 다른 템플릿 인스턴스인 Factorial<n-1>을
	참조하는 곳이다.

	추가로 구조체 안에는 value라는 이름의 TMP변수가 선언되어 있는데 이는
	나열자 둔갑술(enum hack)을 이용한 것이다.

	TMP는 루프 대신에 재귀식 템플릿 인스턴스화를 사용하기 때문에 꼬리에 꼬리를 물고 만들어지는 템플릿 인스턴스화 버전마다
	자체적으로 value의 사본을 갖게되고, 각각의 value에는 루프를 한번 돌 때마다 만들어지는 그 값이 담기게 된다.

	C++프로그래밍에서 TMP가 실력을 발휘하는 예를 들어보면 세 군데이다.

	1. 치수단위(dimensional unit)의 정확성 확인
		과학 기술 분야의 응용프로그램을 만들 떄는 무엇보다도 치수 단위가 똑바로 조합되어야 한다.
		TMP를 사용하면 프로그램 안에서 쓰이는 모든 치수 단위의 조합이 제대로 됐는지를 컴파일 동안에 맞춰볼 수 있다.
		(선행 에러 탐지의 가장 대표적 사례) 재밌는 점은 분수식 지수 표현이 가능하다는 것.
		이런 표현이 가능하려면 컴파일 도중 분수의 약분이 되어야 한다.

	2. 행렬 연산의 최적화
		다음 코드를 보자
*/
template<typename TypeT, size_t U>
class SquareMatrix {};

typedef SquareMatrix<double, 10000> BigMatrix;
BigMatrix m1, m2, m3, m4, m5;
//...
BigMatrix result = m1 * m2 * m3 * m4 * m5;

/*
		곱셉 결과를 보통 방법으로 계산하려면 네 개의 임시 행렬(임시 객체)이 생겨나야 한다
		operator*를 한 번씩 호출할 때마다 반환되는 결과로 생기는 것.
		하지만 TMP를 응요한 고급 기술인 표현식 템플릿을 이용하면 덩치 큰 임시 객체를 없애는 것은 물론
		루프까지 합쳐버릴 수 있다.

	3. 맞춤식 디자인 패턴의 구현
		전략 패턴, 감시자 패턴,방문자 패턴 등의 디자인 패턴은 그 구현 방법이 여러가지일 수 있다.
		TMP를 이용해 정책 기반 설계(Policy-based design)을 사용하면 따로따로 마련된 설계상의 선택을 나타내는 템플릿을
		만들어낼 수 있게 됩니다. 이를 통해 만들어진 정책 템플릿은 사용자의 취향에 맞는 동작을 갖는 패턴으로 구현되는 쓰인다.
		이는 생성식 프로그래밍(generative programming)의 기초이다.

*/