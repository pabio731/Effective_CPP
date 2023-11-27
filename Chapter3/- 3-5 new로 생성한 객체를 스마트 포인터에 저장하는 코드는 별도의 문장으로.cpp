#include<iostream>

int main() {
	/*
		처리 우선순위를 알려주는 함수가 하나 있고, 그 우선 순위에 따라 동적 할당한 객체를 처리하는
		함수가 있다고 가정하자.
	*/

	class Widget {};
	int Priority();	//우선순위를 알려주는 함수.
	void ProcessWidget(std::tr1::shared_ptr<Widget> pWw, int iPriority);

	/*
		자원 관리를 객체에 맡기라는 앞의 법칙에 따라서 process Widget함수는 동적 할당된 Widget객체에 대해
		스마트 포인터를 사용하도록 만들었다.
	*/
	ProcessWidget(new Widget, Priority()); //컴파일 에러
/*
	하지만 결과는 위에 보듯 에러가 난다. tr1::shared_ptr의 생성자가 explicit으로 선언되어 있기 때문에
	new Widget표현식에 의해 만들어진 포인터가 tr1::shared_ptr 타입의 객체로 바꾸는 암시적 변환이 안되기 때문.
	(explicit은 컴파일러가 암시적 변환을 하는 것을 막는 키워드)
*/
	ProcessWidget(std::tr1::shared_ptr<Widget>(new Widget), Priority());
	/*
		반면 위 코드는 에러가 나지 않는다, 하지만 이 코드 또한 자원의 누출에 대해서 걱정을 해야하는 코드이다.
		왜냐하면 컴파일러는 함수의 호출 코드를 만들기 전에 우선인자를 평가하는데, 두 번째 인자인 Priority함수는 호출로 끝나지만
		첫 번째 인자인 std::tr1::shared_ptr<Widget>(new Widget)은 두 부분으로 나누어져 있다.
		- new Widget 표현식의 실행
		- tr1::shared_ptr의 생성자를 실행
		따라서 함수 호출전에 컴파일러는 세 가지 연산을 위한 코드를 만들어야 한다.

		- Priority를 호출
		- new widget을 실행
		- tr1::shared_ptr 생성자 호출

		문제는 여기서, C++의 컴파일러는 C#이나 자바와 달리 실행 순서를 정하는게 자유롭기 때문에 생성된 객체가 스마트 포인터로 받아주지
		못할 수도 있다. 또한 Priority에서 예외 발생시는 어떻게 되는지 알기 어렵다.

		이를 해결하는 방법은 아주 간단하다.
		그냥 매개변수에서 스마트 포인터로 객체를 받는 것이 아닌.
		그전에 미리 스마트 포인터로 객체를 받고, 매개변수로 넘겨주자.
	*/
	std::tr1::shared_ptr<Widget> pW(new Widget);
	ProcessWidget(pW, Priority());		// 자원 누출의 걱정이 없는 코드
}