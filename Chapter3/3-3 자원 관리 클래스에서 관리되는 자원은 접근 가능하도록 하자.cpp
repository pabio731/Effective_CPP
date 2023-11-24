#include<iostream>

/*
	�ڿ� ���� Ŭ������ �Ǽ��� �ڿ� ���� �� �ִ� ��ȣ�� ������ ���ش�. ���� ���� ���� ���� �ڿ��� ������ ������ ����.
	������ ���� ������ �ڵ���� �̹� �ڿ��� ���� �����ϵ��� ������� ���� �� �ֱ� ������ ���� �ڿ��� ������ �� ���� ����� ���� �� �ִ�.

	���ڸ� �����ϴ� Investment Ŭ������ü��
	RAII�� �̿��ϱ� ���� ����Ʈ �����͸� �̿���
	std::tr1::shared_ptr<Investment> pInv(CreateInvestment());
	�޾Ҵ�.

	���� �� ��ü�� ����ϴ� �Լ���
	int DaysHeld(const Investment* pi);		//���ڱ��� ���Ե� ���ķ� ����� ������ ��ȯ
	�� �ִٰ� �ϰ�, �̸� ������ ���� ���� �� ���ε�

	int iDays = DaysHeld(pInv);
	�� �ڵ�� �������� �ȵȴ�. pInv�� Ÿ���� tr1::shared_ptr<Investment>���� Investment*�� �ƴϱ� �����̴�.

	���� RAIIŬ������ ��ü�� �� ��ü�� ���ΰ� �ִ� ���� �ڿ�(Ivestment*)���� ��ȯ�� ����� �ʿ��ϴ�.
	�� ��� �� ������ �Ϲ����� ����� �ִµ�, �ٷ� ����� ��ȯ(explicit conversion)�� �Ͻ��� ��ȯ(implicit conversion)�̴�.

	tr1::shared_ptr�� auto_ptr �� ����� ��ȯ�� �����ϴ� get�̶�� ��� �Լ��� �����Ѵ�. �� �Լ��� ����ϸ� �� Ÿ������ ����
	����Ʈ ������ ��ü�� ����ִ� ���� ������(�� �纻)�� �� �� �ִ�.

	int iDays = dDaysHeld(Pinv.get())	// pInv�� ������ �ִ� ���� �������� �纻�� �ѱ�.

	�� ����Ʈ �����ʹ� ������ ������ ������(operator-> �� operator*)�� �����ε� �ϰ� �ִ�. �̿� ���� �Ͻ��� ��ȯ�� ���� �� �� �ִµ�
	������ ����.
*/

class Investment
{
public:
	bool IsTaxFree() const;
	//...
};

Investment* CreateInvestment();		//���丮 �Լ�

std::tr1::shared_ptr<Investment> pi1(CreateInvestment());	//����Ʈ�����Ͱ� �ڿ� ������ �õ��� ��
bool Taxable1 = !(pi1->IsTaxFree());			//operator->�� �Ἥ �ڿ��� ����.
//...

std::auto_ptr<Investment> pi2(CreateInvestment());
bool Taxable2 = !((*pi2).IsTaxFree());			//operator*�� �Ἥ �ڿ��� ����

/*
	RAII ��ü �ȿ� ��� �ִ� ���� �ڿ��� �� �ʿ䰡 ���� ����� ������ RAII Ŭ���� ���踦 �ϴ� ��� �߿��� �Ͻ��� ��ȯ �Լ��� �����ϴ� ����� �ִ�.
	������ �Ϻ� ���� C API�� ���� ������ ������ ��Ʈ�� RAIIŬ������ �ѷ��μ� ���� ����̴�.
*/
FontHandle Getfont();		//C API���� ������ �Լ�
void ReleaseFont(FontHandle fh);	//C API���� ������ �Լ�

class Font		//RAII Ŭ����
{
public:
	explicit Font(FontHandle fh) : f(fh) {}			//�ڿ��� ȹ��, ���⼭ ���� ���� ������ ����ȴٴ� ���� ����. �ڿ������� C API�� �ϱ� ������.
	~Font() { RereaseFont(f); }
private:
	FontHandle f;		//���� ��Ʈ �ڿ�
};

/*
	�Ϻ� ���� C API�� FontHandle�� ����ϵ��� ������� ������ �Ը� ��ô ũ�ٰ� �����ϸ�, Font��ü�� FontHandler�� ��ȯ�ؾ� �ϴ�
	��쵵 ���� ���� ���̶�� ������ �غ� �� �ִ�.
	FontHandle get() const { return f;  }		//����� ��ȯ �Լ�.
	�� �Լ��� public ���� ������ �����ϸ� �� �� �ֱ�� ������ �Ϻ� ���� API�� ���� ���� ������ get�� ȣ���ؾ� �Ѵ�.
*/

void ChangeFontSize(FontHandle f, int iNewSize);
Font f(getFont());
int iNewFontSize;
//...
ChangeFontSize(f.get(), iNewFontSize);		//FOnt���� FontHandle�� ��������� �ٲ� �Ŀ� �ѱ�.

/*
	���� ���� ����ؼ� �Լ��� ȣ�����־�� �Ѵٴ� ���� ������ ���� �ʴ´�.
	�̿� ���� ������� �Ͻ��� ��ȭ �Լ��� �����ϴ� ���� ����
*/

class Font		//RAII Ŭ����
{
public:
	explicit Font(FontHandle fh) : f(fh) {}
	~Font() { RereaseFont(f); }
	oprator FontHandle() const { return f; }
private:
	FontHandle f;		//���� ��Ʈ �ڿ�
};

/*
	�Ͻ��� ��ȯ �Լ� ���� C API�� ����ϱⰡ �ξ� �������� �ڿ��������� �� �ִ�.
	���� �ڵ带 ����
*/

Font f(getfont());
int iNewFontSize;
//...
ChangeFontsize(f, iNewFontSize);	//Font���� FontHandle�� �Ͻ��� ��ȯ�� ����

//�ٸ� �� ��� �Ǽ��� ������ ������ �������Ƿ� �����ؾ� �Ѵ�.

Font f1(getFont());
//...
FontHandle f2 = f1
//���� �ǵ��� font��ü�� �����ϴ� ���̾��µ�,  f1�� FontHandle�� �ٲٰ� ����Ǿ����.

/*
	���� �̷� ��Ȳ�� ������ Font��ü�� f1�� �����ϰ� �ִ� FontHandle�� f2�� ���ؼ��� ���� ����� �� �ִ� ���°� �Ǿ������..
	������ f1�� �Ҹ��ϸ� f2�� �Ҹ��� ��ü�� ����Ű�Ե�.

	����� ��ȯ�� ������� �Ͻ��� ��ȯ�� ��������� ������ RAII Ŭ�������� �뵵�� ȯ�濡 ����....
	�Ϲ������δ� �Ͻ��� ��ȯ���ٴ� ����� ��ȯ�� ������ �ʴ� ��ȯ�� ������ ���ɼ��� ũ�� ������ ����.

	�߰������� RAIIŬ������ ���� ������ ĸ��ȭ�� �ƴϱ� ������ Ư���� ũ�� ����ȴٰ� ���� ��ƴ�.
	������ ������ RAIIŬ�����鵵 ������ ĸ��ȭ, ������ ĸ��ȭ�� ���ÿ� �����ϴ� �͵��� �� ����
*/