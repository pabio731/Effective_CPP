#include"pch.h"

/*
	Swap 함수는 예외 안전성에 없어서는 안될 감초로, 자기대입에 대처하기 위한 대표적인 매커니즘이다.
	따라서 Swap을 어떻게 구성하냐가 굉장히 중요하다. 쓸만한 Swap함수를 만드는 방법에 대해 생각해보자

	기본적으로 Swap동작을 위해 표준 라이브러리의 Swap알고리즘을 쓰는데, 이 모습을 보면 아주 일반적이다
*/

//namespace std {
//	template <typename T>
//	void swap(T& a, T& b)
//	{
//		T tmp(a);
//		a = b;
//		b = tmp;
//	}
//}

/*
	위 코드를 보면 알겠지만 복사만 제대로 지원하는 타입이기만 하면 어떤 타입의 객체이든
	맞부꾸기 동작을 수행해준다.

	하지만 위 코드를 보면 한 번 호출에 3번의 복사가 일어난다.
	타입에 따라서는 이런 사본이 정말 필요없는 경우도 있는데, 이 경우는 매우 비효율적이다.

	이런 대표적인 예시를 보여주는 타입은 포인터이다.
	이 개념을 통해 많이 쓰는 기법이 "pimpl(Pointer to Implementation)"이라는 것이다.
*/

class WidgetImpl
{
public:
	//...
private:
	int a, b, c;
	std::vector<double> v;	// 아마도 많은 데이터를 가져 복사 비용이 높은 벡터
};
//
//class Widget				//pimpl 관용구를 사용한 클래스
//{
//public:
//	Widget(const Widget& rhs);
//
//	Widget& operator=(const Widget& rhs)		//Widget을 복사하기 위해, 자신읜 WidgetImpl 객체를 복사한다.
//	{
//		//...
//		*pImpl = *(rhs.pImpl);
//	}
//private:
//	WidgetImpl* pImpl;		// Widget의 실제 데이터를 가진 객체에 대한 포인터
//};

/*
	이렇게 만들어진 Widget객체를 우리가 맞바꾼다 생각할때는, pImpl포인터만 살짝 바꾸면 된다.
	하지만 표준 Swap알고리즘은 이 부분을 알 수가 없다.
	똑같이 Widget객체를 3개 복사하고, WidgetImpl 객체 3개도 복사할 것이다.

	따라서 조금 Swap 함수에 무언가를 알려줘 조금 손을 보자.
	내부의 포인터만 바꿀라는 방법으로 std::swap을 Widget에 대해 특수화 하는것.
*/

//namespace std
//{
//	template<>
//	void swap<Widget>(Widget& a, Widget& b)	 // T가 Widget일 경우에 대해 특수화 한것 (아직 컴파일 안됌)
//	{
//		swap(a.pImpl, b, pImpl);
//	}
//}

/*
	일단 시작부의 template<> 는 이 함수가 std::swap의 완전 템플릿 특수화 함수라는 것을 컴파일러에게
	알려주는 부분이다. 그리고 함수 이름뒤에 <Widget>은 T가 Widget일 경우에 대한 특수화라는 것을 알려주는 부분
	다시 말해 타입이 무관한 Swap템플릿이 Widget에 적용될 때는 위의 함수 구현을 사용해야 한다는 뜻.
	일반적으로 std네임스페이스의 구성요소는 함부러 변경할 수 없지만, 프로그래머가 직접 만든 타입에 대해
	표준 템플릿을 특수화 하는 것을 허용이 된다.

	하지만 위의 코드는 pImpl 변수가 private멤버기 때문에 컴파일이 안되는데, 일단 위 함수를
	프렌드 함수로 만드는 것은 표준 템플릿들에 쓰인 규칙과 어긋나므로 좋은 모양은 아니다.
	따라서 Widget 안에 Swap이라는 public 멤버 함수를 선언하고 그 함수가 실제 맞바꾸기를 수행도록 만든 후
	std::swap특수화 함수에게 그 멤버 함수를 호출하는 일을 맡김.
*/
class Widget
{
public:
	void swap(Widget& other)
	{
		using std::swap;

		swap(pImpl, other.pImpl);	//Widget을 맞바꾸기 위해, 각 Widget의 pImpl 포인터를 맞바꿈
	}

private:
	WidgetImpl* pImpl;
};

namespace std
{
	template<>
	void swap<Widget>(Widget& a, Widget& b)	// 템플릿의 특수화도 살짝 고침.
	{
		a.swap(b);	//Widget을 맞바꾸기 위해
	}
}

/*
	위 코드는 컴파일은 물론 기존의 STL 컨테이너와 일관성도 유지된다.
	public 멤버 함수 버전의 swap과 이 멤버 함수를 호출하는 특수화 함수 모두 지원하기도 하고.

	이제 가정을 하나 더 해보자.
	Widget과 WidgetImpl이 클래스가 아니라 클래스 템플릿으로 만들어져 있어서, WidgetImpl에 저장된
	데이터 타입을 매개변수로 바꿀 수 있다면 어떻게 되는가?
*/

template<typename T>
class WidgetImpl2 {};
template<typename T>
class Widget {};

/*
	swap 멤버 함수를 Widget에 넣는 정도는 어렵지 않지만 swap을 특수화 하는 것이 어렵다.

	nmaespace std
	{
		template<typename T>
		void swap<Widget<T>> (Widget<T>& a, Widget<T>& b)	/적법하지 않은 코드
		{
			a.swap(b);
		}
	}

	C++에서는 클래스 템플릿에 대해서는 부분 특수화를 허용하지만 함수 템플릿에 대해서는
	허용하지 않도록 정해져 있다. 따라서 컴파일이 되지 않는다.

	함수 템플릿을 "부분적으로 특수화"하고 싶다면 흔히 오버로드 버전을 하나 추가하는 것이다.

*/
namespace std
{
	template<typename T>
	void swap(Widget<T>& a, Widget<T>& b)
	{
		a.swap(b);
	}
}

/*
	일반적으로 함수 템플릿의 오버로딩은 별 문제 없지만, std는 조금 특별한 이름 공간이기 때문에
	규칙도 다소 특별하다.
	std내의 템플릿에 대한 완전 특수화는 가능하지만 std내에 새로운 템플릿을 추갈하는 것은 안된다.
	std의 구성요소는 C++표준화 위원회에 달려있기 때문에, 마음대로 할 수 없는 것이다.
	하지만 여기서 문제가 std의 영역을 침범하더라도 일단 컴파일까지는 거의 다 되고 실행도 되는데
	결과가 미정의 사항이라는 것이다. 따라서 std내에 아무것도 추가하지 않아야 한다는 것을 잊지말자.

	그렇다면 효율 좋은 템플릿 전용 버전의 swap을 사용하고 싶다면 어떻게 해야할까?
	방법은 간단하다. 멤버 swap을 호출하는 비멤버 swap을 선언해 놓되, 이 비멤버 함수를
	std::swap의 특수화 버전이나 오버로딩 버전으로 선언하지만 않으면 된다.
	예를 들어 Widget기능이 전부 WidgetStuff네임 스페이스에 들어있다고 가정하고 다음과 같이 하자
*/

namespace WidgetStuff
{
	//..								// 템플릿으로 만드러진 WidgetImpl과 기타 등등
	template<typename T>				// 전과 마찬가지로 swap이란 이름의 멤버 함수가 들어있음
	class Widget2 {};
	//...

	template<typename T>
	void swap(Widget2<T>& a, Widget2<T>& b)	 //비멤버 swap함수, std의 일부가 아님
	{
		a.swap(b);
	}
}

/*
	이제는 어떤 코드가 두 객체에 대해 swap을 호출하더라도 컴파일러는 C++의 이름 탐색 규칙에
	의해서 WidgetStuff 네임스페이스 안에서 Widget특수화 버전을 찾아낸다.

	이 방법은 클래스 템플릿뿐만 아니라 클래스에도 잘 통하므로, 잘 알아두면 좋다.
	일을하다보면 분명히 swap을 특수화해야 할 이유가 생기기 때문에 클래스 타입 전용의 swap이
	되도록 많은 곳에서 호출되도록 만들고 싶다면 그 클래스와 동일한 이름공간 안에 비멤버 버전의
	swap을 만들어 놓고 그와 동시에 특수화 버전도 준비해 두도록 하자.

	추가적으로, 위 모든 사항은 이름공간을 사용하지 않더라도 유효하다.
	(멤버  swap을 호출하는 비멤버 swap이 필요하다는 것)

	이제 다른 입장으로 생각해보자
	어떤 함수 템플릿을 만들고 있는데, 이 함수 템플릿은 실행중에 swap을 써서 두 객체의 값을 맞바꾸어야 한다고 가정하자
*/

template<typename T>
void DoSomething(T& obj1, T& obj2)
{
	//...
	swap(obj1, obj2);
	//..
}
/*
	이 부분은 어떤 swap을 호출해야 하는가?
	1. std::swap		:확실히 존재
	2. std::swap의 일반형을 특수화환 버전
	3. T타입 전용의 버전 :있을 수도, 없을 수도.

	타입 T버전이 있다면 그것을, 없다면 std::swap을 호출되도록 만들고 싶다면 어떻게 해야할까?
	다음 코드를 보자
*/

template <typename T>
void DoSomthing2(T& obj1, T& obj2)
{
	using std::swap;		//std::swap을 이 함수 안으로 끌어옹ㄹ 수 있도록 만드는 문장
	//...
	swap(obj1, obj2);		//T타입 전용의 swap을 호출
}

/*
	컴파일러가 swap호출문을 만나면 먼저 상황에 맞는 swap을 찾는다.
	C++의 이름 탐색 규칙을 따라서, 우선 전역 유효범위, 또는 타입 T와 동일한 이름공간 안에
	T전용의 swap이 있는지를 찾는다. 만약 T전용이 없다면 그 다음 순서를 밟는데,
	using std::swap이 있기 때문에 std의 swap을 쓴다.
	하지만 컴파일러는 std::swap보다 T전용 버전을 일반형 템플릿 보다 더 우선적으로 선택하도록 정해져 있기 때문에
	만약 std::swap 특수화 버전이 있다면 그 버전을 씀.

	swap에 std::swap같이 한정자만 붙여주지 않는다면 호출은 쉽다.

	정리
	1. 표준에서 제공하는 swap이 납득할만한 효율을 보인다면 그냥 써라. 문제도 없을 것이다.
	2. 효율이 충분하지 않다면 다음과 같이 하라
		2-1. 사용자 정의 타입으로 만들어진 두 객체의 값을 빛나게 빨리 맞바꾸는 함수를 swap이라는 이름으로 만들고
			 public멤버 함수로 두어라. 단, 절대 예외를 던져서는 안된다. 왜냐면  swap을 진짜 쓸모 있게 응용하는
			 방법들 중 클래스(클래스 템플릿)가 강력한 예외 안정성 보장을 제공하도록 도움을 주는 방법이 있기 때문.(멤버 버전만)

		2-2. 클래스 혹은 템플릿이 들어 있는 이름공간과 같은 이름 공간에 비멤버 swap을 만들고, 1번의 멤버 함수를 이 함수가 호출한다.

		2-3. 새로운 클래스를 만들고 있다면, 그 클래스에 대한 std::swap의 특수화 번전을 준비해둔다.
			 그리고 이 특수화 버전에서 swap멤버 함수를 호출하도록 만든다.

	3. 사용자 입자에서 swap을 호출할 때, swap을 호출하는 함수가 std::swap을 볼 수 있도록 using 선언을 반드시 포함시킨다.
	   그 다음 swap을 호출하되, 이름공간 한정자를 붙이지 않는다.

*/