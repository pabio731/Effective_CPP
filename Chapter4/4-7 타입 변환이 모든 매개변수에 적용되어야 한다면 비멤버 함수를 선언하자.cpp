/*
"Ŭ�������� �Ͻ��� Ÿ�Ժ�ȯ�� �����ϴ� ���� �Ϲ������� ���� �����̴�"��� �Ϳ��� ���ܰ� �ִµ�
�ٷ� ���� Ÿ���� ���� ���̴�. ���� �������� ��Ÿ���� Ŭ������ �ִٸ�, �������� ��������
�Ͻ��� Ÿ�� ��ȯ�� ������ڰ� ����� ������ �� �ְ�, C++�� �⺻ Ÿ�� ��ȯ�� �׷��� �Ѵ�.
�׷��ٸ� �Ʒ� �ڵ带 ����
*/

class Rational
{
public:
	Rational(int iNumerator = 0, int iDenominatir = 1);		//�Ͻ��� ��ȯ�� ����ϱ� ���� explicit�� ������ ����
	int Numerator() const;		//���� �� �и� ���� ���ٿ� �Լ�
	int iDonominator() const;
};

/*
	�������� ��Ÿ���� Ŭ�����̴� ��Ģ������ �⺻������  �����ϰ� ���� ���̴�.
	operator* �Լ��� �������� ������ �������� ���� ������ ��� �Լ��� �ϴ°� ���ڴٴ� ������ ��(4-6 �� ������ ������� �ʰ�)
	�ڵ带 ®�ٰ� ��������
*/

class Rational
{
public:
	//...
	const Rational operator*(const Rational& rhs) const;
};
// ���� �������� ������ �� �ɰ��̴�.

Rational OneEighth(1, 8);
Rational OneHalf(1, 2);

Rational Result = OneEighth * OneHalf;	// ���� ����
Rational Result = Result * OneEighth;	// ���� ����.

//�׷� ���� ȥ���� ������?? int * double ����...

Rational Result = OneHalf * 2;