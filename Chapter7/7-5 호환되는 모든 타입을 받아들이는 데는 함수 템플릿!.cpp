#include"pch.h"

/*
	스마트 포인터는 그냥 포인터처럼 동작하면서도 아주 좋은 기능이 덤으로 달려있는 아주 좋은 객체이다
	하지만 스마트포인터가 대체할 수 없는 포인터만의 특징이 있는데, 바로 암시적 변환을 지원한다는 점이다.
*/

class Top {/*...*/ };
class Middle : public Top {/*...*/ };
class Bottom : public Middle {/*...*/ };
Top* pt1 = new Middle;							//Middle* -> Top*의 변환
Top* pt2 = new Bottom;							//Bottom* -> Top*의 변환
const Top* pct2 = pt1;							//Top* -> const Top*의 변환

// 이를 사용자 정의 스마트 포인터를 써서 흉내내려면 무척 까다롭다.
// 다음과 같은 코드를 컴파일러가 가만히 두지 않는다.

template <typename T>
class SmartPtr
{
public:
	explicit SmartPtr(T* RealPtr);		//스마트 포인터는 대개 기본 제공 포인터로 초기화된다.
};

SmartPtr<Top> pt1 = SmartPtr<Middle>(new Middle);		//Smart<Middle> -> Smart<Top>의 변환
SmartPtr<Top> pt2 = SmartPtr<Bottom>(new Middle);		//Smart<Bottom> -> Smart<Top>의 변환
SmartPtr<const Top> pct2 = pt1;							////Smart<Top> -> Smart<const Top>의 변환

/*
	같은 템플릿으로부터 만들어진 다른 인터턴스들 사이에는 어떤 관계도 없기 때문에
	컴파일러에게 Smart<Middle>과 Smart<Top>는 완전히 별개의 클래스이다.
	따라서 변환이 되도록 직접 만들어야한다.

	생성자 함수를 직접 만드는 것으로 우리에게 필요한 모든 생성자를 만들어내기란 불가능하다.
	위의 계통에서 Middle, Bottom으로부터 Top을 생성할 수 있지만, 클래스 계통이 더 확장한다든지 하면
	다른 스마트 포인터 타입으로부터 SmartPtr<Top> 객체를 만들 방법도 마련되어야 한다.
	이렇게 변경이 생길 때마다 SmartPtr템플릿을 변경해야 한다는 것.

	원칙적으로 우리가 원하는 생성자의 개수는 무한대이다. 따라서 생성자를 찍어내는
	멤버 함수 템플릿을 쓰자.
	멤버 함수 템플릿은 간단히 말해서 어떤 클래스의 메머 함수를 찍어 내는 템플릿이다.
*/

template <typename T>
class SmartPtr2
{
public:
	template <typename U>
	SmartPtr2(const SmartPtr<U>& Other);	//"일반화된 복사 생성자"를 만들기 위한 멤버 템플릿.
};

/*
	위 코드를 해석하면 모든 T타입 및 U 타입에 대해서 SmartPtr<T> 객체가
	SmartPtr<U>로부터 생성될 수 있다는 것.

	SmaerPtr<U>의 참조자를 매개변수로 받아들이는 생성자가 SmartPtr<T> 안에 들어 있기 때문.
	이런 형태를 일반화 복사 생성자라고 부르기도 한다.

	위 일반화 복사 생성자는 explicit으로 선언 하지 않았는데, 이는 기본제공 포인터가
	포인터 타입 사이의 타입 변환이 암시적으로 이루어지며 캐스팅이 필요하지 않기 때문에
	스마트 포인터도 이를 흉내내도록 하기 위해서이다.

	그런데 지금 일반화 복사 생성자는 우리가 원하는 것 이상의 일을 할 수있다.
	우리는 Bottom으로부터 Top을 만들 수 있기만을 바랐지 Top으로부터
	Bottom을 만들 수 있는 것 까지는 바라지 않았기 때문이다.
	위 코드는 심지어 SmartPTr<double>로부터 SmartPTr<int>를 만드는 것까지 가능하기 때문에
	이를 막아줄 수 있는 무언가가 필요하다.

	tr1::shared_ptr에서 쓰는 방법을 그대로 따라서 SmartPTr도 Get함수를 통해
	해당 스마트 포인터 객체에 자체적으로 담긴 기본제공 포인터의 사본을 반환한다고 가정하면
	이것을 이용하여 제약을 줄 수 있다.
*/

template <typename T>
class SmartPtr
{
public:
	template<typename U>
	SmartPTr(const SmartPTr<U>& Other)		//이 SmartPTr에 담긴 포인터를 다른 SmartPTr에 담긴 포인터로 초기화
		:HeldPtr(Other.Get()) {/*...*/
	}
	T* Get() const { return Heldptr; }
private:
	T* HeldPtr;				// SmartPTr에 담긴 기본 제공 포인터
};

/*
	이니셜라이저를 통해서 SmartPTr<T>의 데이터 멤버인 T* 타입의 포인터를 SmartPTr<U>에 들어있는 U*타입의 포인터로 초기화한다.
	이렇게 하면 U*에서 T*로 진행되는 암시적 변환이 가능할 때만 컴파일 에러가 나지 않는다.
	즉 SmartPTr<T>의 일반화 복사 생성자는 호환되는 타입의 매개변수를 넘겨받을 떄만 컴파일 되도록 만들어졌다는 것.

	멤버 함수 템플릿의 활용은 생성자 뿐만 아니라 대입 연산에도 쓰인다
	예를 들면 TR1의 shared_ptr클래스 템플릿은 호환되는 모든 기본 제공 포인터, tr1::shared_ptr, tr1::weak_ptr객체들로부터
	생성자 호출이 가능한데다가, 이들 중 tr1::weak_ptr을 제외한 나머지를 대입 연산에 쓸 수 있도록 만들어져 있다.
	아래는 tr1::shared_ptr템플릿의 모습이다.
*/

template <class T> class shared_ptr
{
public:
	template<class Y>
	explicit shared_ptr(Y* p);

	template<class Y>
	shared_ptr(shared_ptr<Y> const& r);

	template<class Y>
	explicit shared_ptr(weak_ptr<Y> const& r);

	template<class Y>
	explicit shared_ptr(auto_ptr<Y>& r);

	template<class Y>
	shared_ptr& operator=(shared_ptr<Y> const& r);

	template<class Y>
	shared_ptr& operator=(auto_ptr<Y>& r);
}; // 호환되는 모든 기본제공 포인터, shared_ptr, weak_ptr, auto_ptr객체로부터 생성자호출 가능
  //  호환되는 shared_ptr, auto_ptr로부터 대입이 가능

/*
	일반화 복사 생성자를 제외하고 모든 생성자가 explicit으로 선언되어 있다.
	이것은 shared_ptr로만든 어떤 타입으로부터 또 다른 타입으로 진행되는 암시적 변환은 허용되지만
	기본 제공 포인터나 다른 스마트 포인터 타입으로부터 변환되는 것은 막겠다는 뜻이다.

	멤버 함수 템플릿은 훌륭한 기능이지만 C++의 기본 규칙을 바꾸지는 않는다.
	tr1::shared_ptr에는 분명 일반화 복사 생성자가 선언되어 있는데
	T타입과 Y타입이 동일하게 들어온다면 이 일반화 복사 생성자는 분명히
	보통의 복사 생성자를 만드는 쪽으로 인스턴스화 될 것이다.

	그럼 tr1::shared_ptr객체가 자신과 동일한 타입의 다른 tr1::shared_ptr 객체로부터
	생성되는 상황에서 컴파일러는 디폴트 복사 생성자를 만들까 아니면
	일반화 복사 생성자 템플릿을 인스턴스화 할까?

	답은 "디폴트 복사 생성자를 만든다"이다.
	일반화 복사 생성자를 선언하는 것은 비 템플릿 복사 생성자를 만드는 것을 막는 요소가 아닌것.
	이는 대입 연산자도 마찬가지이다.
	다음은 이와 관련해 tr1::shared_ptr이 어떻게 처리했는지 볼 수 있는 코드이다.
*/
template <class T>
class shared_ptr
{
public:
	shared_ptr(shared_ptr const& r);	// 복사 생성자

	template <class Y>					// 일반화
	shared_ptr(shared_ptr<Y> const& r);	// 복사 생성자

	shared_ptr& operator=(shared_ptr const& r);	// 복사 대입 연산자

	template <class Y>
	shared_ptr& operator=(shared_ptr<Y> const& r);	// 복사 대입 연산자
};