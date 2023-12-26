#include "pch.h"

/*
	STL���� �⺻������ �����̳�, �ݺ���, �˰����� ���ø����� �����Ǿ� ������
	�� �ܿ� ��ƿ��Ƽ��� �Ҹ��� ���ø��� �� �� ��� �ִ�.
	�� ���� advence��� �̸��� ���ø��� �ִµ�, �� ���� �ϴ� ����
	������ �ݺ��ڸ� ������ �Ÿ���ŭ �̵� ��Ű�� ���̴�.
*/

template <typename IterT, typename DistT>		// iter�� d ������ŭ ���� ��Ų��.
void advance(IterT& iter, DistT d);				// d < 0�̶�� iter�� d���� ��ŭ ���� ��Ų��.

/*
	�� ���ø��� iter+d�� ������ ���� ���� �ݺ��ڿ��Դ� �ǹ� ���� ��������
	�׿� �ٸ� �ݺ��ڿ��Դ� ++�̳� --������ d�� �����ؼ� advence�� �����ؾ� �Ѵ�.

	�ݺ��� ����
	1. �Է� �ݺ���(Input Iterator)
		- �Է� ������ �б� ���� �����͸� ���� �������, istream_iterator�� ��ǥ�� �Է� �ݺ���
		- ������ ����, �ѹ��� �� ĭ�� �̵�, �ڽ��� ����Ű�� ��ġ���� �б⸸ ����, ���� �� �ִ� Ƚ���� �ѹ�.

	2. ��� �ݺ���(Output Iterator)
		- ��� ������ ���� ���� ���� �����͸� ���� �������, ostream_iterator�� ��ǥ�� ��� �ݺ���
		- ������ ����, �ѹ��� �� ĭ�� �̵�, �ڽ��� ����Ű�� ��ġ���� ���⸸ ����, �� �� �ִ� Ƚ���� �ѹ�.

	�� �� ������ �ݺ��ڴ� 5�� �ݺ����� ���� ����� �������� ���� �н� �˰��򿡸� ����� �� �� �ִ�.

	3. ������ �ݺ���(Forwoard Iterator)
		- �Է�, ��� �ݺ��ڰ� �ϴ� ���� �⺻������ �� �� �� �ִ�.
		- �ڽ��� ����Ű�� ��ġ���� �б�� ���� ��� ����. �׸��� ������ ����
		- ���� �н� �˰��� �������� ��� ����.
	STL�� ��Ģ������ ���� ���� ����Ʈ�� �������� ������ �����ϴ� ��� ���̺귯���� �ִ�.

	4. ����� �ݺ���(Bidirectional Iterator)
		- ������ �ݺ��ڿ� �ڷ� �� �� �ִ� ����� �߰��� ��.
		- STL�� list, set, multiset, map, multimap�� ���δ�.

	5. ���� ���� �ݺ���(Random Access Iterator)
		- ����� �ݺ��ڿ� "�ݺ��� ��� ����" ���� ����� �߰��� ��. -> �ݺ��ڸ� ������ �Ÿ���ŭ �̵���Ű�� ���� ��� �ð���
		- �� ��������� ������ ��� ����� ����ѵ�, �⺻ ���� �����͸� ������ ���� ���� �ݺ��ڸ� ������� ������.
		- STL�� vector, deque, string �� ���

	C++ ǥ�� ���̺귯������ �� �ټ� ���� �ݺ��� ���� ������ �ĺ��ϴµ� tag����ü�� ���ǵǾ� �ִ�.
*/

struct Input_Iterator_tag {};
struct Output_Iterator_tag {};
struct Forward_Iterator_tag : public Input_Iterator_tag {};
struct Bidirectional_Iterator_tag : public Forward_Iterator_tag {};
struct Random_Access_Iterator_tag : public Bidirectional_Iterator_tag {};
// ��Ӱ��谡 is-a���� Ȯ������.

/*
	������ ���� �ݺ��ں� ���̰� �ֱ� ������ advence�� �����ϴµ� ����ؾ� �� ���� �ִ�.
	�ٷ� �ٸ� �ݺ��ڵ��� ������ ���� ���ϴ� Ƚ����ŭ ������Ű�ų� ���ӽ��� ���� �ؾ�������
	���� ���� �ݺ��� ���忡�� ��� �ð��� ������ �����ϱ� ������ ���ظ� ���� ���̱� �����̴�

	���� ������ ���� ������ �ڵ带 ¥���� ���̴�.
*/

template<typename IterT, typename DistT>	//IterT�� �ݺ���, DistT�� �Ÿ�
void advance(IterT& iter, DistT d)
{
	if (/* iter�� ���� ���� �ݺ��ڶ��*/)
		iter += d;
	else	// ���� ���� �ݺ��ڰ� �ƴ� ���
	{
		if (d >= 0)
		{
			while (d--)
			{
				++iter;
			}
		}
		else
		{
			while (d++)
			{
				--iter
			}
		}
	}
}

/*
	�� �� ���� ������ �����Ϸ��� iterator�� � �ݺ������� Ȯ�� �� �� �־�� �Ѵ�.
	�̷� ��, ������ ���߿� � �־��� Ÿ���� ������ ��� ���ִ� ��ü�� "Ư�� ����"��� ���� �ִ�.

	Ư�� ������ C++ �� �̸� ���ǵ� ���������� �ƴϰ�, Ű���嵵 �ƴϴ�. �׳� �����̰�, ���� ����̴�.

	Ư�������� �Ƿ��� �� ���� �䱸������ �������� �ϴµ�, �⺻ ���� Ÿ�� ����� ���� Ÿ�Կ���
	��� ���ư����Ѵٴ� ���� �� �� �ϳ��̴�.
	��Ȯ�� �ǹ̴� Ư������ ����� ������ ���� �⺻ ���� Ÿ�Կ� ������ �� �־�� �Ѵٴ� ���̴�.

	�̰��� ����� �����غ��� � Ÿ�� ���� ��ø�� ���������δ� ������ �ȵȴٴ� ���� ����.
	�ᱹ � Ÿ���� Ư�������� �� Ÿ�� �ȿ� �ִ� ���� �ƴ϶� �ܺο� �־�� �Ѵٴ� ���̴�.

	�̸� �����ϴ� ���� ǥ������ ����� �ش� Ư�������� ���ø��� �� ���ø��� 1�� �̻��� Ư��ȭ ������ �ִ� ��.
	�ݺ����� ���
	iterator_traits ��� �̸����� �Ǿ� �ִ�
*/
template <typename IterT>
struct itarator_traits;				// �ݺ��� Ÿ�Կ� ���� ������ ��Ÿ���� ���ø�

/*
	itarator_traits �� ����ü ���ø��ε�(��� Ŭ������ ����������) Ư�������� �׻� ����ü�� �����ϴ� ���� �����̴�.
	��ó�� Ư�������� �����ϴµ� ����� ����ü�� ������ "Ư�� ���� Ŭ����"��� �θ���.

	itarator_traits<IterT> �ȿ��� IterT Ÿ�� ������ ���� iterator_category ��� �̸� typedef Ÿ����
	����Ǿ� �ִµ�  �� typedef Ÿ���� �ٷ� IterT�� �ݺ��� ���ָ� ����Ű�� ��.

	itarator_traits Ŭ������ �ݺ��� ���ָ� �� �κ����� ������ �����Ѵ�.
	1. ����� ���� �ݺ��� Ÿ�Կ� ���� ����.
		����� ���� �ݺ��� Ÿ������ �Ͽ��� iterator_category��� �̸��� typedefŸ����
		���ο� �� ���� �䱸�������� �д�. �� ��, typedef Ÿ���� �ش� �±� ����ü�� �����Ǿ�� �Ѵ�.
		���� ��� deque�� �ݺ��ڴ� ���� ���� �ݺ����̹Ƿ�, deque Ŭ������ �� �� �ִ� �ݺ��ڴ� ������ ����
*/

template</*...*/>
class Deque
{
public:
	class iterator
	{
	public:
		typedef random_access_iterator_tag iterator_category;
		//...
	};
};

//		list�� �ݺ��ڴ� ����� �ݺ����̱� ������ ������ ���� ���̴�.

template </*...*/>
class list
{
public:
	class iterator
	{
	public:
		typedef bidirectional_iterator_tag iterator_category;
	};
};

//		�� iterator Ŭ���� ���ο� ���� ��øtypedef Ÿ���� �Ȱ��� ����� ���� iterator_traits�̴�.
template<typename IterT>
struct iterator_traits
{
	typedef typename IterT::iterator_category iterator_category;
};

/*
		�� �ڵ�� ����� ���� Ÿ�Կ� ���ؼ��� �� ���ư����� �ݺ����� ���� Ÿ���� �������� ��쿡��
		���� �ȵ��ư���. ������ �ȿ� typedefŸ���� ��ø�ȴٴ� �ͺ��Ͱ� ���� ���� �ʱ� �����̴�.

	2. �ݺ��ڰ� �������� ����� ó��
		������ Ÿ���� �ݺ��ڸ� ó���ϱ� ���ؼ�, itarator_traits�� ������ Ÿ�Կ� ����
		�κ� ���ø� Ư��ȭ ������ �����ϰ� �ִ�. �������� ���ۿ����� ���� ���� �ݺ��ڿ� ���� ������
		itarator_traits�� �̷� ������ �����ϴ� �ݺ��� ���ְ� �ٷ� ���� ���� �ݺ����̴�.
*/

template<typename IterT>
struct iterator_traits<IterT*>
{
	typedef typename IterT::iterator_category iterator_category;
};

/*
	Ư������ Ŭ������ ����� ������ ���� �����غ���.
	- �ٸ� ����� ����ϵ��� �����ְ� ���� Ÿ�� ���� ������ Ȯ���Ѵ�.
	- �� ������ �ĺ��ϱ� ���� �̸��� �����Ѵ�(�� iterator_category)
	- �����Ϸ��� Ÿ�� ���� ������ ���� ���ø��� ���ø� Ư��ȭ�� ���� �����Ѵ�.

	�̸� ������� advance�� �ǻ��ڵ带 �������� ������ ����
*/

template <typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
{
	if (typeid(typename std::iterator_traits<IterT>::iterator_category) == typeid(std::random_access_iterator_tag))
	{
		//...
	}
}

/*
	������ ������ �ִµ� IterTŸ���� ������ ���߿� �ľǵǱ� ������
	iterator_traits<IterT>::iteator_category �� �ľ��� �� �ִ� ���� ���� ������ �����̴�.
	������ if���� ���൵�߿� �򰡵ȴ�.
	������ ���߿� �� �� �ִ� ���� ���൵�߿� �ϴ� ���� �ð� ��������, �����ڵ��� ���ȭ�� �ҷ��´�.

	�̷� �� �ʿ��� ���� �ٷ� �����ε��̴�.
	������ Ÿ�ӿ� Ÿ�Կ� ���� ���õǴ� ���� ó�� ���� ��Ҹ� ������ ����.
	���� advance�� "���� ���� �˸���"�� �Ȱ��� �ϰ�,. �޾Ƶ��̴� iterator_category��ü�� Ÿ���� �ٸ��� �ؼ�
	�����ε� �Լ��� �����.
*/

// �ݺ��ڿ� ���ؼ����� ���ø�
template<typename IterT, typename DistT>
void DoAdvance(IterT& iter, DistT d, std::random_access_iterator_tag)
{
	iter += d;
}

// ����� �ݺ��ڿ� ���ؼ� ���� ���ø�
template<typename IterT, typename DistT>
void DoAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag)
{
	if (d >= 0)
	{
		while (d--)
			++iter;
	}
	else
	{
		while (d++)
			--iter;
	}
}

// �Է� �ݺ��ڿ� ���ؼ� ���� ���ø�
template<typename IterT, typename DistT>
void DoAdvance(IterT& iter, DistT d, std::input_iterator_tag)
{
	if (d < 0)
	{
		throw std::out_of_range("Negative Distance");
	}
	while (d--)
		++iter;
}

/*
	forward_iterator_tag�� input_iterator_tag�κ��� ����� ���� ���̹Ƿ�
	input_iterator_tag�� �Ű������� �޴� DoAdvance�� ������ �ݺ��ڵ� ���� �� �ִ�.

	���� Advance�� ���� ���� �ݺ��� ���� Ÿ�� ��ü�� ���߾� DoAdvance�� ȣ�����ִ� ���̴�.
*/

template <typename ITerT, typename DistT>
void Advance2(IterT& Iter, DistT D)
{
	DoAdvance(Iter, D, typename std::iterator_traits<IterT>::iterator_category());
}

/*
	�����ϸ�, �۾���(Worker) ������ ���� �Լ� Ȥ�� �Լ� ���ø��� Ư������ �Ű������� �ٸ����Ͽ� �����ε��ϰ�,
	���ܵǴ� �ش� Ư�������� ���߾� �� �����ε� ������ �����Ѵ�.

	���� �۾��ڸ� ȣ���ϴ� ���۾���(Master) ������ ���� �Լ� Ȥ�� �Լ� ���ø��� �����.
	�� ��, Ư������ Ŭ�������� �����Ǵ� ������ �Ѱܼ� �۾��ڸ� ȣ���ϵ��� �����Ѵ�.

	iterator_traits �� iterator_category���� �� ������ �� �ִ�. ��ǥ�����δ� value_type�� �ִ�.
	���� ���� Ÿ�Կ� ���� ������ ��� �ִ� char_traits, ���� Ÿ�Կ� ���� ������ ��� �ִ� numeric_limits�� �ִ�.

*/