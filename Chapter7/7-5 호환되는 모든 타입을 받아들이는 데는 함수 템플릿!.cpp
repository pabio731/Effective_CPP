#include"pch.h"

/*
	����Ʈ �����ʹ� �׳� ������ó�� �����ϸ鼭�� ���� ���� ����� ������ �޷��ִ� ���� ���� ��ü�̴�
	������ ����Ʈ�����Ͱ� ��ü�� �� ���� �����͸��� Ư¡�� �ִµ�, �ٷ� �Ͻ��� ��ȯ�� �����Ѵٴ� ���̴�.
*/

class Top {/*...*/ };
class Middle : public Top {/*...*/ };
class Bottom : public Middle {/*...*/ };
Top* pt1 = new Middle;							//Middle* -> Top*�� ��ȯ
Top* pt2 = new Bottom;							//Bottom* -> Top*�� ��ȯ
const Top* pct2 = pt1;							//Top* -> const Top*�� ��ȯ

// �̸� ����� ���� ����Ʈ �����͸� �Ἥ �䳻������ ��ô ��ٷӴ�.
// ������ ���� �ڵ带 �����Ϸ��� ������ ���� �ʴ´�.

template <typename T>
class SmartPtr
{
public:
	explicit SmartPtr(T* RealPtr);		//����Ʈ �����ʹ� �밳 �⺻ ���� �����ͷ� �ʱ�ȭ�ȴ�.
};

SmartPtr<Top> pt1 = SmartPtr<Middle>(new Middle);		//Smart<Middle> -> Smart<Top>�� ��ȯ
SmartPtr<Top> pt2 = SmartPtr<Bottom>(new Middle);		//Smart<Bottom> -> Smart<Top>�� ��ȯ
SmartPtr<const Top> pct2 = pt1;							////Smart<Top> -> Smart<const Top>�� ��ȯ

/*
	���� ���ø����κ��� ������� �ٸ� �����Ͻ��� ���̿��� � ���赵 ���� ������
	�����Ϸ����� Smart<Middle>�� Smart<Top>�� ������ ������ Ŭ�����̴�.
	���� ��ȯ�� �ǵ��� ���� �������Ѵ�.

	������ �Լ��� ���� ����� ������ �츮���� �ʿ��� ��� �����ڸ� ������� �Ұ����ϴ�.
	���� ���뿡�� Middle, Bottom���κ��� Top�� ������ �� ������, Ŭ���� ������ �� Ȯ���Ѵٵ��� �ϸ�
	�ٸ� ����Ʈ ������ Ÿ�����κ��� SmartPtr<Top> ��ü�� ���� ����� ���õǾ�� �Ѵ�.
	�̷��� ������ ���� ������ SmartPtr���ø��� �����ؾ� �Ѵٴ� ��.

	��Ģ������ �츮�� ���ϴ� �������� ������ ���Ѵ��̴�. ���� �����ڸ� ����
	��� �Լ� ���ø��� ����.
	��� �Լ� ���ø��� ������ ���ؼ� � Ŭ������ �޸� �Լ��� ��� ���� ���ø��̴�.
*/

template <typename T>
class SmartPtr2
{
public:
	template <typename U>
	SmartPtr2(const SmartPtr<U>& Other);	//"�Ϲ�ȭ�� ���� ������"�� ����� ���� ��� ���ø�.
};

/*
	�� �ڵ带 �ؼ��ϸ� ��� TŸ�� �� U Ÿ�Կ� ���ؼ� SmartPtr<T> ��ü��
	SmartPtr<U>�κ��� ������ �� �ִٴ� ��.

	SmaerPtr<U>�� �����ڸ� �Ű������� �޾Ƶ��̴� �����ڰ� SmartPtr<T> �ȿ� ��� �ֱ� ����.
	�̷� ���¸� �Ϲ�ȭ ���� �����ڶ�� �θ��⵵ �Ѵ�.

	�� �Ϲ�ȭ ���� �����ڴ� explicit���� ���� ���� �ʾҴµ�, �̴� �⺻���� �����Ͱ�
	������ Ÿ�� ������ Ÿ�� ��ȯ�� �Ͻ������� �̷������ ĳ������ �ʿ����� �ʱ� ������
	����Ʈ �����͵� �̸� �䳻������ �ϱ� ���ؼ��̴�.

	�׷��� ���� �Ϲ�ȭ ���� �����ڴ� �츮�� ���ϴ� �� �̻��� ���� �� ���ִ�.
	�츮�� Bottom���κ��� Top�� ���� �� �ֱ⸸�� �ٶ��� Top���κ���
	Bottom�� ���� �� �ִ� �� ������ �ٶ��� �ʾұ� �����̴�.
	�� �ڵ�� ������ SmartPTr<double>�κ��� SmartPTr<int>�� ����� �ͱ��� �����ϱ� ������
	�̸� ������ �� �ִ� ���𰡰� �ʿ��ϴ�.

	tr1::shared_ptr���� ���� ����� �״�� ���� SmartPTr�� Get�Լ��� ����
	�ش� ����Ʈ ������ ��ü�� ��ü������ ��� �⺻���� �������� �纻�� ��ȯ�Ѵٰ� �����ϸ�
	�̰��� �̿��Ͽ� ������ �� �� �ִ�.
*/

template <typename T>
class SmartPtr
{
public:
	template<typename U>
	SmartPTr(const SmartPTr<U>& Other)		//�� SmartPTr�� ��� �����͸� �ٸ� SmartPTr�� ��� �����ͷ� �ʱ�ȭ
		:HeldPtr(Other.Get()) {/*...*/
	}
	T* Get() const { return Heldptr; }
private:
	T* HeldPtr;				// SmartPTr�� ��� �⺻ ���� ������
};

/*
	�̴ϼȶ������� ���ؼ� SmartPTr<T>�� ������ ����� T* Ÿ���� �����͸� SmartPTr<U>�� ����ִ� U*Ÿ���� �����ͷ� �ʱ�ȭ�Ѵ�.
	�̷��� �ϸ� U*���� T*�� ����Ǵ� �Ͻ��� ��ȯ�� ������ ���� ������ ������ ���� �ʴ´�.
	�� SmartPTr<T>�� �Ϲ�ȭ ���� �����ڴ� ȣȯ�Ǵ� Ÿ���� �Ű������� �Ѱܹ��� ���� ������ �ǵ��� ��������ٴ� ��.

	��� �Լ� ���ø��� Ȱ���� ������ �Ӹ� �ƴ϶� ���� ���꿡�� ���δ�
	���� ��� TR1�� shared_ptrŬ���� ���ø��� ȣȯ�Ǵ� ��� �⺻ ���� ������, tr1::shared_ptr, tr1::weak_ptr��ü��κ���
	������ ȣ���� �����ѵ��ٰ�, �̵� �� tr1::weak_ptr�� ������ �������� ���� ���꿡 �� �� �ֵ��� ������� �ִ�.
	�Ʒ��� tr1::shared_ptr���ø��� ����̴�.
*/

template <class T> class shared_ptr
{
public:
	template<class Y>
	explicit shared_ptr(Y* p);

	template<class Y>
	shared_ptr(shared_ptr<Y> const& r);

	template<class Y>
	explicit shared_ptr(weak_ptr<Y> const& r);

	template<class Y>
	explicit shared_ptr(auto_ptr<Y>& r);

	template<class Y>
	shared_ptr& operator=(shared_ptr<Y> const& r);

	template<class Y>
	shared_ptr& operator=(auto_ptr<Y>& r);
}; // ȣȯ�Ǵ� ��� �⺻���� ������, shared_ptr, weak_ptr, auto_ptr��ü�κ��� ������ȣ�� ����
  //  ȣȯ�Ǵ� shared_ptr, auto_ptr�κ��� ������ ����

/*
	�Ϲ�ȭ ���� �����ڸ� �����ϰ� ��� �����ڰ� explicit���� ����Ǿ� �ִ�.
	�̰��� shared_ptr�θ��� � Ÿ�����κ��� �� �ٸ� Ÿ������ ����Ǵ� �Ͻ��� ��ȯ�� ��������
	�⺻ ���� �����ͳ� �ٸ� ����Ʈ ������ Ÿ�����κ��� ��ȯ�Ǵ� ���� ���ڴٴ� ���̴�.

	��� �Լ� ���ø��� �Ǹ��� ��������� C++�� �⺻ ��Ģ�� �ٲ����� �ʴ´�.
	tr1::shared_ptr���� �и� �Ϲ�ȭ ���� �����ڰ� ����Ǿ� �ִµ�
	TŸ�԰� YŸ���� �����ϰ� ���´ٸ� �� �Ϲ�ȭ ���� �����ڴ� �и���
	������ ���� �����ڸ� ����� ������ �ν��Ͻ�ȭ �� ���̴�.

	�׷� tr1::shared_ptr��ü�� �ڽŰ� ������ Ÿ���� �ٸ� tr1::shared_ptr ��ü�κ���
	�����Ǵ� ��Ȳ���� �����Ϸ��� ����Ʈ ���� �����ڸ� ����� �ƴϸ�
	�Ϲ�ȭ ���� ������ ���ø��� �ν��Ͻ�ȭ �ұ�?

	���� "����Ʈ ���� �����ڸ� �����"�̴�.
	�Ϲ�ȭ ���� �����ڸ� �����ϴ� ���� �� ���ø� ���� �����ڸ� ����� ���� ���� ��Ұ� �ƴѰ�.
	�̴� ���� �����ڵ� ���������̴�.
	������ �̿� ������ tr1::shared_ptr�� ��� ó���ߴ��� �� �� �ִ� �ڵ��̴�.
*/
template <class T>
class shared_ptr
{
public:
	shared_ptr(shared_ptr const& r);	// ���� ������

	template <class Y>					// �Ϲ�ȭ
	shared_ptr(shared_ptr<Y> const& r);	// ���� ������

	shared_ptr& operator=(shared_ptr const& r);	// ���� ���� ������

	template <class Y>
	shared_ptr& operator=(shared_ptr<Y> const& r);	// ���� ���� ������
};