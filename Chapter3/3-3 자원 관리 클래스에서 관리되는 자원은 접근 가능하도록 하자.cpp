#include<iostream>

/*
	자원 관리 클래스는 실수로 자원 생길 수 있는 보호벽 역할을 해준다. 따라서 굳이 직접 실제 자원을 조작할 이유가 없다.
	하지만 실제 현장의 코드들은 이미 자원을 직접 참조하도록 만들어져 있을 수 있기 때문에 실제 자원을 만져야 할 일이 충분히 생길 수 있다.

	투자를 관리하는 Investment 클래스객체를
	RAII를 이용하기 위해 스마트 포인터를 이용해
	std::tr1::shared_ptr<Investment> pInv(CreateInvestment());
	받았다.

	이제 이 객체를 사용하는 함수로
	int DaysHeld(const Investment* pi);		//투자금이 유입된 이후로 경과한 날수를 반환
	가 있다고 하고, 이를 다음과 같이 쓰게 될 것인데

	int iDays = DaysHeld(pInv);
	이 코드는 컴파일이 안된다. pInv는 타입이 tr1::shared_ptr<Investment>이지 Investment*가 아니기 때문이다.

	따라서 RAII클래스의 객체를 그 객체가 감싸고 있는 실제 자원(Ivestment*)으로 변환할 방법이 필요하다.
	이 경우 두 가지의 일반적인 방법이 있는데, 바로 명시적 변환(explicit conversion)과 암시적 변환(implicit conversion)이다.

	tr1::shared_ptr과 auto_ptr 은 명시적 변환을 수행하는 get이라는 멤버 함수를 제공한다. 이 함수를 사용하면 각 타입으로 만든
	스마트 포인터 객체에 들어있는 실제 포인터(의 사본)을 얻어낼 수 있다.

	int iDays = dDaysHeld(Pinv.get())	// pInv로 감싸져 있는 실제 포인터의 사본을 넘김.

	두 스마트 포인터는 포인터 역참조 연산자(operator-> 과 operator*)도 오버로딩 하고 있다. 이에 따라 암시적 변환도 쉽게 할 수 있는데
	다음과 같다.
*/

class Investment
{
public:
	bool IsTaxFree() const;
	//...
};

Investment* CreateInvestment();		//팩토리 함수

std::tr1::shared_ptr<Investment> pi1(CreateInvestment());	//스마트포인터가 자원 관리를 맡도록 함
bool Taxable1 = !(pi1->IsTaxFree());			//operator->를 써서 자원에 접근.
//...

std::auto_ptr<Investment> pi2(CreateInvestment());
bool Taxable2 = !((*pi2).IsTaxFree());			//operator*를 써서 자원에 접근

/*
	RAII 객체 안에 들어 있는 실제 자원을 얻어낼 필요가 종종 생기기 때문에 RAII 클래스 설계를 하는 사람 중에는 암시적 변환 함수를 제공하는 사람도 있다.
	다음은 하부 수준 C API로 직접 조작이 가능한 폰트를 RAII클래스로 둘러싸서 쓰는 경우이다.
*/
FontHandle Getfont();		//C API에서 가져온 함수
void ReleaseFont(FontHandle fh);	//C API에서 가져온 함수

class Font		//RAII 클래스
{
public:
	explicit Font(FontHandle fh) : f(fh) {}			//자원을 획득, 여기서 값에 의한 전달이 수행된다는 점에 주의. 자원해제를 C API로 하기 때문에.
	~Font() { RereaseFont(f); }
private:
	FontHandle f;		//실제 폰트 자원
};

/*
	하부 수준 C API는 FontHandle을 사용하도록 만들어져 있으며 규모도 무척 크다고 가정하면, Font객체를 FontHandler로 변환해야 하는
	경우도 적지 않을 것이라는 예상을 해볼 수 있다.
	FontHandle get() const { return f;  }		//명시적 변환 함수.
	이 함수를 public 접근 지정에 선언하면 쓸 수 있기야 하지만 하부 수준 API를 쓰고 싶을 때마다 get을 호출해야 한다.
*/

void ChangeFontSize(FontHandle f, int iNewSize);
Font f(getFont());
int iNewFontSize;
//...
ChangeFontSize(f.get(), iNewFontSize);		//FOnt에서 FontHandle로 명시적으로 바꾼 후에 넘김.

/*
	위와 같이 계속해서 함수를 호출해주어야 한다는 점이 마음에 들지 않는다.
	이에 대한 대안으로 암시적 변화 함수를 제공하는 것을 보자
*/

class Font		//RAII 클래스
{
public:
	explicit Font(FontHandle fh) : f(fh) {}
	~Font() { RereaseFont(f); }
	oprator FontHandle() const { return f; }
private:
	FontHandle f;		//실제 폰트 자원
};

/*
	암시적 변환 함수 덕에 C API를 사용하기가 훨씬 쉬워지고 자연스러워질 수 있다.
	다음 코드를 보자
*/

Font f(getfont());
int iNewFontSize;
//...
ChangeFontsize(f, iNewFontSize);	//Font에서 FontHandle로 암시적 변환을 수행

//다만 이 경우 실수를 저지를 여지도 많아짐므로 조심해야 한다.

Font f1(getFont());
//...
FontHandle f2 = f1
//원래 의도는 font객체를 보사하는 것이엇는데,  f1이 FontHandle로 바꾸고 복사되어버림.

/*
	만약 이런 상황이 나오면 Font객체인 f1이 관리하고 있는 FontHandle가 f2를 통해서도 직접 사용할 수 있는 상태가 되어버린다..
	심지어 f1이 소멸하면 f2는 소멸한 객체를 가리키게됨.

	명시적 변환을 허용할지 암시적 변환을 허용할지는 각각의 RAII 클래스만의 용도나 환경에 따라....
	일반적으로는 암시적 변환보다느 명시적 변환이 원하지 않는 변환을 막아줄 가능성이 크기 때문에 낫다.

	추가적으로 RAII클래스는 원래 목적이 캡슐화가 아니기 때문에 특성에 크게 위배된다고 보기 어렵다.
	실제로 시중의 RAII클래스들도 엄격한 캡슐화, 느슨한 캡슐화를 동시에 지원하는 것들이 꽤 있음
*/