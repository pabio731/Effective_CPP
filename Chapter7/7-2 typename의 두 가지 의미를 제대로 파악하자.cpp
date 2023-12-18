#include "pch.h"

/*
	우리가 템플릿을 만들 때,
	template<typename T> class Widget
	template<class T> class Widget
	두 가지 모두 가능하다.
	그리고 두 경우 모두 차이는 없고 그저 두 가지 선언 모두 가능하다.
	하지만 그렇다고 typename이 class 와 동의어 같은 것은 아니다.
	분명 typename만을 써야하는 경우가 있다.

	이를 알기 위해서는 템플릿 안에서 우리가 참조할 수 있는 이름의 종류가 두 가지라는 것부터 알아야 한다.

	함수 템플릿이 있고, 이 템플릿은 STL과 호환되는 컨테이너를 받아들이고, 이 컨테이너에 담기는 객체는
	int에 대입할 수 있다 가정하자. 그리고 이 템플릿이 하는 일은 담긴 원소중 두 번째 값을 출력하는 것이다.
*/

template <typename C>
void Print2nd(const C& container)
{
	if (container.size() >= 2)
	{
		C::const_iterator iter(container.begin());

		++iter;						// 컨테이너의 첫 번쨰 원소에 대한 반복자를 얻고 iter를
		int iValue = *iter;			// 두 번째 원소로 옮기고 다른 int타입 변수에 값을 옮기고
		std::cout << iValue;		// 출력
	}
}	// 컴파일이 되지 않는다

/*
	 위 함수안에서 두 가지 변수 iter와 iValue를 잘보자.
	 iter의 타입은 C::const_iterator인데, 템플릿 매개변수인 C에 따라 달라지는 타입이다.
	 이렇게 템플릿 매개변수에 종속된 것을 가리켜 "의존 이름"이라고 한다.
	 이런 의존 이름이 클래스 안에 중첩되어 있는 경우도 있는데
	 위의 C::const_iterator가 그러하다. (중첩 의존 이름이라 하자)

	 다음으로 iValue는 int타입이고, 템플릿 매개변수가 어떻든 상관 없는 타입 이름이다.
	 이런 이름은 "비의존 이름"이라고 한다.

	 그런데 코드 안에 중첩 의존 이름이 있을 때, 문제가 발생할 수 있다.
	 바로 컴파일러의 구문 분석에 애로사항이 생긴다는 것이다.
	 더 극단적인 상황을 가정해서 아래 코드를 보자
*/
template <typename C>
void Print2nd2(const C& Container)
{
	C::const_iterator* X;
}
/*
	언뜻 보면 C::const_iterator에 대한 포인터 지역변수 X를 선언한 것처럼 보인다.
	하지만 이것은 우리가 C::const_iterator가 타입이라는 사실을 우리 인간은 알기 때문이다.
	만약 저것이 타입이 아니라고 가정한다면 위의 코드는 그저
	C::const_iterator와 X를 곱셈 연산한 것에 불과하고, 실제로 컴파일러가 그렇게 작동할 것이다.
	당연히 const_iterator라는 이름의 변수를 선언하는 사람은 없겠지만 중요한 것은
	컴파일러는 C::const_iterator가 타입이라는 것을 모른다는 사실이다.
	그리고 C++는 모호성을 해결하기 위해 쓰는 규칙이 있는데, 바로 구문 분석기는 템플릿 안에서
	중첩 의존 이름을 만나면 기본적으로 타입이 아닌 것으로 해석한다는 것이다.

	해결 방법은 간단하다. 위 C::const_iterator가 타입이라는 사실을 컴파일러에게 알려주는 것이다.
*/

template <typename C>
void Print2nd3(const C& Conatainer)
{
	if (Container.size() >= 2)
	{
		typename C::const_iterator iter(Conatainer.begin());
	}
}

/*
	typename 키워드는 중첩 의존 이름만 식별하는 데 써야한다.  그 외에는 쓰면 안된다.
	예를 들어서 어떤 컨테이너와 컨테이너 내의 반복자를 한꺼번에 받아들이는 함수 템플릿을 다음과 같이 만든 경우이다.
*/
template<typename C>
void f(const C& Container					// typename 쓰면 안된다.
	, typename C::const_iterator iter);		// typename 꼭 써야 한다.

/*
	C는 중첩 의존 타입 이름이 아니기 때문에(의존적인 어떤 것도 C를 품고 있지 않기 때문에)
	typename을 앞에 붙이면 안된다.

	그런데 이 규칙에 예외가 있다. 이 예외는
	중첩 의존 타입 이름이 기본 클래스의 리스트에 있거나 멤버 이니셜라이저내의 기본 클래스 식별자로서
	있는 경우에는 typename을 붙여 주면 안된다
*/

template<typename T>
class Derived : public Base<T>::Nested		//상속되는 기본 클래스 리스트 -> typename 쓰면 안된다.
{
	explicit Derived(int x)
		: Base<T>Nested(x)					// 멤버 이니셜라이저에 있는 기본 클래스 식
	{
		typename Base<T>::Nested Temp;
	}
};

/*
	typename에 관한 마지막 예시로 반복자를 매개변수로 받는 어떤 함수 템플릿을 만들고 있다 가정하자.
	반복자가 가리키는 객체의 사본을 Temp라는 이름의 지역 변수로 만들어 놓고 싶은 상황이다.
*/
template<typename IterT>
void WorkWithIterator(IterT iter)
{
	typename std::iterator_traits<IterT>::value_type Temp(*iter);
}
/*
	std::iterator_traits<IterT>::value_type 은 그저 C++표준의 특성 정보(traits) 클래스를
	사용한 것이며, 의미는 "IterT 타입의 객체로 가리키는 대상의 타입"이라는 뜻이다.
	즉 IterT객체가 가리키는 것과 똑같은 타입의 지역변수(Temp)를 선언한 후, iter가 가리키는 객체로
	그 Temp를 초기화 하는 문장이다.
	여기서 std::iterator_traits<IterT>::value_type은 value_type이 iterator_traits<IterT>안에
	중첩되어 있고, IterT는 템플릿 매개변수이기 때문에 중첩 의존 이름이다. 따라서 typename을 써야한다.

	추가로 위의std::iterator_traits<IterT>::value_type가 너무 길어 typedef로 만들게 된다면 어떻게 해야할까?
	일단 특성정보 클래스에 속한 value_type등의 멤버이름에 대해 typedef이름을 만들 때는 그 멤버 이름과
	똑같이 짓는 것이 관례이다
 */
template<typename IterT>
void WorkWithIterator2(IterT iter)
{
	typedef typename std::iterator_traits<IterT>::value_type value_type;
	value_type Temp(*iter);
}
/*
	typedef와 typename형태로 나란히 있는 것은 그저 중첩 의존 타입 이름을 찹조하는 데
	지켜야할 규칙 때문에 생긴 부산물일 뿐이다. 논리적으로 하자가 없으니
	문제도 없다.

	다만 typename에 관해서 알아두어야 할 것은 컴파일러마다 typename에 관한
	규칙을 얼마나 강조하는지는 꽤 많이 다를 수 있다는 점이다.
	어떤 컴파일러는 typename이 필요한 곳에 쓰지 않아도 잘 받아들이고,
	어떤 컴파일러는 typename이 허용되지 않는 곳에 쓰여도 문제없이 넘어가기도 한다.
	따라서 typename에 대해 잘 이해해두고, 상황에 맞춰 잘 사용하자.
*/