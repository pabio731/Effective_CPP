#include "pch.h"

/*
	�츮�� ���ø��� ���� ��,
	template<typename T> class Widget
	template<class T> class Widget
	�� ���� ��� �����ϴ�.
	�׸��� �� ��� ��� ���̴� ���� ���� �� ���� ���� ��� �����ϴ�.
	������ �׷��ٰ� typename�� class �� ���Ǿ� ���� ���� �ƴϴ�.
	�и� typename���� ����ϴ� ��찡 �ִ�.

	�̸� �˱� ���ؼ��� ���ø� �ȿ��� �츮�� ������ �� �ִ� �̸��� ������ �� ������� �ͺ��� �˾ƾ� �Ѵ�.

	�Լ� ���ø��� �ְ�, �� ���ø��� STL�� ȣȯ�Ǵ� �����̳ʸ� �޾Ƶ��̰�, �� �����̳ʿ� ���� ��ü��
	int�� ������ �� �ִ� ��������. �׸��� �� ���ø��� �ϴ� ���� ��� ������ �� ��° ���� ����ϴ� ���̴�.
*/

template <typename C>
void Print2nd(const C& container)
{
	if (container.size() >= 2)
	{
		C::const_iterator iter(container.begin());

		++iter;						// �����̳��� ù ���� ���ҿ� ���� �ݺ��ڸ� ��� iter��
		int iValue = *iter;			// �� ��° ���ҷ� �ű�� �ٸ� intŸ�� ������ ���� �ű��
		std::cout << iValue;		// ���
	}
}	// �������� ���� �ʴ´�

/*
	 �� �Լ��ȿ��� �� ���� ���� iter�� iValue�� �ߺ���.
	 iter�� Ÿ���� C::const_iterator�ε�, ���ø� �Ű������� C�� ���� �޶����� Ÿ���̴�.
	 �̷��� ���ø� �Ű������� ���ӵ� ���� ������ "���� �̸�"�̶�� �Ѵ�.
	 �̷� ���� �̸��� Ŭ���� �ȿ� ��ø�Ǿ� �ִ� ��쵵 �ִµ�
	 ���� C::const_iterator�� �׷��ϴ�. (��ø ���� �̸��̶� ����)

	 �������� iValue�� intŸ���̰�, ���ø� �Ű������� ��� ��� ���� Ÿ�� �̸��̴�.
	 �̷� �̸��� "������ �̸�"�̶�� �Ѵ�.

	 �׷��� �ڵ� �ȿ� ��ø ���� �̸��� ���� ��, ������ �߻��� �� �ִ�.
	 �ٷ� �����Ϸ��� ���� �м��� �ַλ����� ����ٴ� ���̴�.
	 �� �ش����� ��Ȳ�� �����ؼ� �Ʒ� �ڵ带 ����
*/
template <typename C>
void Print2nd2(const C& Container)
{
	C::const_iterator* X;
}
/*
	��� ���� C::const_iterator�� ���� ������ �������� X�� ������ ��ó�� ���δ�.
	������ �̰��� �츮�� C::const_iterator�� Ÿ���̶�� ����� �츮 �ΰ��� �˱� �����̴�.
	���� ������ Ÿ���� �ƴ϶�� �����Ѵٸ� ���� �ڵ�� ����
	C::const_iterator�� X�� ���� ������ �Ϳ� �Ұ��ϰ�, ������ �����Ϸ��� �׷��� �۵��� ���̴�.
	�翬�� const_iterator��� �̸��� ������ �����ϴ� ����� �������� �߿��� ����
	�����Ϸ��� C::const_iterator�� Ÿ���̶�� ���� �𸥴ٴ� ����̴�.
	�׸��� C++�� ��ȣ���� �ذ��ϱ� ���� ���� ��Ģ�� �ִµ�, �ٷ� ���� �м���� ���ø� �ȿ���
	��ø ���� �̸��� ������ �⺻������ Ÿ���� �ƴ� ������ �ؼ��Ѵٴ� ���̴�.

	�ذ� ����� �����ϴ�. �� C::const_iterator�� Ÿ���̶�� ����� �����Ϸ����� �˷��ִ� ���̴�.
*/

template <typename C>
void Print2nd3(const C& Conatainer)
{
	if (Container.size() >= 2)
	{
		typename C::const_iterator iter(Conatainer.begin());
	}
}

/*
	typename Ű����� ��ø ���� �̸��� �ĺ��ϴ� �� ����Ѵ�.  �� �ܿ��� ���� �ȵȴ�.
	���� �� � �����̳ʿ� �����̳� ���� �ݺ��ڸ� �Ѳ����� �޾Ƶ��̴� �Լ� ���ø��� ������ ���� ���� ����̴�.
*/
template<typename C>
void f(const C& Container					// typename ���� �ȵȴ�.
	, typename C::const_iterator iter);		// typename �� ��� �Ѵ�.

/*
	C�� ��ø ���� Ÿ�� �̸��� �ƴϱ� ������(�������� � �͵� C�� ǰ�� ���� �ʱ� ������)
	typename�� �տ� ���̸� �ȵȴ�.

	�׷��� �� ��Ģ�� ���ܰ� �ִ�. �� ���ܴ�
	��ø ���� Ÿ�� �̸��� �⺻ Ŭ������ ����Ʈ�� �ְų� ��� �̴ϼȶ��������� �⺻ Ŭ���� �ĺ��ڷμ�
	�ִ� ��쿡�� typename�� �ٿ� �ָ� �ȵȴ�
*/

template<typename T>
class Derived : public Base<T>::Nested		//��ӵǴ� �⺻ Ŭ���� ����Ʈ -> typename ���� �ȵȴ�.
{
	explicit Derived(int x)
		: Base<T>Nested(x)					// ��� �̴ϼȶ������� �ִ� �⺻ Ŭ���� ��
	{
		typename Base<T>::Nested Temp;
	}
};

/*
	typename�� ���� ������ ���÷� �ݺ��ڸ� �Ű������� �޴� � �Լ� ���ø��� ����� �ִ� ��������.
	�ݺ��ڰ� ����Ű�� ��ü�� �纻�� Temp��� �̸��� ���� ������ ����� ���� ���� ��Ȳ�̴�.
*/
template<typename IterT>
void WorkWithIterator(IterT iter)
{
	typename std::iterator_traits<IterT>::value_type Temp(*iter);
}
/*
	std::iterator_traits<IterT>::value_type �� ���� C++ǥ���� Ư�� ����(traits) Ŭ������
	����� ���̸�, �ǹ̴� "IterT Ÿ���� ��ü�� ����Ű�� ����� Ÿ��"�̶�� ���̴�.
	�� IterT��ü�� ����Ű�� �Ͱ� �Ȱ��� Ÿ���� ��������(Temp)�� ������ ��, iter�� ����Ű�� ��ü��
	�� Temp�� �ʱ�ȭ �ϴ� �����̴�.
	���⼭ std::iterator_traits<IterT>::value_type�� value_type�� iterator_traits<IterT>�ȿ�
	��ø�Ǿ� �ְ�, IterT�� ���ø� �Ű������̱� ������ ��ø ���� �̸��̴�. ���� typename�� ����Ѵ�.

	�߰��� ����std::iterator_traits<IterT>::value_type�� �ʹ� ��� typedef�� ����� �ȴٸ� ��� �ؾ��ұ�?
	�ϴ� Ư������ Ŭ������ ���� value_type���� ����̸��� ���� typedef�̸��� ���� ���� �� ��� �̸���
	�Ȱ��� ���� ���� �����̴�
 */
template<typename IterT>
void WorkWithIterator2(IterT iter)
{
	typedef typename std::iterator_traits<IterT>::value_type value_type;
	value_type Temp(*iter);
}
/*
	typedef�� typename���·� ������ �ִ� ���� ���� ��ø ���� Ÿ�� �̸��� �����ϴ� ��
	���Ѿ��� ��Ģ ������ ���� �λ깰�� ���̴�. �������� ���ڰ� ������
	������ ����.

	�ٸ� typename�� ���ؼ� �˾Ƶξ�� �� ���� �����Ϸ����� typename�� ����
	��Ģ�� �󸶳� �����ϴ����� �� ���� �ٸ� �� �ִٴ� ���̴�.
	� �����Ϸ��� typename�� �ʿ��� ���� ���� �ʾƵ� �� �޾Ƶ��̰�,
	� �����Ϸ��� typename�� ������ �ʴ� ���� ������ �������� �Ѿ�⵵ �Ѵ�.
	���� typename�� ���� �� �����صΰ�, ��Ȳ�� ���� �� �������.
*/