#include "pch.h"

/*
	� Ÿ���� ��ü���� �׿� �ٸ� Ÿ���� ��ü���� �����ϰ� ���� ��� �� ���踦 �θ��� ���� �ռ�, ���̾, ����, ����, ������� �ִ�.
	�̴� ��� ���� ���� ���ϸ� ������� �ٸ� ��.
	�Ʒ� �ڵ尡 �����̴�.
*/

class Address {};
class PhoneNumber {};
class Person
{
public:
	//...
private:
	std::string strName;
	Address addres;
	PhoneNumber VoiceNumber;
	PhoneNumber FaxNumber;
};

/*
	Person ��ü�� string, Address, PhoneNumber ��ü�� �̷���� �ִ�.

	�̷��� ��ü �ռ� ���� �ǹ̸� �����µ�, has-a ����(...��...�� ����) �Ǵ� is-implemented-in-terms-of (...��..�� �Ἥ ������)
	�� ������ �ϳ��� �ǹ̸� ���� �� �ִ�. �� ������ ����Ʈ���� ���߿��� �츮�� ���ϴ� ������ ���� �޶����µ�,
	���� �츮 �ϻ��Ȱ���� �� �� �ִ� �繰�� �� �� ��ü�� �����ϴ� ���� ����Ʈ������ ���뿵���̰�,
	�� ���� ����,���ؽ�, Ž�� Ʈ�� �� �����ϰ� �ý��� �������� ���� �ΰ����� ������ ���� ���� �����̴�.

	��ü �ռ��� ���� �������� �Ͼ�ٸ� has-a�����̰�
	��ü �ռ��� ���� �������� �Ͼ�ٸ� is-implemented-in-terms-of �����̴�.
��2
	���� ������ Person Ŭ������ ��Ÿ���� ����� has-a ����� �� �� �ִ�.
	Person��ü�� �̸��� �ּ�, ��ȣ�� ������ �ȴ�. ����� �̸��� �����̰ų� �ּ��� ������ �ƴϴ�.

	�򰥸� �� �ִ� �κ��� is-a ����� is-implemented-in-terms-of�����ε�
	���ø� ���� �˾ƺ���.
	��ü�� ������ set(�ߺ� ��� x)�� �ʿ��ѵ�, ��������� ���� �����ϴ� Ŭ���� ���ø����� �ʿ��ϴٰ� �����غ���
	���� ���� ǥ�� ���̺귯���� set�� ���ø��� �� ���ε�, set���ø��� Ž��, ����, ������ �ð� ���⵵�� ���� �ð� ���⵵��
	�ϱ� ���ؼ� ���� Ž�� Ʈ���� ��������� ������ ���� �� ���� ������ �����Ͱ� �־� ���� ������ ���� ���� �ϴ� ���ø����δ�
	����ġ �ʴ�.

	���� ���ø��� �׳� ����� �ȴٰ� �����ϸ�, ���� ����Ʈ�� ���ؼ� �����ڴ� ������ ���
	Set ���ø��� ���� ��, list���� �Ļ��� ���º��� �����ϵ��� ����� ��Ȳ ���� �� ���̴�.
	�� Set<T>�� list<T>�� ����ϴ� ��.
	�׷� �ᱹ Set ��ü�� list��ü�� ������ �Ǵ� ���̴�
*/
#include <list>
template <typename T>
class Set1 :public list<t >> {};

/*
	������ ���⼭ ������ �ִ�. is-a ���迡 ���� list���� ���� �͵��� Set������ ���̿��� ���ٵ�
	list�� �ߺ��� �����Ͱ� ���� �� �ִٴ� ���̴�.
	���� �̸� �ذ��ϱ� ���ؼ�
	�ռ� ���� is-implemented-in-terms-of���踦 �̿��ؼ�
	Set��ü�� list ��ü�� �Ἥ �����Ǵ� ���¸� ������
*/

template<typename T>
class Set
{
public:
	bool Member(const  T& Item) const;
	void Insert(const T& Item);
	void Remove(const T& Item);
	std::size_t size() const;

private:
	std::list<T> rep;				// Set �������� ���� ǥ����
};

/*
	Set�� ����Լ��� list���� �̹� �����ϴ� ��ɰ� ǥ�� C++���̺귯���� �ٸ� ���� ��Ҹ� �� ������ ����⸸ �ϸ�Ǳ� ������ �����ϴ�
*/

template<typename T>
bool Set<T>::Member(const T& Item) const
{
	return std::find(rep.begin(), rep.end(), Item) != rep.end();
}

template<typename T>
void Set<T>::Insert(const T& Item)
{
	if (!Member(Item))
		rep.push_back(iTem);
}

template<typename T>
void Set<T>::Remove(const T& ITem)
{
	typename std::list<T>::iterator it = std::find(rep.bigin(), rep.end(), Item);

	if (it != rep.end())
		rep.erase(it);
}

template<typename T>
std::size_t Set<T>::size() const
{
	return rep.size();
}

/*
	�� �ڵ� ���ø� ���ؼ� is-implemented-in-terms-of�� ���ؼ� �� ��������
*/