#include "pch.h"

/*
	아래 코드는 배경그림을 깔고 나오는 GUI메뉴를 구현하기 위해 만든 클래스이며, 스레딩 환경에서 동작하기위해
	병행성 제어를 위해 뮤텍스를 가지고 있다.
*/
class Image
{
public:
	Image(int) {}
	void Reset(Image tmp) {}
};
class PrettyMenu
{
public:
	//...
	void ChangBackground(std::istream& ImgSrc);	// 배경 그림을 바꾸는 함수
	//...

private:
	mutex		Mutex;
	Image* BgImgage;		//현재의 배경
	int			iImageChanges;	//배경을 바꾼 횟수
};
/*
	여기서 PrettyMenu의 ChangeBackground 함수가 다음과 같이 구현되었다고 생각해보자
*/

void PrettyMenu::ChangBackground(std::istream& ImgSrc)
{
	lock(&Mutex);					//뮤텍스를 획득

	delete  BgImgage;					//이전의 배경을 없앰.
	++iImageChanges;					// 그림 변경 횟수를 갱신함
	BgImgage = new Image; (ImgSrc);		// 새 배경을 깔아 놓음

	unlock(&Mutex);						//뮤텍스를 해제
}

/*
	위 함수는 "예외 안정성"측면에서 이보다 더 나쁠 수 없을 정도이다.

	일반적으로 예외 안정성을 위해서는 두 가지의 요구사항을 맞추어야 한다.
	1. 자원이 새지 안도록 만든다
	   위 코드는 자원이 샌다. 왜냐면 new에서 예외를 던지면 뮤텍스가 계속 잡힌 상태로 남기 때문

	2. 자료 구조가 더럽혀지지 않도록 만든다.
	   위 코드에서 new가 예외를 던지면 BgImage가 가리키는 객체는 이미 삭제된 후임.
	   또 새 그림이 제대로 깔린것이 아닌데도 imageChanges변수는 이미 증가 되었을 것이다.

	자원 누출 문제는 맞서 싸우기 그리 까다롭지 않다. 객체를 써서 자원 관리를 하게 하고,
	이전에 배운 내용을 살려 적절한 시점에 뮤텍스를 해제하면 되기 때문
*/

void PrettyMenu::ChangBackground(std::istream& ImgSrc)
{
	Lock m1(&mutex);			// 뮤텍스를 대신 획득하고, 이것이 필요 없어질 시점에 바로 해제해주는 객체

	delete BgImgage;
	++iImageChanges;
	BgImgage = new Image(imgSrc);
}

/*
	이와 같이 자원관리 전담 클래스를 만들면 가장 좋은 점 중 하나는 함수 코드 길이가 짧아진다는 것

	위 코드에서는 unlock을 호출할 필요가 없어졌다.

	이제 자료구조의 오염에 관해서인데, 먼저 예외 안전성을 갖춘 함수의 세가지 "보장"을 알아보자
	1. 기본적인 보장
	   함수 동작 중에 예외가 발생하면, 실행중인 프로그램에 관련된 모든 것들을 유효한 상태로 유지하겠다는 보장
	   어떤 객체나 자료구조가 더럽혀지지 않고, 모든 객체는 내부적으로 일관성을 유지하고 있다.

	   하지만 프로그램 상태가 어떤지 정확히 예측이 안될 수 있다. 예를 들어 위의 경우 예외가 발생했을 때
	   PrettyMenu객ㄱ체는 바로 이전의 배경을 그대로 계속 그릴수도 있고, 기본 배경그림을 사용할 수도 있을것.
	   전적으로 함수를 만든사람에게 달렸다.

	2. 강력한 보장
	   함수 동작 중, 예외가 발생하면, 프로그램의 상태를 절대로 변경하지 않겠다는 보장.
	   이런 함수를 호출하는 것은 원자적인(atomic) 동작임. 호출이 성공하면 마무리까지
	   완벽하게 성공하고 호출이 실패한다면 호출이 없었던 것처럼 상태가 돌아간다.

	   기본적인 보장을 해주는 함수보다 더 사용하기 쉽다.
	   함수가 성공적으로 실행을 마친 후의 상태와 함수가 호출될 때의 상태 두 가지의
	   상태만 존재 가능하기 때문.

	3. 예외불가 보장
	   절대로 예외를 던지지 않겠다는 보장.
*/
int DoSomething() throw();	//비어있는 예외지정
/*
	위의 함수 선언은 DoSomething이 예외를 던지지 않겠다는 것이 아님.
	만약 예외가 발생했다면 매우 심각한 에러가 생긴 것으로 판단되므로, 지정되지 않은 예외가 발생했을 경우
	실행되는 처리자인 unexpected 함수가 호출되어야 한다는 뜻.
	사실 DoSomething은 어떤 예외 안정성 보장도 제공하지 않을 수도 있음.
	함수의 특성을 결정하는 것은 결국 구현부이다.

	예외 안정성을 갖추기 위해서는 위 세 가지중 무조건 하나를 골라야 한다. 가장 이상적인 것은
	예외 불가 보장이지만 결국 기본적인 보장과 강력한 보장중 선택해야 할 것이며, 가장 실용성으로는 강력한 보장이다.

	다시ChageBackground 함수를 보자. 이 함수의 경우 강력한 보장을 제공하는 것이 어렵지 않다.
	우선 PrettymMenu의 BgImage데이터 멤버의 타입을 기본 제공 포인터 타입인 Image*에서
	자원관리 전담용 포인터로 바꾼다.
	다음으로 함수내의 문장을 재배하여 배경이 진짜로 바뀌어야만 iImageChanges를 증가시키도록 한다.
*/

class PrettyMenu2
{
	//...
	tr1::shared_ptr<Image> BgImage;
	//...
	int iiImageChages;
public:
	void ChageBackground(istream& ImgSrc);
};

void PrettyMenu2::ChageBackground(istream& ImgSrc)
{
	Lock m1(&mutex);
	BgImage.Reset(new Image(ImgSrc));	//BgImage 내부 포인터를 new Image 표현식의 실행 결과로 바꿔치기한다.

	++iImageChages;
}

/*
	이제 복사 후 맞바꾸기(copy-and-swap) 이라는 설계 전략을 알아보자
	이 것은 예외에 속수무책인 함수를 탈바꿈시켜 강력한 예외 안전성 보장을 제공하는 함수로 거듭나게 만드는 전략이다

	원리는 어떤 객체를 수정하고 싶다면 그 객체의 사본을 하나 만들어 놓고는 그 사본을 수정하는 것이다.
	이렇게 하면 수정 동작중 실행되는 연산에서 예외가 던져지더라도 원본 객체는 바뀌지 않은 채로 남는다.

	수정이 끝나면 원본과 맞바꾸는데, 이 작업을 "예외를 던지지 않는" 연산 내부에서 수행한다.

	이 전략은 대개 "진짜"객체의 모든 데이터를 별도의 "구현" 객체에 넣어두고, 그 구현 객체를 가리키는
	포인터를 진짜 객체가 물고 있게 하는 방식으로 구현한다. 이를 적용한다면
*/

struct PMImpl
{
	tr1::shared_ptr<Image> BgImage;
	int iImagechanges;
};

class PrettyMenu3
{
	//...

private:
	mutex Mutex;
	tr1::shared_ptr<PMImpl> pImpl;
public:
	void ChageBackground3(istream& ImgSrc);
};

void PrettyMenu3::ChageBackground3(istream& ImgSrc)
{
	using std::swap;
	lock(&Mutex);
	tr1::shared_ptr<PMImpl> pNew(new PMImpl(*pImpl));	//객체의 데이터 부분을 복사함.
	pNew->BgImage.Reset(new Image(ImgSrc));	//사본을 수정
	++pNew->iImagechanges;

	swap(pImpl, pNew);
}

/*
	PMImpl이 구조체로 만들어져 있는 이유는 PrettyMenu 클래스에서 pImpl이 private 멤버로 되어 있어서
	구현 객체의 데이터가 바로 캡슐화 되기 때문

	복사후-맞바꾸기 전략은 객체의 상태를 전부 바꾸거나, 혹승 안바꾸거나 방식으로 유지하려는 경우 아주 잘 맞다.
	그러나 함수 전체가 강력한 예외 안전성은 함수 전체가 강력한 예외 안전성을 갖도록
	보장하지 않는다는 것이 일반적인 정설이다. 왜인지는 ChangeBackground 함수의 전체 흐름을 추상화 해놓은
	SomeFunc()를 보자
*/
void SomeFunc()
{
	//..	이 함수의 현재 상태에 대한 사본을 만들어 놓는다
	f1();
	f2();
	//.. 변경된 상태를 바꾸너 넣는다
}

/*
	f1혹은 f2에서 보장하는 예외 앚전성이 "강력"하지 못하면 SomeFunc 함수에서 강력한 보장을 제공하기 힘들어짐.
	만약 f1이 기본적인 보장만 제공하고 있고, someFunc에서 강력한 보장을 제공하고자 한다면
	1. f1을 호출하기 전에 프로그램 전체의 상태를 결졍하고,
	2. f1에서 발생하는 모든 예외를 잡아낸 후에
	3. 원래의 상태로 되돌리는 코드 작성

	f1, f2모두가 강력한 예외 안정성을 보장한다고 해도 별로 나아지는 것은 없다.
	f1이 끝까지 실행되고 나면 프로그램 상태는 f1에 의해 변해있을 것이고,
	그 더음 f2가 실행되다가 예외를 던지면 그 프로그램의 상태는
	전과 아예 달라져 있을 것이기 때문.

	여기서 불거지는 문제가 바로 함수의 side effect임.
	자기 자신에만 국한된 것들의 상태를 바꾸며 동작하는 함수의 경우에는 강력한 보장을
	제공하기 수월하지만 비지역 데이터에 side effect를 주는 함수는 어떻게 하기 까다롭다.

	다만 이 복사후 맞바꾸기는 효율문제를 무시할 수 없다. 수정하고 싶은 객체를 복사해 둘 공간과
	복사에 걸리는 시간을 감수해야 한다. 하지만 어쨌든 실용성이 확보될 때는 강력한 보장이 좋으며,
	효율이나 복잡성을 고려해야 하는 경우에는 기본적인 보장을 쓰면 된다.

	예외 안전성을 가지지 않는 함수가 한 개라도 있다면 그 시스템은 전부가 예외에 안전하지 않은것.
	철저하게 예외에 안전한 코드를 만들자.

*/