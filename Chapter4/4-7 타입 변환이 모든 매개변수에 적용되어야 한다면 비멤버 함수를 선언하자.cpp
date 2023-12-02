#include"pch.h"
/*
"Ŭ�������� �Ͻ��� Ÿ�Ժ�ȯ�� �����ϴ� ���� �Ϲ������� ���� �����̴�"��� �Ϳ��� ���ܰ� �ִµ�
�ٷ� ���� Ÿ���� ���� ���̴�. ���� �������� ��Ÿ���� Ŭ������ �ִٸ�, �������� ��������
�Ͻ��� Ÿ�� ��ȯ�� ������ڰ� ����� ������ �� �ְ�, C++�� �⺻ Ÿ�� ��ȯ�� �׷��� �Ѵ�.
�׷��ٸ� �Ʒ� �ڵ带 ����
*/

class Rational1
{
public:
	Rational1(int iNumerator = 0, int iDenominatir = 1);		//�Ͻ��� ��ȯ�� ����ϱ� ���� explicit�� ������ ����
	int Numerator() const;		//���� �� �и� ���� ���ٿ� �Լ�
	int iDonominator() const;
};

/*
	�������� ��Ÿ���� Ŭ�����̴� ��Ģ������ �⺻������  �����ϰ� ���� ���̴�.
	operator* �Լ��� �������� ������ �������� ���� ������ ��� �Լ��� �ϴ°� ���ڴٴ� ������ ��(4-6 �� ������ ������� �ʰ�)
	�ڵ带 ®�ٰ� ��������
*/

class Rational2
{
public:
	Rational2(int iNumerator = 0, int iDenominatir = 1);		//�Ͻ��� ��ȯ�� ����ϱ� ���� explicit�� ������ ����
	int Numerator() const;		//���� �� �и� ���� ���ٿ� �Լ�
	int iDonominator() const;
	const Rational2 operator*(const Rational2& rhs) const;
};
// ���� �������� ������ �� �ɰ��̴�.
int main()
{
	Rational2 OneEighth(1, 8);
	Rational2 OneHalf(1, 2);

	Rational2 Result = OneEighth * OneHalf;	// ���� ����
	Rational2 Result = Result * OneEighth;	// ���� ����.

	//�׷� ���� ȥ���� ������?? int * double ����...

	Rational2 Result = OneHalf * 2;
}