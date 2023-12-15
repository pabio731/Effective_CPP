#include "pch.h"

/*
	���� ����ϸ� ���� ���� ���ø� �� �ִ� ������ �̸��� �ߺ��̴�.
	�� �̻��� �⺻ Ŭ�����κ��� ���� �̸��� �������� ���ɼ��� ����ٴ� ��.
	�Ʒ� ���ø� ���� MP3Player Ŭ������ �� ���� Ŭ������ ����Ѵ�.
*/
class CBorrowableItem
{
public:
	void CheckOut();		//���̺귯���κ��� üũ �ƿ��մϴ�
};

class CElectronicGadget
{
private:
	bool CheckOut() const; //��ü �׽�Ʈ�� �ǽ��ϰ�, �������� ��ȯ
};

class CMP3Player :
	public CBorrowableItem,
	public CElectronicGadget
{};

int main()
{
	CMP3Player mp;
	mp.CheckOut();
}

/*
	CheckOut�� ȣ���ϴ� �κп��� ��ȣ���� �߻��Ѵ�.
	������ mp��ü�� ȣ���� �� �ִ� CheckOut�� BorrowableItem::CheckOut����
	�и��ѵ��� ��ȣ���� �߻��Ѵ�(ElectronicGadget::CheckOut�� private�̹Ƿ�)

	�̰��� � �Լ��� ���� ������ �Լ����� Ȯ���غ������� �����Ϸ��� C++�� �ߺ���
	�Լ� ȣ�� �� �ϳ��� ��󳻴� ��Ģ�� ���� ���̴�.
	�־��� ȣ�⿡ ���� �������� ��ġ�ϴ� �Լ����� ���� Ȯ�� ��,
	�Լ��� ���� ���ɼ��� �Ǵ���.
	��  ElectronicGadget::CheckOut �Լ��� ���� ���ɼ��� �����ϴ� �������� ���� ���� ��.

	�̷��� ��ȣ���� �ؼ��ϱ� ���ؼ��� Scope�����ڷ� � Ŭ������ �Լ����� ���� ����� �Ѵ�.
	mp.BorrowableItem::CheckOut(); ó��!

	�� ������ ���� �ܰ��� �⺻ Ŭ������ ���� �� ���� Ŭ���� ���뿡�� �ɽ�ġ �ʰ� ���� �ڴ�.
	Ư�� ���̾Ƹ�� ����� ������ ���� �� �ִ�.
*/
class CFile {};
class CInputFile : public CFile {};
class COutputFile : public CFile {};
class CIOFile : public CInputFile, public COutputFile {};

/*
	�̷� ����� �Ǹ� ������ �⺻ Ŭ������ ������ ����� ��� ������ŭ �ߺ� �����Ǵ� ������
	�����.
	���� ������ ����� �ߺ������ϴ� ���� ���� ���� �ƴϿ��ٸ� �ߺ��� �� �ִ� ������ �����
	���� Ŭ�������� ���� �⺻ Ŭ������ ���� �� ������ �ذ��� �� �ִ�.
	�� ���� ����� ����ϴ� ��.
*/

class CFile2 {};
class CInputFile2 : virtual public CFile2 {};
class COutputFile2 : virtual public CFile2 {};
class CIOFile2 : public CInputFile2, public COutputFile2 {};

/*
	������ c++�� ǥ�� ���̺귯���� basic_ios, basic_istream, basic_ostream, basic_iostream��
	�̷� ������ �Ǿ� �ִ�.

	��Ȯ�� ������ ���������� public����� �ݵ�� �׻� ���� ���̾�� �ϴ� ���� �´�.
	������ ��� ��ӵǴ� ������ ����� �ߺ������� ���� ������ �츮���� ������ �ʴ� �����Ϸ���
	����� ����ִ�. �� ������ ���� ����� ����ϴ� Ŭ������ ������� ��ü�� �׷��� ����
	��ü���� ũ�Ⱑ �� ũ��. ���� ���� �⺻ Ŭ������ ������ ����� �����ϴ� �ӵ��� �񰡻�
	�⺻ Ŭ������ ������ ����� �����ϴ� �ӵ����� ������.
	�� ���� ����� ��δ�.

	������ �ٸ� ������ �� ����� ������. ���� �⺻ Ŭ������ �ʱ�ȭ�� ���õ� ��Ģ��
	�񰡻� �⺻ Ŭ������ �ʱ�ȭ ��Ģ���� �ξ� �����ѵ��ٰ� �������� �� ��������.
	1. �ʱ�ȭ�� �ʿ��� ���� �⺻ Ŭ�����κ��� Ŭ������ �Ļ��� ���, �� �Ļ� Ŭ������
	   ����⺻ Ŭ�������� �Ÿ��� ������� ���� �⺻ Ŭ������ ���縦 ���ο� �ΰ� �־�� �Ѵ�.
	2. ������ Ŭ���� ���뿡 �Ļ� Ŭ������ ���� �߰��� ���� �� �Ļ� Ŭ������ ���� �⺻ Ŭ������
	   �ʱ�ȭ�� ���þƾ� �Ѵ�.

	���� ���� ��ӿ� ���ؼ��� ���� �� ������ ������
	1. ���¿� �� �ʿ䰡 ������ ���� �⺻ Ŭ������ ������� �� ��.
	2. ���� �⺻ Ŭ������ ���� ���� ������ �ȵ� ��Ȳ�̶��, ���� �⺻ Ŭ�������� �����͸� ���� ������ ����Ѵ�.

	����, C++�������̽� Ŭ������ �Ἥ ����� ����ȭ�� �ڵ带 ����
*/
class IPerson
{
public:
	virtual ~IPerson();
	virtual std::string strName() const = 0;
	virtual std::string strBirthDate() const = 0;
};
/*
	IPerson�� ������ �и��� IPerson �����ͳ� �����ڸ� �뷡�� ���α׷��� �ؾ� �� ���̴�.
	�߻� Ŭ�����̴ϱ�. ������ ������ IPerson ��ü(��Ȯ���� IPerson�� ���ۿ����� ���� ��ü)��
	�����ϱ� ���ؼ� IPerson�� ����ڴ� ���丮 �Լ��� ����ؼ� IPerson�� ��ü �Ļ� Ŭ������
	�ν��Ͻ��� �����.
*/
typedef size_t DatabaseID;
DatabaseID ID(0);
std::tr1::shared_ptr<IPerson> MakePerson(DatabaseID piecewise_construct);
DatabaseID AskUserForDatabaseID();		// ����ڷκ��� �����ͺ��̽�ID�� ���� �Լ�
DatabaseID Id(AskUserForDatabaseID());

// CPersom�������̽��� �����ϴ� ��ü�� �ϳ� ����� pp�� ����Ű�� �Ѵ�.
// ���� *pp�� ������ ���� IPerson�� ����Լ��� ����Ѵ�.
std::tr1::shared_ptr<IPerson> pp(MakePerson(ID));

/*
	MakePerson�Լ��� ��� �ؼ� �� �Լ��� ��ȯ�� �����ͷ� ����ų ��ü�� ���� ���� ��
	�ִ� ���ϱ�? �Ƹ� MakePerson�Լ��� �ν��Ͻ��� ���� �� �ִ�
	��ü Ŭ������ IPerson�κ��� �Ļ��Ǿ� �־�� �� ���̴�.
	�� Ŭ������ �̸��� CPerson�̶� ��������. CPerson�� IPerson���κ��� ��������
	���� ���� �Լ��� ���� ������ ������ ���̴�.
	�׷��� ������ ������ �����ͺ��̽� ���� Ŭ������ PersonInfo�� ����
	���� CPerson�� �ʿ��� �ٽ� ����� �� ���� �־��ٰ� ��������
*/

class PersonInfo
{
public:
	explicit PersonInfo(DatabaseID pID);
	virtual ~PersonInfo();

	virtual const char* TheName() const;
	virtual const char* TheBirthData() const;

private:
	virtual const char* ValueDelimOpen() const;
	virtual const char* ValueDelimClose() const;
};
/*
	�׷��ٸ� CPerson�� PersonInfo ���̸� �մ� ����� ���� ����. PersonInfo Ŭ������ CPerson��
	�����ϱ� ���ϰ� ������ִ� �Լ��� ��¼�� ���� �ִ� ���̴�
	�׸��� �츮�� �̷� ���踦 is-implemented-in-terms-of�� ����� �ϱ�� �ߴ�.
	�׸��� �̷� ���踦 �����ϴ� ������� ��ü �ռ��� private��� �� ������ �ִ�.
	��κ��� ��ü �ռ��� ��ȣ������, ���� �Լ��� ������ �ؾ��Ѵٸ� private ����� �ؾ��Ѵ�
	ValueDelimOpen �Լ��� ValueDelimClose�Լ��� �������� �ʿ䰡 �ִ� ��Ȳ�̶� �����Ѵٸ�
	private ����� �ϰ� �� ���̴�. ���� ��ü �Լ��� ����� �����ϴ� ����� ������ private�����
	�̿��Ѵٰ� �����غ��� �׷��ٸ� �ٷ� ������ ���� ����� �ǹ��ְ� �� �� �ִ� ��Ȳ�̵� ���̴�.
	�������̽��� public ��Ӱ� private����� �����ϴ� ���̴�.
*/

class CPerson : public IPerson, private PersonInfo
{
public:
	explicit CPerson(DatabaseID pid)
		: PersonInfo(pid) {}
	virtual std::string strName() const { return PersonInfo::TheName(); }
	virtual std::string strBirthDate() const { return PersonInfo::TheBirthData(); }
private:
	const char* ValueDelimOpen() const { return ""; }
	const char* ValueDelimClose() const { return ""; }
}

/*
	���� ��ӵ� ��ü ���� ������� ����Ʈ��� �����ϴ� �� ���̴� ���� �� �ϳ��̴�.
	���� ���� ��Ӻ��ٴ� ����ϱ⵵, �����ϱ⵵ ��Ʊ� ������ �����ϴٸ� ���� ������� �ذ��ϴ� ���� ����.
	�����ϸ� ���� ������� �ִ��� �ذ��Ϸ� �ø��� �ϰ�, ���� �ʿ��ϴٸ� ���� ����� ������ ����.
*/