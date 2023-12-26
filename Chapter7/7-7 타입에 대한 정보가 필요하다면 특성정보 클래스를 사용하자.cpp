#include "pch.h"

/*
	STL에는 기본적으로 컨테이너, 반복자, 알고리즘의 템플릿으로 구성되어 있지만
	이 외에 유틸리티라고 불리는 템플릿도 몇 개 들어 있다.
	한 예로 advence라는 이름의 템플릿이 있는데, 이 것이 하는 일은
	지정된 반복자를 지정된 거리만큼 이동 시키는 것이다.
*/

template <typename IterT, typename DistT>		// iter를 d 단위만큼 전진 시킨다.
void advance(IterT& iter, DistT d);				// d < 0이라면 iter를 d단위 만큼 후진 시킨다.

/*
	이 템플릿은 iter+d가 가능한 임의 접근 반복자에게는 의미 없어 보이지만
	그외 다른 반복자에게는 ++이나 --연산을 d번 적용해서 advence를 구현해야 한다.

	반복자 정리
	1. 입력 반복자(Input Iterator)
		- 입력 파일의 읽기 전용 포인터를 본떠 만들었고, istream_iterator가 대표적 입력 반복자
		- 전진만 가능, 한번에 한 칸만 이동, 자신이 가리키는 위치에서 읽기만 가능, 읽을 수 있는 횟수가 한번.

	2. 출력 반복자(Output Iterator)
		- 출력 파일의 쓰기 전용 파일 포인터를 본떠 만들엇고, ostream_iterator가 대표적 출력 반복자
		- 전진만 가능, 한번에 한 칸만 이동, 자신이 가리키는 위치에서 쓰기만 가능, 쓸 수 있는 횟수가 한번.

	위 두 가지의 반복자는 5대 반복자중 가장 기능이 떨어지며 단일 패스 알고리즘에만 제대로 쓸 수 있다.

	3. 순반향 반복자(Forwoard Iterator)
		- 입력, 출력 반복자가 하는 일은 기본적으로 다 할 수 있다.
		- 자신이 가리키는 위치에서 읽기와 쓰기 모두 가능. 그리고 여러번 가능
		- 다중 패스 알고리즘에 문제없이 사용 가능.
	STL은 원칙적으로 단일 연결 리스트를 제공하지 않지만 제공하는 몇몇 라이브러리가 있다.

	4. 양방향 반복자(Bidirectional Iterator)
		- 순방향 반복자에 뒤로 갈 수 있는 기능을 추가한 것.
		- STL의 list, set, multiset, map, multimap에 쓰인다.

	5. 임의 접근 반복자(Random Access Iterator)
		- 양방향 반복자에 "반복자 산술 연산" 수행 기능을 추가한 것. -> 반복자를 임의의 거리만큼 이동시키는 것을 상수 시간에
		- 위 산술연산은 포인터 산술 연산과 비슷한데, 기본 제공 포인터를 본떠서 임의 접근 반복자를 만들었기 때문에.
		- STL의 vector, deque, string 에 사용

	C++ 표준 라이브러리에는 위 다섯 개의 반복자 범주 각갓을 식별하는데 tag구조체가 정의되어 있다.
*/

struct Input_Iterator_tag {};
struct Output_Iterator_tag {};
struct Forward_Iterator_tag : public Input_Iterator_tag {};
struct Bidirectional_Iterator_tag : public Forward_Iterator_tag {};
struct Random_Access_Iterator_tag : public Bidirectional_Iterator_tag {};
// 상속관계가 is-a임을 확인하자.

/*
	위에서 보듯 반복자별 차이가 있기 떄문에 advence를 구현하는데 고려해야 할 것이 있다.
	바로 다른 반복자들은 루프를 돌며 원하는 횟수만큼 증가시키거나 감속시켜 구현 해야하지만
	임의 접근 반복자 입장에서 상수 시간에 접근이 가능하기 때문에 손해를 보는 것이기 때문이다

	따라서 다음과 같은 구조로 코드를 짜야할 것이다.
*/

template<typename IterT, typename DistT>	//IterT는 반복자, DistT는 거리
void advance(IterT& iter, DistT d)
{
	if (/* iter가 임의 접근 반복자라면*/)
		iter += d;
	else	// 임의 접근 반복자가 아닌 경우
	{
		if (d >= 0)
		{
			while (d--)
			{
				++iter;
			}
		}
		else
		{
			while (d++)
			{
				--iter
			}
		}
	}
}

/*
	위 와 같이 구현이 가능하려면 iterator가 어떤 반복자인지 확인 할 수 있어야 한다.
	이럴 떄, 컴파일 도중에 어떤 주어진 타입의 정보를 얻게 해주는 객체인 "특성 정보"라는 것이 있다.

	특성 정보는 C++ 에 미리 정의된 문법구조도 아니고, 키워드도 아니다. 그냥 관례이고, 구현 기법이다.

	특성정보가 되려면 몇 가지 요구사항이 지켜져야 하는데, 기본 제공 타입 사용자 정의 타입에서
	모두 돌아가야한다는 점이 그 중 하나이다.
	정확한 의미는 특성정보 기법을 포인터 등의 기본 제공 타입에 적용할 수 있어야 한다는 것이다.

	이것을 뒤집어서 생각해보면 어떤 타입 내에 중첩된 정보등으로는 구현이 안된다는 말과 같다.
	결국 어떤 타입의 특성정보는 그 타입 안에 있는 것이 아니라 외부에 있어야 한다는 것이다.

	이를 구현하는 가장 표준적인 방법은 해당 특성정보를 템플릿과 그 템플릿의 1개 이상의 특수화 버전에 넣는 것.
	반복자의 경우
	iterator_traits 라는 이름으로 되어 있다
*/
template <typename IterT>
struct itarator_traits;				// 반복자 타입에 대한 정보를 나타내는 템플릿

/*
	itarator_traits 는 구조체 템플릿인데(사실 클래스의 일종이지만) 특성정보는 항상 구조체로 구현하는 것이 관례이다.
	이처럼 특성정보를 구현하는데 사용한 구조체를 가리켜 "특성 정보 클래스"라고 부른다.

	itarator_traits<IterT> 안에는 IterT 타입 각각에 대해 iterator_category 라는 이름 typedef 타입이
	선언되어 있는데  그 typedef 타입이 바로 IterT의 반복자 범주를 가리키는 것.

	itarator_traits 클래스는 반복자 범주를 두 부분으로 나누어 구현한다.
	1. 사용자 정의 반복자 타입에 대한 구현.
		사용자 정의 반복자 타입으로 하여금 iterator_category라는 이름의 typedef타입을
		내부에 둘 것을 요구사항으로 둔다. 이 때, typedef 타입은 해당 태그 구조체에 대응되어야 한다.
		예를 들어 deque의 반복자는 임의 접근 반복자이므로, deque 클래스에 쓸 수 있는 반복자는 다음과 같다
*/

template</*...*/>
class Deque
{
public:
	class iterator
	{
	public:
		typedef random_access_iterator_tag iterator_category;
		//...
	};
};

//		list의 반복자는 양방향 반복자이기 때문에 다음과 같을 것이다.

template </*...*/>
class list
{
public:
	class iterator
	{
	public:
		typedef bidirectional_iterator_tag iterator_category;
	};
};

//		이 iterator 클래스 내부에 지닌 중첩typedef 타입을 똑같이 재생한 것이 iterator_traits이다.
template<typename IterT>
struct iterator_traits
{
	typedef typename IterT::iterator_category iterator_category;
};

/*
		위 코드는 사용자 정의 타입에 대해서는 잘 돌아가지만 반복자의 실제 타입이 포인터인 경우에는
		전혀 안돌아간다. 포인터 안에 typedef타입이 중첩된다는 것부터가 말이 되지 않기 때문이다.

	2. 반복자가 포인터인 경우의 처리
		포인터 타입의 반복자를 처리하기 위해서, itarator_traits는 포인터 타입에 대한
		부분 템플릿 특수화 번전을 제공하고 있다. 포인터의 동작원리가 임의 접근 반복자와 같기 때문에
		itarator_traits가 이런 식으로 지원하는 반복자 범주가 바로 임의 접근 반복자이다.
*/

template<typename IterT>
struct iterator_traits<IterT*>
{
	typedef typename IterT::iterator_category iterator_category;
};

/*
	특성정보 클래스의 설계와 구현에 대해 정리해보자.
	- 다른 사람이 사용하도록 열어주고 싶은 타입 관련 정보를 확인한다.
	- 그 정보를 식별하기 위한 이름을 선택한다(예 iterator_category)
	- 지원하려는 타입 관련 정보를 담은 템플릿과 템플릿 특수화를 통해 제공한다.

	이를 기반으로 advance의 의사코드를 다음으면 다음과 같다
*/

template <typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
{
	if (typeid(typename std::iterator_traits<IterT>::iterator_category) == typeid(std::random_access_iterator_tag))
	{
		//...
	}
}

/*
	하지만 문제가 있는데 IterT타입은 컴파일 도중에 파악되기 때문에
	iterator_traits<IterT>::iteator_category 를 파악할 수 있는 때도 역시 컴파일 도중이다.
	하지만 if문은 실행도중에 평가된다.
	컴파일 도중에 할 수 있는 굳이 실행도중에 하는 것은 시간 낭비이자, 실행코드의 비대화도 불러온다.

	이럴 때 필요한 것이 바로 오버로딩이다.
	컴파일 타임에 타입에 따라 선택되는 조건 처리 구분 요소를 가지기 때문.
	따라서 advance의 "동작 원리 알맹이"는 똑같게 하고,. 받아들이는 iterator_category객체의 타입을 다르게 해서
	오버로드 함수를 만든다.
*/

// 반복자에 대해서쓰는 템플릿
template<typename IterT, typename DistT>
void DoAdvance(IterT& iter, DistT d, std::random_access_iterator_tag)
{
	iter += d;
}

// 양방향 반복자에 대해서 쓰는 템플릿
template<typename IterT, typename DistT>
void DoAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag)
{
	if (d >= 0)
	{
		while (d--)
			++iter;
	}
	else
	{
		while (d++)
			--iter;
	}
}

// 입력 반복자에 대해서 쓰는 템플릿
template<typename IterT, typename DistT>
void DoAdvance(IterT& iter, DistT d, std::input_iterator_tag)
{
	if (d < 0)
	{
		throw std::out_of_range("Negative Distance");
	}
	while (d--)
		++iter;
}

/*
	forward_iterator_tag는 input_iterator_tag로부터 상속을 받을 것이므로
	input_iterator_tag를 매개변수로 받는 DoAdvance는 순방향 반복자도 받을 수 있다.

	이제 Advance가 해줄 일은 반복자 범주 타입 객체를 맞추어 DoAdvance를 호출해주는 것이다.
*/

template <typename ITerT, typename DistT>
void Advance2(IterT& Iter, DistT D)
{
	DoAdvance(Iter, D, typename std::iterator_traits<IterT>::iterator_category());
}

/*
	정리하면, 작업자(Worker) 역할을 맡을 함수 혹은 함수 템플릿을 특성정보 매개변수를 다르게하여 오버로딩하고,
	전단되는 해당 특성정보에 맞추어 각 오버로드 버전을 구현한다.

	이후 작업자를 호출하는 주작업자(Master) 역할을 맡을 함수 혹은 함수 템플릿을 만든다.
	이 때, 특성정볼 클래스에서 제공되는 정보를 넘겨서 작업자를 호출하도록 구현한다.

	iterator_traits 는 iterator_category말고도 네 가지가 더 있다. 대표적으로는 value_type이 있다.
	또한 문자 타입에 대한 정보를 담고 있는 char_traits, 숫자 타입에 대한 정보를 담고 있는 numeric_limits도 있다.

*/