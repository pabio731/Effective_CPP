#include "pch.h"

/*
	다중 상속하면 가장 먼저 떠올릴 수 있는 문제는 이름의 중복이다.
	둘 이상의 기본 클래스로부터 같은 이름을 물려받을 가능성이 생긴다는 것.
	아래 예시를 보자 MP3Player 클래스는 두 개의 클래스를 상속한다.
*/
class CBorrowableItem
{
public:
	void CheckOut();		//라이브러리로부터 체크 아웃합니다
};

class CElectronicGadget
{
private:
	bool CheckOut() const; //자체 테스트를 실시하고, 성공여부 반환
};

class CMP3Player :
	public CBorrowableItem,
	public CElectronicGadget
{};

int main()
{
	CMP3Player mp;
	mp.CheckOut();
}

/*
	CheckOut을 호출하는 부분에서 모호성이 발생한다.
	심지어 mp객체가 호출할 수 있는 CheckOut은 BorrowableItem::CheckOut임이
	분명한데도 모호성이 발생한다(ElectronicGadget::CheckOut은 private이므로)

	이것은 어떤 함수가 접근 가능한 함수인지 확인해보기전에 컴파일러가 C++의 중복된
	함수 호출 중 하나를 골라내는 규칙을 따른 것이다.
	주어진 호출에 대해 최적으로 일치하는 함수인지 먼저 확인 후,
	함수의 접근 가능성을 판단함.
	즉  ElectronicGadget::CheckOut 함수의 접근 가능성을 점검하는 순서조차 가지 않은 것.

	이러한 모호성을 해소하기 위해서는 Scope연산자로 어떤 클래스의 함수인지 지정 해줘야 한다.
	mp.BorrowableItem::CheckOut(); 처럼!

	이 문제는 상위 단계의 기본 클래스를 여러 개 갖는 클래스 계통에서 심심치 않게 눈에 뛴다.
	특히 다이아몬드 상속의 문제가 나올 수 있다.
*/
class CFile {};
class CInputFile : public CFile {};
class COutputFile : public CFile {};
class CIOFile : public CInputFile, public COutputFile {};

/*
	이런 방식이 되면 문제는 기본 클래스의 데이터 멤버가 경로 개수만큼 중복 생성되는 문제가
	생긴다.
	만약 데이터 멤버를 중복생성하는 것을 원한 것이 아니였다면 중복될 수 있는 데이터 멤버를
	가진 클래스들을 가상 기본 클래스로 만들어서 이 문제를 해결할 수 있다.
	즉 가상 상속을 사용하는 것.
*/

class CFile2 {};
class CInputFile2 : virtual public CFile2 {};
class COutputFile2 : virtual public CFile2 {};
class CIOFile2 : public CInputFile2, public COutputFile2 {};

/*
	실제로 c++의 표준 라이브러리중 basic_ios, basic_istream, basic_ostream, basic_iostream이
	이런 구조로 되어 있다.

	정확한 동작의 관점에서는 public상속은 반드시 항상 가상 상복이어야 하는 것이 맞다.
	하지만 사실 상속되는 데이터 멤버의 중복생성을 막는 데에는 우리눈에 보이지 않는 컴파일러의
	노력이 들어있다. 이 때문에 가상 상속을 사용하는 클래스로 만들어진 객체는 그렇지 않은
	객체보다 크기가 더 크다. 또한 가상 기본 클래스의 데이터 멤버에 접근하는 속도도 비가상
	기본 클래스의 데이터 멤버에 접근하는 속도보다 느리다.
	즉 가상 상속은 비싸다.

	심지어 다른 쪽으로 또 비용이 나간다. 가상 기본 클래스의 초기화에 관련된 규칙은
	비가상 기본 클래스의 초기화 규칙보다 훨씬 복잡한데다가 직관성도 더 떨어진다.
	1. 초기화가 필요한 가상 기본 클래스로부터 클래스가 파생돤 경우, 이 파생 클래스는
	   가상기본 클래스와의 거리에 상관없이 가상 기본 클래스의 존재를 염두에 두고 있어야 한다.
	2. 기존의 클래스 계통에 파생 클래스를 새로 추가할 때도 그 파생 클래스는 가상 기본 클래스의
	   초기화를 떠맡아야 한다.

	따라서 가상 상속에 대해서는 다음 두 가지를 따르자
	1. 구태여 쓸 필요가 없으면 가상 기본 클래스를 사용하지 말 것.
	2. 가상 기본 클래스를 정말 쓰지 않으면 안될 상황이라면, 가상 기본 클래스에는 데이터를 넣지 않으려 노력한다.

	이제, C++인터페이스 클래스를 써서 사람을 모형화한 코드를 보자
*/
class IPerson
{
public:
	virtual ~IPerson();
	virtual std::string strName() const = 0;
	virtual std::string strBirthDate() const = 0;
};
/*
	IPerson을 쓰려면 분명히 IPerson 포인터나 참조자를 통래서 프로그래밍 해야 할 것이다.
	추상 클래스이니까. 조작이 가능한 IPerson 객체(정확히는 IPerson의 동작원리를 쓰는 객체)를
	생성하기 위해서 IPerson의 사용자는 팩토리 함수를 사용해서 IPerson의 구체 파생 클래스를
	인스턴스로 만든다.
*/
typedef size_t DatabaseID;
DatabaseID ID(0);
std::tr1::shared_ptr<IPerson> MakePerson(DatabaseID piecewise_construct);
DatabaseID AskUserForDatabaseID();		// 사용자로부터 데이터베이스ID를 얻어내는 함수
DatabaseID Id(AskUserForDatabaseID());

// CPersom인터페이스를 지원하는 객체를 하나 만들고 pp로 가리키게 한다.
// 이후 *pp의 조작을 위해 IPerson의 멤버함수를 사용한다.
std::tr1::shared_ptr<IPerson> pp(MakePerson(ID));

/*
	MakePerson함수는 어떻게 해서 그 함수가 반환할 포인터로 가리킬 객체를 새로 만들 수
	있는 것일까? 아마 MakePerson함수가 인스턴스로 만들 수 있는
	구체 클래스가 IPerson로부터 파생되어 있어야 할 것이다.
	그 클래스의 이름을 CPerson이라 가정하자. CPerson은 IPerson으로부터 물려받은
	순수 가상 함수에 대한 구현을 제공할 것이다.
	그런데 이전에 만들어둔 데이터베이스 전담 클래스인 PersonInfo를 보니
	현재 CPerson에 필요한 핵심 기능을 다 갖고 있었다고 가정하자
*/

class PersonInfo
{
public:
	explicit PersonInfo(DatabaseID pID);
	virtual ~PersonInfo();

	virtual const char* TheName() const;
	virtual const char* TheBirthData() const;

private:
	virtual const char* ValueDelimOpen() const;
	virtual const char* ValueDelimClose() const;
};
/*
	그렇다면 CPerson과 PersonInfo 사이를 잇는 관계는 별게 없다. PersonInfo 클래스는 CPerson을
	구현하기 편하게 만들어주는 함수를 어쩌다 갖고 있는 것이다
	그리고 우리는 이런 관계를 is-implemented-in-terms-of의 관계라 하기로 했다.
	그리고 이런 관계를 구현하는 방법으로 객체 합성과 private상속 두 가지가 있다.
	대부분은 객체 합성를 선호하지만, 가상 함수를 재정의 해야한다면 private 상속을 해야한다
	ValueDelimOpen 함수와 ValueDelimClose함수를 재정의할 필요가 있는 상황이라 가정한다면
	private 상속을 하게 될 것이다. 물론 객체 함성과 상속을 조합하는 방법도 있지만 private상속을
	이용한다고 생각해보자 그렇다면 바로 지금이 다중 상속을 의미있게 쓸 수 있는 상황이된 것이다.
	인터페이스의 public 상속과 private상속을 조합하는 것이다.
*/

class CPerson : public IPerson, private PersonInfo
{
public:
	explicit CPerson(DatabaseID pid)
		: PersonInfo(pid) {}
	virtual std::string strName() const { return PersonInfo::TheName(); }
	virtual std::string strBirthDate() const { return PersonInfo::TheBirthData(); }
private:
	const char* ValueDelimOpen() const { return ""; }
	const char* ValueDelimClose() const { return ""; }
}

/*
	다중 상속도 객체 지향 기법으로 소프트웨어를 개발하는 데 쓰이는 도구 중 하나이다.
	물론 단일 상속보다는 사용하기도, 이해하기도 어렵기 때문에 가능하다면 단일 상속으로 해결하는 것이 좋다.
	가능하면 단일 상속으로 최대한 해결하려 궁리를 하고, 정말 필요하다면 다중 상속을 쓰도록 하자.
*/