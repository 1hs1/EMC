#include <memory>

//������ ���� �ڿ��� �������� std::unique_ptr�� ����϶�

//std::unique_ptr�� �� ������ ������ ��ɵ��� �����Ѵ�.
//�޸𸮿� cpu�ֱⰡ �������� �ʾƵ� ����� �� �ִ�.
//std::unique_ptr�� ������ ������(exclusive ownership)�ǹ̷��� �����ϰ� �ִ�.
//NULL�� �ƴ� std::unique_ptr�� �׻� �ڽ��� ����Ű�� ��ü�� �����Ѵ�.
//std::unique_ptr�� �̵� ���� �������� std::unique_ptr �Ҹ� �� �ڽ��� ����Ű�� �ڿ��� �ı��Ѵ�.

//��ü�� �����ϴ� ���丮 ��ȯ �Լ�����, ���丮 �Լ��� ���� ��ü�� �����ϰ� �� ��ü�� ����Ű�� �����͸� �����ش�.
//���� �Ҹ� �� ����Ī ��ü�� �ڵ����� ������ �ش�.
class Investment { };
class Stock :
	public Investment {
};
class Bond :
	public Investment {
};
class RealEstate :
	public Investment {
};
template<typename... Ts>
std::unique_ptr<Investment>
makeInvestment(Ts&&... params);
/*
{
	{
		auto pInvestment = makeInvestment(�μ�);
	} // *pInvestment �� �ı��ȴ�
}
*/

//std::unique_ptr�� �ı��� �⺻������ delete�� ���� �Ͼ����, 
//std::unique_ptr��ü�� ������ �� Ŀ���� �����ڸ� ����ϵ��� �����ϴ� �͵� �����ϴ�.

void makeLogEntry(Investment* pInvestment) {};
//makeInvestment ���� ��ü�� delete�� �������� �ʰ� �α� �׸� ��� �Ŀ� ����
auto delInvmt = [](Investment* pInvestment) // custom
{ 
	makeLogEntry(pInvestment);
	delete pInvestment;
};
template<typename... Ts> // revised
std::unique_ptr<Investment, decltype(delInvmt)> // return type
makeInvestment(Ts&&... params)
{
	std::unique_ptr<Investment, decltype(delInvmt)> // ptr to be
		pInv(nullptr, delInvmt);
	if ( /*Stock object �������*/)
	{
		pInv.reset(new Stock(std::forward<Ts>(params)...));
	}
	else if ( /*Bond object �������*/)
	{
		pInv.reset(new Bond(std::forward<Ts>(params)...));
	}
	else if ( /*RealEstate object �������*/)
	{
		pInv.reset(new RealEstate(std::forward<Ts>(params)...));
	}
	return pInv;
}
//-delInvmt�� Ŀ���� �������̴�.makeLogEntry�� ȣ�� �� delete�� �����Ѵ�.
//-Ŀ���� ������ ���� delInvmt�� std::unique_ptr�� ��° ���� �μ��� �����ؾ� �Ѵ�.
// makeInvestment�� ��ȯ ������ std::unique_ptr<Investment, decltype(delInvmt)>�̴�
//-makeInvestment�� �� std::unique_ptr�� ���� ������ ������ ��ü�� ����Ű�� �� �� �����ش�.
//-�� �����͸� std::unique_ptr�� �����ϴ� ������ �����ϵ��� �ʴ´�.������ new�� ������ ��ü�� �������� �ο��ϱ� ���� reset�� ȣ���ߴ�.
//-�Լ��� ���޵� �μ����� new�� �Ϻ��ϰ� �����ϱ� ���� std::forward�� ����ߴ�.
//-Ŀ���� �����ڴ� Investment* ������ �Ű������� �޴´�.�� ��ü�� ���ٽ� �ȿ��� Investment* ��ü�μ� delete�ȴ�.
// �̸� ���ؼ� Investment�� �Ҹ��ڰ� virtual �̾�� �Ѵ�.
/*
class Investment {
public:
	virtual ~Investment();
};
*/
// C++14
template<typename... Ts>
auto makeInvestment(Ts&&... params) // C++14
{
	auto delInvmt = [](Investment1* pInvestment)
	{ // inside
		makeLogEntry(pInvestment); // makedelete
		pInvestment; // Investment
	};
}


//std::unique_ptr�� ��ü�� ũ��� ���������� ũ��� ����ϳ�, Ŀ���� �������� ��� �޶�����.
//�����ڰ� �Լ� ��ü�� ������ std::unique_ptr�� ũ�Ⱑ �� �Լ� ��ü�� ����� ������ ũ�⸸ŭ �����Ѵ�.
//���� ���� �Լ� ��ü�� ��� ũ�� ��ȭ�� ������, ���� ���� �Լ��ε� ������ �� ������, ���� ǥ������ �� ��ȣ�Ѵ�.
// ��ȯ ������ ũ��� Investment*
auto delInvmt1 = [](Investment* pInvestment)
{
	makeLogEntry(pInvestment);
	delete pInvestment;
};
template<typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt1)>
makeInvestment(Ts&&... args);
void delInvmt2(Investment* pInvestment)
{
	makeLogEntry(pInvestment);
	delete pInvestment;
}
// ��ȯ ������ ũ�� : Investment* + �Լ� �������� ũ��
template<typename... Ts>
std::unique_ptr<Investment,
void(*)(Investment*)> 
makeInvestment(Ts&&... params);


//���°� ���� �Լ� ��ü �����ڸ� ����Ѵٸ� std::unique_ptr�� ũ�Ⱑ ��� ������ �Ѿ �� ������, 
//�� ��� ���踦 �����ؾ� �Ҽ� �� �ִ�.

//std::unique_ptr�� ��� ��� ���� ���̰� �ִ�.��ü�� ��� ���� ���� ����(operator[])�� �������� ������, 
//�迭�� ��� ������ �����ڵ�(operator*�� operator->)�� �������� �ʴ´�.

//std::unique_ptr�� std::shared_ptr�ε� ��ȯ�� �����ϴ�.������ std::unique_ptr�� ���丮 �Լ��� ��ȯ�������� �����ϴ�.
//std::shared_ptr<Investment> sp = makeInvestment(args);


//- std::unique_ptr�� ���� ������ �ǹ̷��� ���� �ڿ��� ������ ����, �۰� ���� �̵� ���� �ȶ��� �������̴�.
//- �⺻������ �ڿ��ı��� delete�� ���� �Ͼ��, Ŀ���� �����ڸ� ������ ���� �ִ�.
//  ���°� �ִ� �����ڳ� �Լ� �����͸� ����ϸ� std::unique_ptr ��ü�� ũ�Ⱑ Ŀ����.
//- std::unique_ptr�� std::shared_ptr�� �ս��� ��ȯ�� �� �ִ�.

int main()
{
	return 0;
}