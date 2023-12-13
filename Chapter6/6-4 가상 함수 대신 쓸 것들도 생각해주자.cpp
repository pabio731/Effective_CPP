#include"pch.h"
#include<functional>
/*
	������ ���� ĳ���͸� Ŭ������ �����ϴ� �۾��� �ϰ� �ִٰ� ��������.
	ĳ������ ü���� �󸶳� ���Ҵ����� ��Ÿ���� �������� ��ȯ�ϴ� HealthValue��� ����Լ��� �����,
	�̸� �� ĳ���͸��� �ٸ��� ���� ���ִ� ���� ������ ���̴� �����Լ��� �����ϸ�, �⺻���� ü�¿� ����
	���� ������ �����Ѵٰ� ��������
*/

class GameCharacter
{
public:
	virtual int HealthValue() const;	// ĳ������ ü��ġ�� ��ȯ�ϴ� �Լ�. �Ļ� Ŭ������ �� �Լ��� ������ �� �� ����.
};
/*
	�ʹ��� �翬�� ���������� �� �ٸ� �ǹ̷δ� �̰��� �����̴�.
	���� �̿��� �ٸ� ���� ����� ������ ����

	1. �񰡻� �������̽� ���뱸�� ���� ���ø� �޼��� ����
		"���� �Լ��� �ݵ�� private����� �ξ�� �Ѵ�"��� '���� �Լ� �����'�� ������ �ڵ带 ����
		HealthValue�� public ��� �Լ��� �״�� �ε�, �񰡻� �Լ��� �����ϰ�, ���������δ� ���� ������ ���� private���� �Լ��� ȣ���ϴ� ��
*/

class GameCharacter2
{
private:
	virtual int DoHealthValue() const
	{
		int RetVal(0);
		//ĳ������ �ɷ�ġ ����� ���� �⺻ �˰��� ����
		return RetVal;
	}
public:
	int HealthValue() const
	{
		// "����"������ ����
		int RetVal = DoHealthValue();
		// "����"������ ����

		return RetVal;
	}
};

/*
		�̷� ����� �����̴�. public �񰡻� ��� �Լ��� ���� private���� �Լ��� ���������� ȣ���ϰ� ����� �������,
		�񰡻� �Լ� �������̽�(non-virtual interface:NVI)���뱸��� �Ѵ�.
		�� ���뱸�� ���ø� �޼���(Template Method)�� �Ҹ��� ���� ������ ������ C++������ ������ ��

		�� ����� ������ "����"���۰� "����"������ ������ �� �ִٴ� �Ϳ� �ִ�. ���� �Լ��� ȣ���ϱ� ���� � ������ �ϰ�,
		(���� ��� ���ؽ� ����̳� �α� ����� ����)
		���� �Լ��� ȣ���� �� �� ���¸� ���ִ� �۾��� �����ϴ�
		(���� ��� ���ؽ� ��� ������ �Լ��� ���� ������ ���� ����)

		private ���� �Լ��� �Ļ� Ŭ�������� ������ �� ���� ������ ȣ���� �� �� ����.
		������� ����� �ִ� ���� �ƴϴ�. ���� �Լ��� ������ �ϴ� ���� ������ ��� ������ ���ΰ��� �����ϴ� ���̰�,
		�����Լ��� ȣ���ϴ� ���� �� ������ ����� ������ �����ϴ� ��. �� ���� ���ɻ簡 ������ ��.

		NVI���뱸������ �Ļ� Ŭ������ �����Լ� �����Ǹ� ����ϱ� ������ � ����� ��� ������ ���ΰ��� �Ļ� Ŭ������
		���������� �Լ��� ���� ȣ���� �������� ���ϴ� ���� �⺻ Ŭ������ ���� �����̴� (�ؾ��ϴ� ���� ������ ���� �� �Լ��� ȣ���ϵ��� �⺻ Ŭ�������� �����������)
		�� ���뱸���� ���� �Լ��� �����ϰ� private�� �ʿ�� ����. � Ŭ���� ������ ��� �Ļ� Ŭ�������� �����ǵǴ� ���� �Լ��� �⺻ Ŭ������
		���� �Լ��� ȣ���� ���� �����ϰ� ����� �͵� �ִµ� �� ��� ������ �Լ� ȣ���� �Ƿ��� protected������� �Ѵ�.

	2. �Լ� �����ͷ� ������ ��������
		�� NVI���뱸�� ���� �Լ��� ����� �� �ִ� ������ ���� ���������, Ŭ���� ������ �������� ���� �������̳� �ٸ�����.
		��·�� �����Լ��� ����ϱ� ������.
		���� �� ��������, ĳ������ ü��ġ�� ����ϴ� �۾��� ĳ������ Ÿ�԰� ������ ���� ���� ���� ���̴�.
		�� ����, �� ĳ������ �����ڿ� ü�� ���� �Լ��� �����͸� �ѱ�� �����, �� �Լ��� ȣ���ؼ� ����� �����ϵ��� �غ���
*/

class GameCharacter3;	//���漱��

int DefaultHealthCalc(const GameCharacter3& gc);

class GameCharacter3
{
public:
	typedef int (*HealthCalcFunc)(const GameCharacter3&);

	explicit GameCharacter3(HealthCalcFunc Hcf = DefaultHealthCalc)
		: HealthFunc(Hcf) {}

	int HealthValue() const { return HealthFunc(*this); }

private:
	HealthCalcFunc HealthFunc;
};

/*
		�� ����� ����(strategy)������ ������ ���̴�.
		���� �Լ��� ���� �ͺ��� ��մ� ���뼺�� �����µ�, ���� ĳ����Ÿ������ ������� ��ü�鵵 ü��ġ ��� �Լ��� ���� �ٸ��� ���� �� �ִٴ� ��
		���� �ڵ带 ����
*/

class EvilBadGuy :public GameCharacter3
{
public:
	explicit EvilBadGuy(HealthCalcFunc Hcf = DefaultHealthCalc)
		: GameCharacter3(Hcf) {}
};

int LoseHealthQuickly(const GameCharacter3&);
int LoseHealthSlowly(const GameCharacter3&);	//���� �ٸ� ���� ������ ������ ü��ġ ��� �Լ���

EvilBadGuy edg1(LoseHealthQuickly);
EvilBadGuy edg2(LoseHealthSlowly);		//���� Ÿ�������� ü��ġ�� �ٸ��� ������ ĳ���͵� (�ٸ� �Լ��� �ּҸ� �Ѱ��ֱ� ������)

/*
		������ ����Ǵ� ���߿� Ư�� ĳ���Ϳ� ���� �ɷ�ġ ��� �Լ��� �ٲ� �� �ִ�.
		���� ��� GameCharacterŬ�������� �ٸ� ü�� ��� ��� �Լ��� ��� �ִٸ� �̸� ���ؼ�
		���� ������� ü�� ��� �Լ��� ��ü�� ������ ��.

		������ ������ �����Ѵ�. ü��ġ ��� �Լ��� ��� �Լ��� �ƴϱ� ������ ���Ǵ� ��ü�� ����� �����Ϳ� ������ �ȵȴ�.
		��Ȯ�� ����� ���� public����� �ƴ� ������ ����ϴ� ��� ������ �ȴ�.
		public������ ���� �κ��� ���� �Լ��� ������ �� �ְ� �Ϸ��� �� Ŭ������ ĸ��ȭ�� ��ȭ ��Ű�� ����ۿ��� ���ٴ� ���� �Ϲ��� ��Ģ�̴�.
		�������� �����̵�, ���� �������׿� ���� �����ڸ� public ����� �����ϵ� ���̴�.
		������ �� �̵��ΰ��� ��Ȳ�� ���� �Ǵ��ؾ��Ѵ�.

	3. tr1::function���� ������ ���� ����

		�׷��ٸ� ü��ġ ����� �Լ��� ���� �ʵ��� �ϴ� �Ϳ� ���� �����غ���. �Լ��� �����Ѵٸ� �� ��� �Լ��� �ȵǳĴ� �ǹ��� ���� �� �ִ�.
		tr1::functionŸ���� ��ü�� �Ἥ ������ �Լ� �����͸� ����ϰ� ����� �ȴ�.
		tr1::functionŸ���� ��ü�� �Լ�ȣ�⼺ ��ü(callable entity)(�Լ� ������, �Լ� ��ü Ȥ�� ��� �Լ� ������)�� ���� �� �ְ�,
		�̵� ��ü�� �־��� �������� ����Ǵ� �ñ׳��Ŀ� ȣȯ�Ǵ� �ñ׳��ĸ� ������ �ִ�.
*/

class GameCharacter4;
int DefaultHealthCalc2(const GameCharacter4& gc);

class GameCharacter4
{
public:
	typedef std::tr1::function<int(const GameCharacter4&)> HealthCalcFunc;

	explicit GameCharacter4(HealthCalcFunc Hcf = DefaultHealthCalc2)
		:HealthFunc(Hcf) {}

	int HealthValue() const { return HealthFunc(*this); }
private:
	HealthCalcFunc HealthFunc;
};

/*
		HealthCalcFunc�� tr1::function�� ���ø��� �ν��Ͻ�ȭ �� �Ϳ� ���� typedefŸ���̴�
		��, �Ϲ�ȭ�� ������ Ÿ��ó�� �����Ѵٴ� ��.

		typedef std::tr1::function<int(const GameCharacter4&)> �� HealthCalcFunc�� typedef �� ���̰�,
		int(const GameCharacter4&)�� tr1::function�� �ν��Ͻ�ȭ �ϱ� ���� �Ű������� ���� ��� �ñ״����̴�.
		�� �ñ״��ĸ� �ؼ��ϸ� const GameCharacter4�� ���� �����ڸ� �ް� int�� ��ȯ�ϴ� �Լ��̴�.
		�̷��� ���ǵ� tr1::functionŸ���� ��ü�� ������ ��� �ñ״�ó�� ȣȯ�Ǵ� �Լ� ȣ�⼺ ��ü�� � �͵� ���� �� �ִ�.

		���⼭ "ȣȯ�ȴ�"��� ���ϴ� ���� �Լ���ü�� �Ű� ���� Ÿ���� const GameCharacter4& �̰ų� const GameCharacter4&��
		�Ͻ��� ��ȯ�� ������ Ÿ���̸�, ��ȯ Ÿ�Ե� �Ͻ������� int �� ��ȯ�� �� �ִٴ� ���̴�.

		���� ����� ���ؼ� ũ�� �ٸ� ���� ������ ���� GameCharacter4�� ���� �� �� �Ϲ�ȭ�� �Լ� �����͸� ���� �ƴٴ� ���̴�.
		������ �� ���� ��ȭ������ ���뼺�� ��û���� �Ҿ�����
*/

short CalcHealth(const GameCharacter4&);	// ��ȯŸ���� int�� �ƴ� ü�� ��� �Լ�

struct HealthCalculator
{
	int operator()(const GameCharacter4&) const {}	//ü�� ���� �Լ� ��ü�� ����� ���� Ŭ����
};

class GameLevel
{
public:
	float Health(const GameCharacter4&) const;		// ü��ġ ��꿡 ���� ��� �Լ�. ��ȯŸ���� int�� �ƴ�
};

class EvilBadGuy2 : public GameCharacter4
{
	explicit EvilBadGuy2(HealthCalcFunc Hcf = DefaultHealthCalc2)
		: GameCharacter4(Hcf) {}
};

class EyeCandyCharacter : public GameCharacter4
{
	explicit EyeCandyCharacter(HealthCalcFunc Hcf = DefaultHealthCalc2)
		:GameCharacter4(Hcf) {}
};

int main()
{
	EvilBadGuy2 ebg1(CalcHealth);		// ü�� ����� ���� �Լ��� ����ϴ� ĳ����

	EyeCandyCharacter ecc1(HealthCalculator()); //ü��ġ ����� ���� �Լ� ��ü�� ����ϴ� ĳ����

	GameLevel CurrentLevel;

	EvilBadGuy2 ebg2(std::tr1::bind(&GameLevel::Health, CurrentLevel, _1));
}

/*
		�� ���ǹ��� ���ϴ� �ٴ� ebg2�� ü���� ����ϱ� ���� GameLevel Ŭ������ Health ��� �Լ��� ����Ѵٴ� ���̴�.  �׸��� ��
		�Լ��� �Ű����� �ϳ��� �޴� ������ ����Ǿ� ������ �����δ� GameLevel ��ü�� �Ͻ������� �޾Ƶ��̱� ������ �� ���̴�.
		������ GameCharacter4 ��ü�� ���� ü�� ��� �Լ��� �޴� �Ű������� ü���� ���Ǵ� GameCharacter4 ��ü �ϳ����̴�.
		ebg2�� ü�� ��꿡 GameLevel::Health �Լ��� ������ �Ѵٸ� ��Ե� �Ű����� �ϳ��� �Լ��� �ٲ�� �Ѵٴ� ��.
		������ �ڵ忡���� ebg2 �� ü�� ��꿡 �� GameLevel ��ü�� CurrentLevel���� �� �����̹Ƿ�, GameLevel::Health �Լ���
		ȣ��� ������ CurrentLevel�� ���ǵ��� ������ ���̴�. �ٽø��� ebg2�� ü�� ��� �Լ��� �Ի� CurrentLevel ����
		GameLevel��ü�� ���ٰ� ������ ��.

	4. �������� ��������

		ü�� ��� �Լ��� ��Ÿ���� Ŭ���� ������ �ƿ� ���� �����, ���� ü�� ��� �Լ���
		�� Ŭ���� ������ ���� ��� �Լ��� ����� ��.
		GameCharacter�� ��� ������ �ֻ��� Ŭ�����̰� EvilBadGuy�� EyeCandyCharacter�� ���⼭ ������ ���� �Ļ� Ŭ�����̴�.
		���� HealthCalcFunc�� SlowHealthLoser �� FastHealthLoser���� �Ļ� Ŭ������ �Ŵ��� �ֻ��� Ŭ�����̴�.
		�߰��� GameCharacterŸ���� ������ ��� ��ü�� HealthCalcFuncŸ���� ��ü�� ���� �����͸� �����ϰ� �ִ�.
*/

class GameCharacter5;
class HealthCalcFunc
{
public:
	//...
	virtual int Calc(const GameCharacter5& gc) const {}
};

HealthCalcFunc DefaulthealthCalc;

class GameCharacter5
{
public:
	explicit GameCharacter5(HealthCalcFunc* phcf = &DefaulthealthCalc)
		:pHealthCalc(phcf) {}

	int healthValue() const { return pHealthCalc->Calc(*this); }
private:
	HealthCalcFunc* pHealthCalc;
};
/*
	�� ����� "ǥ������" ���� ���� ���� ����� ģ���� ��쿡 ���� ������ �� �ִٴ� ������ �ŷ����̴�.
	�Դٰ� HealthCalcFuncŬ���� ���뿡 �Ļ� Ŭ������ �߰������ν� ������ ü�� ��� �˰����� ����/������ �� �ִ� ���ɼ��� ������Ҵ�.
*/