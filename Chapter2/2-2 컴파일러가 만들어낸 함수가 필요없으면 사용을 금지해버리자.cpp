 


/*
	상황에 따라 어떤 객체는 사본을 만들어서는 안되는 경우가 있다. 원래 유일한 객체를 복사를 할 수 있게 되면 안되는 것.
	일반적으로 어떠한 기능을 하는 것을 막고 싶다면 해당 기능을 하는 함수 같은 것을 구현하지 않음으로써 막을 수 있는데, 
	복사 생성자. 복사 대입 연산자의 경우 자동으로 만들어지기 때문에 말이 달라진다.

	이를 해결하기 위한 방법으로, 자동으로 만들어지는 클래스의 복사 생성자, 복사 대입 연산자는 모두 public 
	접근 권한을 가진다는이 힌트가 된다. 이들이 꼭 public에 선언되어야 한다는 법은 없다. 일단 private에
	이들을 명시적으로 선언하면 자동으로 생성되는 것을 막음과 동시에 외부로부터의 호출을 막을 수 있게 된다.
	하지만 아직 friend함수로부터의 접근이 가능하다는 점에서 100%가 아닌데, 이 부분은 정의를 하지 않음으로써
	해결 할 수 있다. 선언만 하고 정의하지 않음으로써 해당 기능을 이용하려하면 링크 과정에서 에러가 뜨는 것을 통해
	막을 수 있는 것. 이 기법은 꽤나 잘 쓰일 수 있는 방법이고, C++라이브러리에서도 복사 방지책으로 쓰이고 있다.
*/
class HomeForSale
{
private :
	HomeForSale(const HomeForSale&);					
	HomeForSale& operator=(const HomeForSale&);			//매개변수들은 읽기 편하라고 해주는 관습일뿐 신경쓸 필요없다.
};
/*
	이제 객체를 복사하려 한다면 컴파일러가, 프렌드 함수가 복사하려 한다면 링커가 막아줄 것. 
	추가적으로 링크 시점의 에러를 컴파일 시점 에러로 옮길 수 있는데(조금 더 미리 에러를 탐지)
	방법은 복사 생성자와 복사 대입 연산자를 private 접근 권한으로 하는 것은 유지하고, 클래스를 분리한 뒤,
	그 클래스를 기본 클래스로, HomeForSale 클래스를 파생클래스로 하는 것. 기본 클래스가 복사를 막는 역할만 한다는 것.
	상속 할 때는 private 접근 권한으로 상속.*/

class Uncopyable
{
protected:
	Uncopyable() {}		//생성과
	~Uncopyable() {}	//소멸은 허용

private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);	//복사는 방지
};
class HomeForSale2 : private Uncopyable {};		//이것으로 복사 HomeForSale 클래스의 복사를 방지할 수 있다.

/*
	컴파일러가 생성한 복사 함수는 기본 클래스의 대응 버전을 호출하게 되어 있는데, 여기서부터 막히게 되어 복사 방지

	추가적으로 알아두면 좋은 점들 (복사를 막는 역할을 하는 기본 클래스에 대해)
		1. 해당 기본 클래스로부터의 상속은 public일 필요가 없다. 
		2. 해당 기본 클래스의 소멸자는 가상 소멸자가 아니어도 된다.
		3. 해당 기본 클래스는 데이터 멤버가 전혀 없기 때문에 이후 나올 공백 기본 클래스 최적화 기법이 통할 수 있다.
			다만 해당 클래스는 기본 클래스 이기 대문에 다중 상속으로 갈 여지가 있다.
		4. 다중 상속시 공백 기본 클래스 최적화가 돌아가지 못할 때가 종종 있다.
		5. 부스트 라이브러리를 보면 해당 클래스와 똑같은 구실은 하는 클래스를 찾을 수 있는데 그것을 사용해도 된다.
			이름은 noncopyable.
*/
	

