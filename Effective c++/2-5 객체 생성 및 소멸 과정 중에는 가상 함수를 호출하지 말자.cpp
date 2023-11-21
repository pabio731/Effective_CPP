#include<iostream>
/*
객체를 상성 및 소멸하는 과정에서는 절대 가상 함수를 호출해서는 안된다.
아마 그렇게 한다고 해도 프로그램이 돌아가지 않을 것이며, 돌아가더라도 문제가 생길 가능성이 높다.

아래 예시는 주식 거래를 클래스를 통해 구현한 것이다. 이 주식 거래에서 거래 객체가 생성될 때마다 감사 로그에
거래 내역이 만들어지도록 한 코드이다.
*/

//기본 클래스
class Transaction
{
public:
	Transaction();
	virtual void LogTransaction() const = 0;	//타입에 따라 달라지는 로그 기록을 만드는 순수가상함수
	//...
};
Transaction::Transaction()
{
	LogTransaction();
}

// 파생 클래스1
class BuyTransaction : public Transaction
{
public:
	virtual void LogTransaction() const override;	// 이 타입에 따른 거래내영 로깅을 구현
	//...
};

// 파생 클래스2
class SellTransaction : public Transaction
{
public:
	virtual void LogTransaction() const override;	// 이 타입에 따른 거래내영 로깅을 구현
};

/*
	여기서 BuyTransaction B를 선언한다면 BuyTransaction의 생성자가 호출되는 것은 맞다. 그러나 파생 클래스의 객체가
	생성될 때 기본 클래스의 부분이 파생 클래스 부분모다 먼저 호출이 되기 때문에 우선 Transaction의 생성자가 호출되어야 한다.
	이 때, Transaction의 마지막 부분에 LogTransaction 가상 함수가 호출 되는데, 이 부분에서 문제가 생긴다.
	이때 실행되는 것은 BuyTransaction::LogTransaction이 아니라 Transaction::LogTransaction이 되는데, 이는
	생성자가 호출되는 동안은 절대로 파생 클래스쪽으로 내려가지 않고, 자신이 기본 클래스 타입인 것 처럼 동작하기 때문이다.

	간단하게 생각하면 기본 클래스 생성자가 실행되고 있는 시점에 파생 클래스 데이터 멤버는 아직 초기화된
	상태가 아니라는 것이다. 그래도 만약 어찌어찌 파생 클래스쪽으로 내려간다면 어찌될까?

	파생 클래스에서 오버라이딩된 멤버 함수는 파생 클래스의 데이터 멤버를 건드릴 가능성이 농후한데, 아직 이들은 초기화되지
	않았을 것이기 때문에 또 다시 문제가 된다.

	이 문제의 핵심은 파생 클래스의 객체가 생성되는 과정에서 기본 클래스의 생성자가 실행되고 있는 동안 이 객체의 타입은
	"기본 클래스"라는 것이다. 결국 가상 함수뿐만 아니라 런타임 타입 정보( ex. dynamic_cast, typeid)를 사용한다면
	파생 클래스의 객체가 기본 클래스 타입의 객체로 취급된다는 것이다.

	이는 소멸자가 실행되는 상황에서도 마찬가지이다. 파생 클래스의 소멸자가 !일단 호출되고 나면! 파생 클래스만의 데이터 멤버는
	정의되지 않은 값으로 가정하기 떄문에, C++은 이들을 없는 것처럼 취급하고 진행한다. 기본 클래스의 소멸자에 진입할 당시 객체는
	기본 클래스 객체로 취급된다. 위의 생성자, 소멸자에서의 상황은 알기 쉬운 상황이라 컴파일러가 알아서 경고가 나올 수도 있다.

	하지만 실제 상황에서는 생성자의 수도 많아지고, 이에 따라 비슷한 코드가 많아지면서 편의성을 위해 중복된 코드를 다시 하나의
	함수안에 넣어버리는 상황이 나올 수도 있다. 만약 거기서 가상 함수를 호출하고, 생성자 또는 소멸자가 중복을 방지하기 위해
	선언한 함수를 호출한다면, 컴파일러도, 링크도 말끔히 되어버리는 상황이 나오기 때문에 더 골치아파진다.
	그래도 호출되는 가상 함수가 순수 가상 함수라면 대개 프로그램이 종료되는데, 일반 가상 함수라면 디버깅에 어려움을
	겪을 수도 있다.

	이를 해결하는 방법으로, 아래코드를 보자
*/

class Transaction
{
public:
	explicit Transaction(const std::string& LogInfo);
	void LogTransaction(const std::string& LogInfo);		//비 가상 함수로 선언
};
Transaction::Transaction(const std::string& LogInfo)
{
	//...
	LogTransaction(LogInfo);	//비 가상 함수를 호출
}

class BuyTransaction : public Transaction
{
public:
	BuyTransaction() : Transaction(CreateLogString(/*parameters*/)) {} //로그 정보를 기본 클래스 생성자로 넘깁니다.
private:
	static std::string CreateLogString(/*parameters*/);
};

/*
	간단하게 말하면 필요한 초기화 정보를 파생 클래스쪽에서 기본 클래스의 생성자로 올려주는 것.
	CreateLogString 함수는 이 것을 구현하는데 필요한 도우미 함수로, 기본 클래스쪽으로 넘길 값을 생성하는 용도로 쓰임.
	(생성자에 초기화 리스트가 주렁주렁 달릴 때 사용하면 좋음)
	이 함수도 정적 멤버이기 때문에, 생성이 끝나지 않은 파생 클래스 객체의 데이터 멤버를 건드릴 위험도 없다.
*/