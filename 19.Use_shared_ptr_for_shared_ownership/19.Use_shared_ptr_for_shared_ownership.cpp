// ������ ���� �ڿ��� �������� std::shared_ptr�� ����϶�
// ��ü�� ����Ű�� ������ std::shared_ptr�� ��ü�� �� �̻� ����Ű�� �ʰ� �Ǹ�,
// �� std::shared_ptr�� �ڽ��� ����Ű�� ��ü�� �ı��Ѵ�.
// ��ü�� ���� ���� �Ű� �� �ʿ䰡 ����, ��ü�� �ı� ������ ���������̴�.

// ���� Ƚ���� �����Ǵ� �ڿ��� ������ ��, �� �ڿ��� ����Ű�� std::shared_ptr���� ����
// std::shared_ptr�� �����ڴ� �� ����Ƚ���� ����, �Ҹ��ڴ� ����, ���� ���� �����ڴ� ������ ���Ҹ� ��� ����.
// � std::shared_ptr�� �ڿ��� ����Ƚ���� ������ �� �� Ƚ���� 0�� �Ǿ��� ��,
// �� �ڿ��� ����Ű�� std::shared_ptr�� �� �̻� �ʿ���ٴ� ��. std::shared_ptr�� �ڿ��� �ı��Ѵ�.

// 1. std::shared_ptr�� ũ��� �� �������� �� ���̴�.
// 2. ����Ƚ���� ���� �޸𸮸� �ݵ�� �������� �Ҵ��ؾ� �Ѵ�.
// std::make_shared�� �̿��ؼ� std::shared_ptr�� �����ϸ� ���� �Ҵ��� ����� ���� �� �ִ�. (�׸�21)
// 3. ����Ƚ���� ������ ���Ұ� �ݵ�� ������ �����̾�� �Ѵ�.

// std::shared_ptr�� �����ڰ� ����Ī ��ü�� ����Ƚ���� �������� �ʴ� ���
// �̵� ������ ���.

// std::unique_ptró�� std::shared_ptr�� delete�� �⺻���� �ڿ� �ı� ��Ŀ�������� ���.
// Ŀ���� �����ڸ� �����ϴµ�, ��ü���� ����� std::unique_ptr�� �ٸ���.
#include <memory>
#include <vector>
class Widget : public std::enable_shared_from_this<Widget>
{
public:
	template<typename... Ts>
	static std::shared_ptr<Widget> crate(Ts&&... params);

	void process()
	{
		processedWidgets.emplace_back(this);	// �߸��� ���
		processedWidgets.emplace_back(shared_from_this());
	}

	std::vector<std::shared_ptr<Widget>> processedWidgets;

private:
	// �����ڵ�
};
auto loggingDel = [](Widget* pw) {
	delete pw;
};
std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel);
std::shared_ptr<Widget> spw(new Widget, loggingDel);

// std::shared_prt�� ���谡 �� �����ϴ�.
// ����ϴ� Ŀ���� �������� ������ ���� �ٸ� �� std::shared_ptr<<Widget>�� ���
auto customDeleter1 = [](Widget* pw) {};
auto customDeleter2 = [](Widget* pw) {};
std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
std::shared_ptr<Widget> pw2(new Widget, customDeleter2);
std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };
// �ϳ��� �ٸ� �ϳ��� ������ ���� �ְ�, �� �� std::shared<Widget>������ �Ű������� �޴� �Լ��� �Ѱ��� �� �ִ�.
// std::unique_ptr������ �Ұ���. ���Ŀ� Ŀ���� �������� ������ ������ ��ġ�� ������

// std::unique_ptr�� �� �ٸ� ������ : Ŀ���� �����ڸ� �����ص� std::shared_ptr ��ü�� ũ�Ⱑ ������ �ʴ´�.
// std::shared_ptr<T>��ü�� �޸� : T ��Ī ������ -> T ��ü, ��������Ī ������ -> ����Ƚ��, ���� Ƚ��, ��Ÿ �ڷ�(Ŀ���� ������, �Ҵ��� ��)

// ��ü�� �������� �� ��ü�� ����Ű�� ������ std::shared_ptr�� �����ɶ� �����ȴ�.
// �����ϴ� �ڵ忡�� �� ��ü�� ����Ű�� �ٸ� std::shared_ptr�� �̹� �����ϴ����� �˾Ƴ��� ���� �Ұ���,
// �������� �������ο� ���� ������ ���� ��Ģ ���� ����
// 1. std::make_shared(�׸�21)�� �׻� �������� �����Ѵ�.
// 2. ���� ������ ������(�� std::unique_ptr�� std::auto_ptr)�κ��� std::shared_ptr��ü�� �����ϸ� �������� �����ȴ�.
// 3. �� �����ͷ� std::shared_ptr �����ڸ� ȣ���ϸ� �������� �����ȴ�.

// �ϳ��� �� �����ͷ� �������� std::shared_ptr�� �����ϸ� ������ ��ü�� ���� ���� ���� ����� ��������Ƿ�,
// �ǹ��� ���� ���� �������� �ൿ�� �ȴ�.
auto pw = new Widget;
std::shared_ptr<Widget> spw1(pw, loggingDel);
std::shared_ptr<Widget> spw2(pw, loggingDel);

// std::shared_ptr �����ڿ� �� �����͸� �Ѱ��ִ� ���� ���϶�.
// ���: std::make_shared(�׸�21)�� ����϶�, ��, �� �� Ŀ���� �����ڸ� ���� ����.
// std::shared_ptr �����ڸ� �� �����ͷ� ȣ���� �� �ۿ� ���� ��Ȳ������ 
// �� ������ ������ ��ġ�� ����, new�� ����� ���� �����϶�.
std::shared_ptr<Widget> fix_spw1(new Widget, loggingDel);
std::shared_ptr<Widget> fix_spw2(fix_spw1);

// this�����ͷ� ���� �߸��� ��
// ���� widgetŬ���� ���� process�Լ�
// std::shared_ptr���� �����̳ʿ� this�� ������, std::shared_ptr��ü�� �����Ǹ鼭,
// ����Ī Widget��ü(*this)�� ���� �� �������� ���������.
// ����Լ� �ٱ��� �̹� �� Widget�� ����Ű�� �ٸ� std::shared_ptr���� �ִٸ�, �ʿ� ������ �ൿ�� �߻�
// std::shared_ptr�� �����ϴ� Ŭ������ �ۼ��� ��, �� Ŭ������ this�����ͷκ��� std::shared_ptr�� �����ϰ� �����Ϸ���
// std::enable_shared_from_this�� �� Ŭ������ ���Ŭ������ ������ �ȴ�. �׸��� shared_from_this() ���

// shared::from_this�� ���� ��ü�� ���� �������� ��ȸ�ϰ�, �� �������� ��Ī�ϴ� �� std::shared_ptr�� ����
// ���� ��ü�� �̹� �������� �����Ǿ� �ִٰ� ����. ���� ���ٸ�, �Լ��� �ൿ�� ���ǵ��� �ʴ´�.
// std::shared_ptr�� ��ȿ�� ��ü�� ����Ű�⵵ ���� shared_from_this�� ȣ���ϴ� ���� �����ϱ� ����,
// std::enble_shared_from_this�� ��ӹ��� Ŭ������ �ڽ��� �����ڵ��� private�� ����.
// �ܺο��� ��ü�� ������ �� �ֵ��� std::shared_ptr�� �����ִ� ���͸� �Լ��� ����

// std::unique_ptr���� std::shared_ptr�� ���׷��̵�� ����.
// std::shared_ptr���� std::unique_ptr�� ���ư� ���� ����.

// std::shared_ptr�� �� �� ���� �Ϸδ� �迭������ �ִ�.
// std::shared_ptr�� API�� ���� ��ü�� ����Ű�� �����͸� ���ο� �ΰ� ����.
// std::shared_ptr�� operator[]�� �������� �����Ƿ� �������� �迭���ҿ� �����Ϸ��� ������ ����� ������ ����� ǥ������ ����
// std::shared_ptr�� �����ϴ� �Ļ�Ŭ���� �����Ϳ��� ��� Ŭ���������ͷ��� ��ȯ�� ���ϰ�ü���� �մ�, 
// �迭�� �����ϸ�, ���Ľý��ۿ� ������ �����.
// C++11�� ���� �迭�� ���� �پ��� ��ȵ� std::array, std::vector, std::string�� ����

int main(){}

//1. std::shared_ptr�� ������ �����ڿ��� ������ ���ϰ� (������ ���ſ� �ñ涧 ��ŭ�̳�) ������ �� �ִ� ������ ����
//2. ��ü�� std::shared_ptr ��ü�� �� ũ�Ⱑ std::unique_ptr��ü�� �� ���̸�, ���� ��Ͽ� ���õ� �߰� �δ��� ����,
// ������ ����Ƚ�� ������ �䱸�Ѵ�.
//3. �ڿ��� �⺻������ delete�� ���� �ı��ǳ�, Ŀ���� �����ڵ� �����Ѵ�.
// �������� ������ std::shared_ptr�� ���Ŀ� �ƹ��� ���⵵ ��ġ�� �ʴ´�.
//4. �� ������ ������ �����κ��� std::shared_ptr�� �����ϴ� ���� ���ؾ� �Ѵ�.