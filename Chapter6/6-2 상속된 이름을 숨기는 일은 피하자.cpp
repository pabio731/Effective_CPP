#include"pch.h"

/*
	기본 클래스와 파생클래스를 다룰 때, 각 멤버 함수들에 대한 유효범위에 대해서 잘 신경써줘야한다.

	기본적으로 C++은 안쪽 유효범위에 있는 이름이 바깥쪽 유효범위의 이름과 같은면 이름을 가려버리기(덮기)때문.

	상속관계에서 기본 클래스에 속해 있는 것을 파생 클래스 멤버함수 안에서 참조하는 문장이 있으면 컴파일러는
	이 대상을 바로 찾아낼 수 있다. 기본 클래스에 선언된 것은 파생 클래스가 모두 물려받기 때문.
	다음 코드를 보자
*/

class Base1
{
private:
	int iTmp;
public:
	virtual void mf1() = 0;
	virtual void mf2();
	void mf3();
};

class Derived1 : public Base1
{
public:
	virtual void mf1();
	void mf4();
};

/*
	데이터 멤버와 멤버 함수의 이름이 public으로 공개되거나 private로 숨겨진 상태로 뒤섞여 있는 상황이다.
	그리고 mf4가 파생 클래스에서 구현되어 있다고 가정해보자
*/
void Derived::mf4()
{
	//...
	mf2();
}
/*
	컴파일러는 이 함수 안을 읽다가 mf2라는 이름이 쓰이고 있다는 것을 발견하게 된다면 이 이름이 어느 것에 대한
	이름인지를 파악하는 것이 가장 급선무이다.
	이름의 출처를 파악하기 위해 현재의 유효범위에서 찾아본다. 위에 보듯 mf2가 없기 때문에 다음으로
	mf4를 감싸고 있는 유효범위를 본다. (Derived의 유효범위가 그것) 하지만 이번에도 없기 때문에
	다음으로 Derived 클래스를 감싸고 있는 유효범위, 즉 Base클래스의 유효범위롤 옮겨간다.
	여기서 mf2를 찾아내고 끝날 것. 만약 여기서도 없다면 Base를 둘러싸고 있는 이름 영역, 다음으로
	전역 유효범위에서 찾을 것이다.

	여기서 추가로 mf1과 mf3를 오버로딩하고, mf3d의 오버로딩 버전을 Derived에 추가해보자.
*/

class Base2
{
private:
	int iTmp;

public:
	virtual void mf1() = 0;
	virtual void mf1(int);		//함수 오버로딩

	virtual void mf2();

	void mf3();
	void mf3(double);			//함수 오버로딩
};

class Derived2 : public Base2
{
public:
	virtual void mf1();
	void mf3();
	void mf4();
};
/*
	위 상황을 보면 기본 클래스의 함수중 mf1과 mf3라는 이름이 붙은 것은 모두 파생 클래스에 있는 mf1과 mf3에 의해 가려지게 된다.
	즉 이름 탐색의 시점에서 Base::mf1과 Base::mf3은 Derived가 상속한 것이 아니게 되는 것.
*/
int main()
{
	Derived2 d;
	int iDst;

	d.mf1();		// 가능. Derived::mf1을 호출
	d.mf1(iDst);	// 불가. Derived::mf1이 Base::mf1을 가림

	d.mf2();		// 가능 Base::mf2가 호출됌.

	d.mf3();		// 가능 Derived::mf3가 호출
	d.mf3(iDst);	// 불가. Derived::mf3이 Base::mf3을 가림
}
/*
	위에서 보듯 이름 가리기는 기본 클래스와 파생 클래스에 있는 이름이 같은 함수들이 매개 변수가 다르더라도 예외없이 적용된다.
	심지어 함수들이 가상함수인지, 비가상함수인지 여부에도 상관없이 이름이 가려진다.

	이는 어떤 라이브러리나 응용프로그램 프레임워크로를 이용하여 파생 클래스 하나 만들 때, 멀리 떨어져 있는 기본 클래스로부터
	오버로딩 버전을 상속시키는 경우를 막겠다는 것.

	public상속을 하면서 오버로딩 함수를 상속받지 않겠다는 것도 엄연히 is-a관계 위반이다.
	이런 가려진 이름을 사용하는 것은 using 을 이용하면 해결할 수 있다.
*/

class Base3
{
private:
	int iTmp;

public:
	virtual void mf1() = 0;
	virtual void mf1(int);
	virtual void mf2();
	void mf3();
	void mf3(double);
};

class Derived3 : public Base3
{
public:
	using Base3::mf1;
	using Base3::mf3;

	virtual void mf1();
	void mf3();
	void mf4();
};
// 이제 우리가 예상했던 대로 돌아가는 상속이 되었다.
int main()
{
	Derived3 D;
	int iTmp;

	D.mf1();
	D.mf1(iTmp);

	D.mf2();

	D.mf3();
	D.mf3(iTmp);	//이제 모두 가능
}

/*
	어떤 기본 클래스로부터 상속을 받으려고 하는데, 오버로딩된 함수가 그 클래스에 들어 있고, 이 함수들 중 몇개만 재정의 하고 싶다면
	각 이름에 대해 using선언을 붙여주어야 한다는 것.

	기본 클래스가 가진 함수 전부 상속했으면 하는 것이 아닌 경우도 있다. 물론 이 경우와 public상속은 함께 놓고 생각하면 안된다.
	기본 클래스와 파생 클래스의 is-a관계가 깨지기 때문이다. (using선언이 파생 클래스의 public영역에 있어야 하는 이유도 이것 때문)

	Derived가 Base로부터 private상속이 이루어졌다고 가정하자. 그리고 Derived가 상속했으면 하는 mf1함수는 매개변수가 없는 버전 하나밖에 없다고 치자.
	이 때는 using선언으로 해결할 수 없다. 이유는 using을 사용하면 그 이름의 모든 함수들이 파생 클래스로 내려가버리기 때문.
	이 때 필요한 것은 바로 간단하게 만든 전달 함수이다.
*/

class Base4
{
public:
	virtual void mf1() = 0;
	virtual void mf1(int);
};

class Derived4 : private Base4
{
public:
	virtual void mf1() { Base4::mf1(); }	// 전달함수. 암시적 인라인함수
};
int main()
{
	Derived4 d;
	int iTmp;

	d.mf1();		//Derived의 mf1이 호출
	d.mf1(iTmp);	//Base::mf1()은 가려져 있음.
}