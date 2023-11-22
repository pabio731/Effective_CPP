#include<iostream>

/*
	���� ���谡 �� �� �ý����� ���� ��ü�� �����ϴ� �Լ��� �� �� ���� �ִ�.
	�� ������ �ٷ� ���� �����ڿ� ���� ���� �������̴�.(���� ���� ���� �Լ�)
	�ϴ� �츮�� �� �͵��� ����� ���� �ʾƵ� �����Ϸ��� ����� �ֱ� �����ε�, ��Ȳ�� ���󼭴�
	�츮�� ���� ������ �ϴ� ��쵵 �ִ�. ������ ������ �츮�� �ǵ��Ѵ�� ����� ���� �ʴ���
	������ ���� �ʴ� �ٴ� ���̴�. �Ʒ� ���ø� ����. ���� ��Ÿ���� Ŭ������ �ְ�,
	�����ڰ� ���� ������ ���� �Լ��� �ִ�. ���� �Լ��� ȣ���� �� ���� �α׸� ���⵵�� �ۼ�.
*/

void LogCall(const std::string& FuncName);	// �α� ����� ����

class Customer
{
public:
	//...
	Customer(const Customer& rhs);
	Customer& operator=(const Customer& rhs);
	//...
private:
	std::string Name;
};

Customer::Customer(const Customer& rhs) : Name(rhs.Name) //rhs�� �����͸� ����
{
	LogCall("Customer copy constructor");
}

Customer& Customer::operator=(const Customer& rhs)
{
	LogCall("Customer copy assignement operator");
	Name = rhs.Name;		//rhs�� �����͸� ����
	return *this;
}
//������� ���� ���� ���δ�.
class Date {/*...*/ };	// ��¥ ������ ���� Ŭ����

class Customer2
{
public:
	//...			//���� ����
private:
	std::string name;
	Date LastTransaction;		//������ ����� �߰�
};

/*
	�̷��� �ǰ� ����,(������ ����� �߰��Ǹ鼭) ���� �Լ��� ������ ���� ���簡 �ƴ϶� �κ� ���簡 �ȴ�. ���� Name�� ����������
	LastTransaction�� ��������x, ���⼭ ���� �ؾ��� ���� �����µ�, �� ��Ȳ�� ���� �����Ϸ��� �������� �ʴ´�.
	���� ������ ����� �߰� �ƴٸ�, �츮�� �ؾ� �� ����
	1. ���� �Լ��� �ٽ� �ۼ�
	2. ������ ����
	3. ��ǥ����operator=�Լ��� ��� �ٲ������.

	�Ʒ��� �̷��� �������� ���� ���α׷��Ӹ� ����ϰ� �������� ����� ���� (Ŭ���� ���)
*/

class PriorityCustomer : public Customer2
{
public:
	PriorityCustomer(const PriorityCustomer& rhs);
	PriorityCustomer& operator=(const PriorityCustomer& rhs);
	//...

private:
	int iPriority;
};

PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs) : iPriority(rhs.iPriority)
{
	LogCall("PriorityCustomer copy construction");
}
PriorityCustomer& PriorityCustomer::operator=(const PriorityCustomer& rhs)
{
	LogCall("PriorityCustomer copy assignment operator");
	iPriority = rhs.iPriority;
	return *this;
}

/*
	PriorityCustomer Ŭ������ �����Լ��� ��� ���⿣ PriorityCustomer�� ��� ���� �����ϰ� �ִ� ��ó�� ��������,
	Customer�κ��� ����� ������ ������� �纻�� PriorityCustomerŬ������ �� �ִµ� �̵��� ���簡 �ȵǰ� �ִ�.
	PriorityCustomer�� ���� �����ڿ��� �⺻ Ŭ���� �����ڿ� �ѱ� ���ڵ鵵 ��õǾ� ���� �ʾƼ� Customer�� �κ���
	�⺻ �����ڿ� ���� �ʱ�ȭ�ȴ�. �翬�� �� �����ڴ� Name, LastTransaction�� ���� '�⺻����' �ʱ�ȭ�� ���ٰ�.

	������ PriorityCustomer�� ���� ���� �����ڴ� �⺻ Ŭ���� �κ��� �ǵ帮���� �ʴ´�.
	��·�� ���� �����Լ��� ���� ����� �ȴٸ� �⺻ Ŭ���� �κ��� ���߸��� �ʵ��� ������ �����ؾ� �Ѵ�.
	���� �⺻ Ŭ���� �κ��� private ����� ���ɼ��� ���� ������ ���� �ǵ帮�� ��ư�, �Ļ� Ŭ������ ���� �Լ� �ȿ���
	�⺻ Ŭ������ ���� �Լ��� ȣ���ϵ��� ����� �ȴ�.
*/

PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs) : Customer2(rhs), iPriority(rhs.iPriority)
{		//�ʱ�ȭ ����Ʈ�� ���� �⺻ Ŭ������ ���� ������ ȣ��.
	LogCall("PriorityCustomer copy construction");
}
PriorityCustomer& PriorityCustomer::operator=(const PriorityCustomer& rhs)
{
	LogCall("PriorityCustomer copy assignment operator");
	Customer2::operator=(rhs);		//�⺻ Ŭ���� �κ��� ����
	iPriority = rhs.iPriority;
	return *this;
}

/*
	�߰��� �����Լ����� ������ �ٸ� ������ ȣ���ϴ� ����� ����� ���Ұ�. ���� �ڵ��� �ߺ��� �־
	�̸� ���ϰ� �ʹٸ� �ߺ��Ǵ� �κ��� �Լ��� ���� ���� ���� �̸� ȣ���ϵ��� �Ұ�.
	�̷� �뵵�� �Լ��� private ���� ������ ��찡 ����, init��¼�� �ϴ� �̸��� ���� ����.
*/