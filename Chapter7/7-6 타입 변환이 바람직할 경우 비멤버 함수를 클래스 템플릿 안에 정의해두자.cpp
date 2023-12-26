#include "pch.h"

/*
	�ռ� ����� Ÿ�� ��ȯ�� �ǵ��� ����� ���ؼ��� ���� �Լ��ۿ� ����� ���ٰ� ������� �ִ�.

	���⼭ �ణ�� ���̷Ḧ ���ؼ� ��� ���÷� ���� ������ Ŭ���� Rational�� �Լ� operator*��
	���ø����� ����� ����
*/

template <typename T>
class Rational
{
public:
	Rational(const T& Numerator = 0, const T& Denominator = 0);
	const T Newmerator() const;
	const T Denominator() const;
};

template <typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {/*...*/ }

/*
	ȥ���� ��ġ ������ �ʿ��ϱ� ������ operator�� ������ ����� ���� �Լ��� �߰�,
	���⼭ ���ø�ȭ�� �ߴ�. �׷��� ������ �ִ�.
*/

Rational<int> OneHalf(1, 2);	// ���ø��̶�� �͸� �����ϸ� ���� �ڵ�
Rational<int> Result = OneHalf * 2;	//�׷��� ���⼭ ������ ������ �����. (�ǿ����� ���� ��ġ ����)

/*
	���ø� ������ �����ø� ������ ���̴� �ٷ� �����Ϸ��� ȣ���Ϸ��� �ϴ� �Լ��� �������� �˰� �ִ��Ĵ� ���̴�.
	���� ���� �����Ϸ��� ��� �Լ��� ȣ���Ϸ��� �ϴ��� �� �� �ִ� ����� ���� ����.

	�ϴ� OnHalf�� operator*�� ù ��° �Ű������� Rational<T> Ÿ���̰� operator�� �Ѱ����� �Ű����� ����
	Rational<int>�̱� ������ �����Ϸ��� ���� �����Ѵ�. ������ �� ���� �Ű������� intŸ���ε� �����Ϸ���
	T�� ��ü�� �߷����� ���Ѵ�. ���� �ڵ忡�� �����ڰ� explicit�� �ƴѵ�, �׷��ٰ� �����ڸ� �̿��ؼ�
	�Ͻ��� Ÿ�� ��ȯ�� ���������� �ʴ�. ���ø� ���� ���� ���������� �Ͻ��� Ÿ�� ��ȯ�� ������� �ʱ� ����.

	�� ����� �ذ��ϱ� ���ؼ��� �� operator* �Լ� ���ø��� friend �Լ��� ���� �ϴ� ���̴�.
*/

template<typename T>
class Rational2
{
public:
	friend const Rational2 operator*(const Rational2& lhs, const Rational2& rhs);
};
template<typename T>
const Rational2<T> operator*(const Rational2<T>& lhs, const Rational2<T>& rhs) {/*...*/ }

/*
	���� OneHalf ��ü�� Rational<int>Ÿ������ ����Ǹ� Rational<int> Ŭ������ �ν��Ͻ��� ���������
	�� ������ �Ϻημ� Rational<int> Ÿ���� �Ű������� �޴� ������ �Լ��� operator*�� �ڵ����� ����ȴ�.

	������ �ٸ��� ������ Ŭ���� ���ο� �Լ� ���ø��� �ƴ϶� �Լ��� ������ �� ���̹Ƿ� �����Ϸ���
	ȣ�⹮�� ���ؼ� �Ͻ��� ��ȭ �Լ��� ������ �� �ְ� �Ǵ� ��.

	������ �߰��� �ؾ� �� ���� ���Ҵµ� �� �ڵ带 ������ �ٽ� �������� �غ��� ��ũ�� ���� �ʴ´�.
	�� ������ operaotr* �� ���ø� Ŭ���� ���ο� ���� �Ǿ� �ִ� ������, ���� �Ǿ� �ִ� ���� �ƴϴ�.

	���� ������ ����� ���Ǹ� �ٿ��� �ذ��� �� �ִ�.
*/

template<typename T>
class Rational3
{
public:
	friend const Rational3 operator*(const Rational3& lhs, const Rational3& rhs)
	{
		return Rational3(lhs.Numerator() * rhs.Numerator()),
			lhs.Denominator()* rhs.Denominator());
	}
};

/*
	�̷��� ������ �ذ�ȴ�.

	���⼭ ��մ� �κ��� ������ �Լ��� �����ϱ� ������ Ŭ������ public���� ������ �ƴ�
	�ٸ� ���� ��� ������ ���� �ϱ� ���ؼ� ������ �Լ��� �� ���� �ƴ϶�� ���̴�.
	��Ȳ�� ���󼭴� private�� �����ϱ� ���ؼ� ������ �Լ��� ���� ���� �ƴ�
	�ٸ� ��쵵 ���� �� �ִٴ� ��.

	�߰��� ���� �ζ��� �Լ��� ���� ������ ���� �ʱ� ���ؼ� �� �� �ִ� �����
	������ �Լ��� ����̸� ȣ���ϰ� ����� ���̴�.
*/

template <typename T> class Rational4;

template <typename T>
const Rational4<T> DoMultiply(const Rational<T>& lhs, const Rational<T>* rhs);

template<typename T>
class Rational4
{
public:
	friend Rational4<T> operator* (const Rational<T>& lhs, const Rational<T>* rhs)
	{
		return DoMultiply(lhs, rhs);		// ������ �Լ��� ����� �Լ��� ȣ���ϰ� ����
	}
};

/*
	�߰��� ��ټ��� �����Ϸ����� ���ø� ���Ǹ� ��� ���Ͽ� ���� ���� ���� �����ϴ�
	�� �κ��� ���� �ʰ� ���� ������ָ�ȴ�.
	DoMultiply�� ���ø����ν� ȥ���� ������ �������� �ʰ�����
	�ش� ���ø��� operator* �� ����� ���̰�, operator*��
	ȥ���� ������ �����ϱ� ������ ��� ����.

*/