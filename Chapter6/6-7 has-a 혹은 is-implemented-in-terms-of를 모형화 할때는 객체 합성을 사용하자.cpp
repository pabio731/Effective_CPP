#include "pch.h"

/*
	어떤 타입의 객체들이 그와 다른 타입의 객체들을 포함하고 있을 경우 그 관계를 부르는 용어로 합성, 레이어링, 포함, 통합, 내장등이 있다.
	이는 모두 같은 것을 말하며 사람따라 다른 것.
	아래 코드가 예시이다.
*/

class Address {};
class PhoneNumber {};
class Person
{
public:
	//...
private:
	std::string strName;
	Address addres;
	PhoneNumber VoiceNumber;
	PhoneNumber FaxNumber;
};

/*
	Person 객체는 string, Address, PhoneNumber 객체로 이루어져 있다.

	이러한 객체 합성 또한 의미를 가지는데, has-a 관계(...는...을 가짐) 또는 is-implemented-in-terms-of (...는..을 써서 구현됨)
	두 가지중 하나의 의미를 가질 수 있다. 두 가지는 소프트웨어 개발에서 우리가 대하는 영역에 따라 달라지는데,
	먼저 우리 일상생활에서 볼 수 있는 사물을 본 뜬 객체를 구현하는 것은 소프트웨어의 응용영역이고,
	그 외의 버퍼,뮤텍스, 탐색 트리 등 순수하게 시스템 구현만을 위한 인공물을 구현한 것은 구현 영역이다.

	객체 합성이 응용 영역에서 일어난다면 has-a관계이고
	객체 합성이 구현 영역에서 일어난다면 is-implemented-in-terms-of 관계이다.
ㅈ2
	위의 예제는 Person 클래스가 나타내는 관계는 has-a 관계로 볼 수 있다.
	Person객체는 이름과 주소, 번호를 가지게 된다. 사람이 이름의 일종이거나 주소의 일종이 아니다.

	헷갈릴 수 있는 부분은 is-a 관계와 is-implemented-in-terms-of관계인데
	예시를 통해 알아보자.
	객체로 구성된 set(중복 허용 x)이 필요한데, 저장공간도 적게 차지하는 클래스 템플릿으로 필요하다고 생각해보자
	가장 먼저 표준 라이브러리의 set을 떠올리게 될 것인데, set템플릿은 탐색, 삽입, 삭제의 시간 복잡도를 균형 시간 복잡도로
	하기 위해서 균형 탐색 트리로 만들어지기 때문에 원소 한 개당 세개의 포인터가 있어 저장 공간을 적게 차지 하는 템플릿으로는
	적절치 않다.

	따라서 템플릿을 그냥 만들게 된다고 생각하면, 연결 리스트를 통해서 만들자는 생각이 들어
	Set 템플릿을 만들 되, list에서 파생된 형태부터 시작하도록 만드는 상황 까지 온 것이다.
	즉 Set<T>는 list<T>를 상속하는 것.
	그럼 결국 Set 객체는 list객체의 일종이 되는 것이다
*/
#include <list>
template <typename T>
class Set1 :public list<t >> {};

/*
	하지만 여기서 문제가 있다. is-a 관계에 따라 list에서 참인 것들이 Set에서도 참이여야 할텐데
	list는 중복된 데이터가 있을 수 있다는 것이다.
	따라서 이를 해결하기 위해서
	앞서 말한 is-implemented-in-terms-of관계를 이용해서
	Set객체는 list 객체를 써서 구현되는 형태를 만들어보자
*/

template<typename T>
class Set
{
public:
	bool Member(const  T& Item) const;
	void Insert(const T& Item);
	void Remove(const T& Item);
	std::size_t size() const;

private:
	std::list<T> rep;				// Set 데이터의 내부 표현부
};

/*
	Set의 멤버함수는 list에서 이미 제공하는 기능과 표준 C++라이브러리의 다른 구성 요소를 잘 버무려 만들기만 하면되기 때문에 간단하다
*/

template<typename T>
bool Set<T>::Member(const T& Item) const
{
	return std::find(rep.begin(), rep.end(), Item) != rep.end();
}

template<typename T>
void Set<T>::Insert(const T& Item)
{
	if (!Member(Item))
		rep.push_back(iTem);
}

template<typename T>
void Set<T>::Remove(const T& ITem)
{
	typename std::list<T>::iterator it = std::find(rep.bigin(), rep.end(), Item);

	if (it != rep.end())
		rep.erase(it);
}

template<typename T>
std::size_t Set<T>::size() const
{
	return rep.size();
}

/*
	위 코드 예시를 통해서 is-implemented-in-terms-of에 대해서 잘 숙지하자
*/