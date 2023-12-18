#include "pch.h"
/*
	�ٸ� �� ���� ȸ��� �޼����� ������ �� �ִ� �����÷α׷��� ����ٰ� ��������.
	�� �޼����� ��ȣȭ�� �� �ְ�, ���ȣȭ ���°� �� �� �ִ�.
	���� � �޽����� � ȸ��� ���۵����� ������ ���߿� ������ �� �ִ� ����� ������ �ִٸ�
	���ø� ������� �ڵ带 © ���̴�
*/
class CompanyA
{
public:
	//...
	void SendClearText(const std::string& Msg);
	void SendEncrypted(const std::string& Msg);
	//...
};

class CompanyB
{
public:
	//...
	void SendClearText(const std::string& Msg);
	void SendEncrypted(const std::string& Msg);
	//...
};

class MsgInfo {/*...*/ };

template <typename Company>
class MsgSender
{
public:
	//...
	void SendClear(const MsgInfo& Info)
	{
		std::string Msg;
		//Info�κ��� Msg�� �����.

		Company C;
		C.SendClearText(Msg);
	}
	void SendSecret(const MsgInfo& Info)
	{
		std::string Msg;
		//Info�κ��� Msg�� �����.

		Company C;
		C.SendEncrypted(Msg);
	}
};

/*
	���⿡ ���ٿ��� �޼����� ���� ������ ���� ������ �α׷� ����� �ʹٸ�?
*/
template <typename Company>
class LoggingMsgSender : public msgSender<Company>
{
public:
	void SendClearMsg(const MsgInfo& Info)
	{
		// �޼��� ���� �� ������ �α׿� ���
		SendClear(info);                    // �⺻ Ŭ������ �Լ��� ȣ���ϴµ�, �� �ڵ尡 �������� ���� �ʴ´�!
		// �޼��� ���� �� ������ �α׿� ���
	}
};

/*
	�Ļ� Ŭ������ �ִ� �޼��� ���� �Լ��� �̸�(SendClearMsg)�� �⺻ Ŭ������ �Լ�(SendClear)��
	�ٸ��ٴ� ���� ���δ�. �⺻ Ŭ�����κ��� �������� �̸��� �Ļ� Ŭ�������� ������ ������ ����,
	��ӹ��� �񰡻� �Լ��� ������ ���� �ʾƾ� �Ѵٴ����� �� ������ ���̴�.
	������ �������� ���� �ʴ´�.
	�⺻ Ŭ�������� SendClear �Լ��� ������ �ִµ��� ���� �ʴ´�.

	������ �����ϴ�. �����Ϸ��� LoggingMsgSender Ŭ���� ���ø��� ���ǿ� ����ĥ ��,
	�����Ϸ��� ��ü �� Ŭ������ ��� �Ļ��� �������� �𸥴ٴ� ���̴�.
	MsgSender<Company>�� ���� �и� ������. Company�� ���ø� �Ű������̰�,
	�� ���ø� �Ű������� LoggingMSgSender�� ��ü�� ������� ������ ������ ���� �� �� ����.
	Company�� ��Ȯ�� �������� �𸣴� ��Ȳ������ MsgSender<Company> Ŭ������ � �������� �� ����� ����.

	���� ���� ����
	CompanyZ��� Ŭ������ �ְ� �� Ŭ������ ��ȣȭ�� ��Ÿ��� ����ؾ� �Ѵ�.
*/

class CompanyZ  // �� Ŭ������ SendClearText�Լ��� �������� �ʴ´�.(��ȣȭ�� ��Ÿ� ����ϹǷ�)
{
public:
	void SendEncrypted(const std::string& Msg);
	//...
};
/*
	�Ϲ��� MsgSender ���ø��� �״�� CompanyZ Ŭ������ ���⿣ �� �׷���.
	�� ���ø��� CompanyZ��ü�� ���� ö�а� ���� �ʴ� SendClear�Լ��� �����ϱ� �����̴�...
	���� CompanyZ�� ���� MsgSender�� Ư��ȭ ������ ���� �� �ִ�.
*/

template <>
class MsgSender<CompanyZ>
{   // MsgSender ���ø��� ���� Ư��ȭ ����. SendClear �Լ��� ���� �͸� �����ϸ� �Ϲ��� ���ø��� ����.
public:
	void SendSecret(const MsgInfo& Info)
	{
		//...
	}
};

/*
	�� �ڵ�� MsgSender ���ø��� ���ø� �Ű������� CompanyZ�� �� �� �� �ֵ��� Ư��ȭ�� �����̴�.
	�׸��� �̷��� Ư��ȭ�� ���� ���ø� Ư��ȭ(total template Specializaion)�̶�� �Ѵ�.
	Ư��ȭ�� ���ø��� �Ű��������� �ϳ��� �������� ��ü���� Ÿ������ ������ ���¶�� ��.
	���� �ϴ� Ÿ�� �Ű������� CompanyZ�� ���ǵ� �̻� �� ���ø��� �Ű������δ� �ٸ� ���� �� �� ���� �ȴٴ� ��.

   ���� LoggingMsgSender�� �ٽ� ���ƿͺ���
*/

template <typename Company>
class LoggongMsgSender2 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		// �޼��� ���� �� ������ �α׿� ���
		SendClear(Info);        //���� Company == CompanyZ��� �� �Լ��� ���� �� ���� ����.
		// �޼��� ���� �� ������ �α׿� ���
	}
};
/*
	������ �̷� ���� ���� �� �ֱ� ������ ���� ���� �Լ� ȣ���� C++�� �޾����� �ʴ� ���̴�.
	�⺻ Ŭ���� ���ø��� ������ Ư��ȭ�� �� �ְ�, �̷� Ư��ȭ �������� �����ϴ� �������̽���
	������ �Ϲ��� ���ø��� �� �������� ���� ���ٴ� ���� C++�� �ν��Ѵٴ� ��!
	���� C++ �����Ϸ��� ���ø����� ������� �⺻ Ŭ������ ������ ��ӵ� �̸��� ã�� ���� �ź��Ѵ�.

	�̸� �ذ��ϱ� ���� �� ���� ����� �ִ�.
	1. �⺻ Ŭ���� �Լ��� ���� ȣ�⹮ �տ� this->�� ���δ�.
*/

template <typename Company>
class LoggongMsgSender3 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		// �޼��� ���� �� ������ �α׿� ���
		this->SendClear(Info);
		// �޼��� ���� �� ������ �α׿� ���
	}
};

//  2. using Ű���带 ����Ѵ�.

template <typename Company>
class LoggongMsgSender4 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		using MsgSender<Company>::SendClear;
		// �޼��� ���� �� ������ �α׿� ���
		SendClear(Info);
		// �޼��� ���� �� ������ �α׿� ���
	}
};// ���ø�ȭ�� �⺻ Ŭ������ ��ȿ������ ������� �츮�� �����Ϸ����� �˷����� ������
//   �����Ϸ��� �˾Ƽ� ã�� ���� ���ٴ� ��.

// 3. ȣ��� �Լ��� �⺻ Ŭ������ �Լ���� ���� ��������� �����ϴ� ��.

template <typename Company>
class LoggongMsgSender5 : public MsgSender<Company>
{
public:
	void SendClearmsg(const MsgInfo& Info)
	{
		// �޼��� ���� �� ������ �α׿� ���
		MsgSender<Company>::SendClear(Info);
		// �޼��� ���� �� ������ �α׿� ���
	}
};
/*
	�� ����� ��õ���� �ʴ´�. ȣ��Ǵ� �Լ��� ���� �Լ��� ��� �̷������� �����ع�����
	���� ���ε��� ���õǱ� �����̴�.(�������� ���ε��Ǿ����)

	�⺻������ �� �� ���� ����� ���ۿ����� ����. �⺻ Ŭ���� ���ø���
	���Ŀ� ��� Ư��ȭ�Ǵ��� ������ �Ϲ��� ���ø����� �����ϴ� �������̽��� �״�� ������ ���̶��
	�����Ϸ����� ����� �Ѵٴ� �κ���.

	������ �� ����� �߸��ƴٸ�?
*/

LoggingMsgSender<CompanyZ> zMsgSender;
MsgInfo MsgData;
//MsgData�� ������ ä��
zMsgSender.SendClearMsg(MsgData);   //�����ϵ��� ����

/*
	���� �̺κп����� �⺻ Ŭ������ MsgSender<CompanyZ>��� ����� �����Ϸ��� �˰� �ִµ��ٰ�
	SendClearMsg�Լ��� ȣ���Ϸ���SendClear�Լ��� MsgSender<CompanyZ>Ŭ������ �� ����ִٴ� ��ǵ� �����Ϸ��� �˾�æ ���̱� ����.

	�⺻ Ŭ������ ����� ���� ������ ��ȿ������ �����Ϸ��� �����ϴ� ������ �Ļ� Ŭ���� ���ø��� ���ǰ� �����м��� �Ǵ� ��Ȳ�� ������
	�ƴϸ� �Ļ� Ŭ���� ���ø��� Ư���� ���ø� �Ű������� �޾� ��üȭ�� �� �����İ� �� ������ �ٽ��̰�,
	C++���̸� ������ ��ȣ�ϴ� ��å���� �����ߴٴ� ���̴�.
	�Ļ� Ŭ������ ���ø����κ��� ��üȭ�� �� �����Ϸ��� �⺻ Ŭ������ ���뿡 ���� �ƹ��͵� �𸣴� ������ �����ϴ� ������ �װ��̴�
*/