#include"pch.h"

/*
	생성자 혹은 소멸자를 끌고 다니는 타입으로 변수를 정의하면 반드시 생성자와 소멸자라는 비용을 지불하게 된다.
	이는 사용되지 않더라도 마찬가지이다.

	다음 함수를 보자 이 함수는 주어진 비밀번호가 충분히 길 경우, 해당 비밀번호르 암호화 하여 반환하는 함수.
	비밀번호가 너무 짧으면 logic_error타입의 예외를 던짐. (logic_error 타입은 표준 라이브 러리에 정의되어 있음)
 */

std::string EncryptPassword(const std::string& Password)
{
	using namespace std;

	string sEncrypted;	// 너무 일찍 정의된 변수

	if (Password.length() < MinimumPasswordLenth)
	{
		throw logic_error("Password is too short");
	}

	//...				주어진 비밀번호를 암호화하여 Encrypted 변수에 넣는데 필요한 일들을 여기서 함

	return sEncrypted;
}

/*
	이 함수에서 sEncrypted가 완전히 안쓰이진 않지만, 예외가 발생한다면 이 변수는 분명히 사용되지 않게 된다.
	다시 말해 함수 중간에 예외가 생겨도 sEncrypted객체의 생성과 소멸에 대해 비용을 내야한다는 것.
	따라서 이 변수를 꼭 필요해지기 전까지로 미루는 편이 낫다는 생각이 들 것.
*/

std::string EncryptPassword2(const std::string& Password)
{
	using namespace std;

	if (Password.length() < MinimumPasswordLenth)
	{
		throw logic_error("Password is too short");
	}
	string sEncrypted;	//필요할 때 변수를 정의

	//...				주어진 비밀번호를 암호화하여 Encrypted 변수에 넣는데 필요한 일들을 여기서 함

	return sEncrypted;
}

/*
	이제 위 변수의 초기화에 대해 생각해보자. 일반적으로 많이들 대입연산을 쓰지만
	클래스의 생성자 초기화 리스트를 쓰지 않고 생성자 함수 내에서 대입을 하는 것 처럼
	효율이 나쁘다.

	따라서 진정으로 바람직한 방법으로 sEncrypted를 Password로 초기화 해버려야 할것.
*/

std::string EncryptPassword3(const std::string& Password)
{
	using namespace std;

	if (Password.length() < MinimumPasswordLenth)
	{
		throw logic_error("Password is too short");
	}
	string sEncrypted(Password);	//필요할 때 변수를 정의와 동시에 초기화

	//...				주어진 비밀번호를 암호화하여 Encrypted 변수에 넣는데 필요한 일들을 여기서 함

	return sEncrypted;
}

/*
	이것이 "늦출 수 있는 데까지"의 진짜 뜻이 이것임.
	초기화 인자를 손에 넣기 전까지 정의를 늦출 수 있는가? 이렇게 하면 불필요한 생성자 호출을
	막을 수 있다.추가적으로 변수의 의미가 명확한 상황에서 초기화가 이루어지기 때문에, 쓰임새를 문서화 하는 데도 도움이 된다.

	그렇다면 루프에서는 어떤가?
	1. 루프의 밖에서 정의.	:	생성자 1번, 소멸자 1번, 대입 n번
	2. 루프의 안에서 정의.	:	생성자n 번, 소멸자 n번

	클래스중에는 대입에 들어가는 비용이 생성자 소멸자 쌍보다 적게 나오는 경우가 있는데,
	이 경우 1번 방법이 낫다. 차이는 n이 클수록 더 난다.

	다만 1번의 경우 w라는 이름을 볼 수 있는B의 경우보다 넓기 때문에 프로그램의 이해도와 유지 보수성이 안좋아질 수 있다.

*/