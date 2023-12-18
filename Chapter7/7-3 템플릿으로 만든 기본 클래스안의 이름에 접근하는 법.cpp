#include "pch.h"
/*
	다른 몇 개의 회사로 메세지를 전송할 수 있는 응용플로그램을 만든다고 가정하자.
	이 메세지는 암호화될 수 있고, 비암호화 형태가 될 수 있다.
	만약 어떤 메시지가 어떤 회사로 전송될지를 컴파일 도중에 결정할 수 있는 충분한 정보가 있다면
	템플릿 기반으로 코드를 짤 것이다
*/
class CompanyA
{
public:
	//...
	void SendClearText(const std::string& Msg);
	void SendEncrypted(const std::string& Msg);
	//...
};

class CompanyB
{
public:
	//...
	void SendClearText(const std::string& Msg);
	void SendEncrypted(const std::string& Msg);
	//...
};

class MsgInfo {/*...*/ };

template <typename Company>
class MsgSender
{
public:
	//...
	void SendClear(const MsgInfo& Info)
	{
		std::string Msg;
		//Info로부터 Msg를 만든다.

		Company C;
		C.SendClearText(Msg);
	}
	void SendSecret(const MsgInfo& Info)
	{
		std::string Msg;
		//Info로부터 Msg를 만든다.

		Company C;
		C.SendEncrypted(Msg);
	}
};

/*
	여기에 덧붙여서 메세지를 보낼 떄마다 관련 정보를 로그로 남기고 싶다면?
*/
template <typename Company>
class LoggingMsgSender : public msgSender<Company>
{
public:
	void SendClearMsg(const MsgInfo& Info)
	{
		// 메세지 전송 전 정보를 로그에 기록
		SendClear(info);                    // 기본 클래스의 함수를 호출하는데, 이 코드가 컴파일이 되지 않는다!
		// 메세지 전송 후 정보를 로그에 기록
	}
};

/*
	파생 클래스에 있는 메세지 전송 함수의 이름(SendClearMsg)이 기본 클래스의 함수(SendClear)과
	다르다는 점이 보인다. 기본 클래스로부터 물려받은 이름을 파생 클래스에서 가리는 문제는 물론,
	상속받은 비가상 함수를 재정의 하지 않아야 한다는점도 잘 지켜진 것이다.
	하지만 컴파일이 되지 않는다.
	기본 클래스에서 SendClear 함수가 버젓이 있는데도 되지 않는다.

	문제는 간단하다. 컴파일러가 LoggingMsgSender 클래스 템플릿의 정의와 마주칠 때,
	컴파일러는 대체 이 클래스가 어디서 파생된 것인지를 모른다는 것이다.
	MsgSender<Company>인 것은 분명 맞지만. Company는 템플릿 매개변수이고,
	이 템플릿 매개변수는 LoggingMSgSender가 객체로 만들어질 때까지 무엇이 될지 알 수 없다.
	Company가 정확히 무엇인지 모르는 상황에서는 MsgSender<Company> 클래스가 어떤 형태인지 알 방법이 없다.

	다음 예도 보자
	CompanyZ라는 클래스가 있고 이 클래스는 암호화된 통신만을 사용해야 한다.
*/

class CompanyZ  // 이 클래스는 SendClearText함수를 제공하지 않는다.(암호화된 통신만 사용하므로)
{
public:
	void SendEncrypted(const std::string& Msg);
	//...
};
/*
	일반형 MsgSender 템플릿은 그대로 CompanyZ 클래스에 쓰기엔 좀 그렇다.
	이 템플릿은 CompanyZ객체의 설계 철학과 맞지 않는 SendClear함수를 제공하기 때문이다...
	따라서 CompanyZ를 위한 MsgSender의 특수화 버전을 만들 수 있다.
*/

template <>
class MsgSender<CompanyZ>
{   // MsgSender 템플릿의 완전 특수화 버전. SendClear 함수가 빠진 것만 제외하면 일반형 템플릿과 같다.
public:
	void SendSecret(const MsgInfo& Info)
	{
		//...
	}
};

/*
	위 코드는 MsgSender 템플릿은 템플릿 매개변수가 CompanyZ일 때 쓸 수 있도록 특수화한 버전이다.
	그리고 이러한 특수화를 완전 템플릿 특수화(total template Specializaion)이라고 한다.
	특수화된 템플릿의 매개변수들이 하나도 빠짐없이 구체적인 타입으로 정해진 상태라는 것.
	따라서 일단 타입 매개변수가 CompanyZ로 정의된 이상 이 템플릿의 매개변수로는 다른 것이 올 수 없게 된다는 것.

   이제 LoggingMsgSender로 다시 돌아와보자
*/

template <typename Company>
class LoggongMsgSender2 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		// 메세지 전송 전 정보를 로그에 기록
		SendClear(Info);        //만약 Company == CompanyZ라면 이 함수는 있을 수 조차 없다.
		// 메세지 전송 후 정보를 로그에 기록
	}
};
/*
	ㅏㅂ로 이런 일이 생길 수 있기 때문에 위와 같은 함수 호출을 C++이 받아주지 않는 것이다.
	기본 클래스 템플릿은 언제라도 특수화될 수 있고, 이런 특수화 번전에서 제공하는 인터페이스가
	원래의 일반형 템플릿과 꼭 같으리란 법은 없다는 점을 C++이 인식한다는 것!
	따라서 C++ 컴파일러는 템플릿으로 만들어진 기본 클래스를 뒤져서 상속된 이름을 찾는 것을 거부한다.

	이를 해결하기 위한 세 가지 방법이 있다.
	1. 기본 클래스 함수에 대한 호출문 앞에 this->을 붙인다.
*/

template <typename Company>
class LoggongMsgSender3 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		// 메세지 전송 전 정보를 로그에 기록
		this->SendClear(Info);
		// 메세지 전송 후 정보를 로그에 기록
	}
};

//  2. using 키워드를 사용한다.

template <typename Company>
class LoggongMsgSender4 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		using MsgSender<Company>::SendClear;
		// 메세지 전송 전 정보를 로그에 기록
		SendClear(Info);
		// 메세지 전송 후 정보를 로그에 기록
	}
};// 템플릿화된 기본 클래스의 유효범위를 뒤지라고 우리가 컴파일러에게 알려주지 않으면
//   컴파일러가 알아서 찾는 일이 없다는 것.

// 3. 호출될 함수가 기본 클래스의 함수라는 점을 명시적으로 지정하는 것.

template <typename Company>
class LoggongMsgSender5 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		// 메세지 전송 전 정보를 로그에 기록
		MsgSender<Company>::SendClear(Info);
		// 메세지 전송 후 정보를 로그에 기록
	}
};
/*
	이 방법은 추천하지 않는다. 호출되는 함수가 가상 함수인 경우 이런식으로 한정해버리면
	동적 바인딩이 무시되기 때문이다.(정적으로 바인딩되어버림)

	기본적으로 위 세 가지 방법의 동작원리는 같다. 기본 클래스 템플릿이
	이후에 어떻게 특수화되더라도 원래의 일반형 템플릿에서 제공하는 인터페이스를 그대로 제공할 것이라고
	컴파일러에게 약속을 한다는 부분이.

	하지만 그 약속이 잘못됐다면?
*/

LoggingMsgSender<CompanyZ> zMsgSender;
MsgInfo MsgData;
//MsgData에 정보를 채움
zMsgSender.SendClearMsg(MsgData);   //컴파일되지 않음

/*
	지금 이부분에서는 기본 클래스가 MsgSender<CompanyZ>라는 사실을 컴파일러가 알고 있는데다가
	SendClearMsg함수가 호출하려는SendClear함수는 MsgSender<CompanyZ>클래스에 안 들어있다는 사실도 컴파일러가 알아챈 후이기 때문.

	기본 클래스의 멤버에 대한 참조가 무효한지를 컴파일러가 진단하는 과정이 파생 클래스 템플릿의 정의가 구문분석이 되는 상황에 들어가느냐
	아니면 파생 클래스 템플릿이 특정한 템플릿 매개변수를 받아 객체화될 때 들어가느냐가 이 내용의 핵심이고,
	C++은이른 진단을 선호하는 정책으로 결정했다는 것이다.
	파생 클래스가 템플릿으로부터 객체화될 대 컴파일러가 기본 클래스의 내용에 대해 아무것도 모르는 것으로 가정하는 이유가 그것이다
*/