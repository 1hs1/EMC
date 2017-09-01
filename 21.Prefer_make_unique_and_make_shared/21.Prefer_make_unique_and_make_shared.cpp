// new�� ���� ����ϴ� �ͺ��� std::make_unique�� make_shared�� ��ȣ�϶�.
// make_shared : C++11
// make_unique : C++14

#include <memory>
#include <vector>

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

// std::make_unique�� std::make_shared�� ������ ������ ������ �μ����� �޾Ƽ� �װ͵��� �����ڷ�
// �Ϻ� �����ؼ� ��ü�� �������� �����ϰ�, �� ��ü�� ����Ű�� �ȶ��� �����͸� �����ִ� ������
// make�Լ� �� ���̴�.
// ������ �ϳ��� std::allocate_shared�̴�. ù �μ��� ���� �޸� �Ҵ翡 ���� �Ҵ��� ��ü�� ���� std::make_shared�� �ٸ���.
class Widget{};
auto upw1(std::make_unique < Widget>());
std::unique_ptr<Widget> upw2(new Widget);
auto spw1(std::make_shared<Widget>());
std::shared_ptr<Widget> spw2(new Widget);

void processWidget(std::shared_ptr<Widget> spw, int priority){}
int computePriority() { return 0; }
// ��ȣ ����
// 1. �ڵ� �ߺ��� ����, Ÿ�ڷ��� �ش�.
// 2. ���� �������� ����
// 3. ȿ����

void cusDel(Widget *ptr){}
int main() 
{
	processWidget(std::make_shared<Widget>(), computePriority());
	processWidget(std::shared_ptr<Widget>(new Widget), computePriority());
	// 1. ǥ���� "new Widget"�� �򰡵ȴ�. ��, Widget�� ���� �����ȴ�.(�μ����� ���� ��)
	// 2. new�� ������ �����͸� �����ϴ� std::shared_ptr<Widget>�� �����ڰ� ����
	// 3. computePriority�� ����(2���� ������ �ٲ���� �ִ�.)
	// ���� computePriority�� ���� ����ǰ�, ���ܰ� �߻��Ǿ��ٸ�, 1������ ������ ��ü�� ����.

	// ����å(�Ʒ� ���� ���� �ð�)
	std::shared_ptr<Widget> spw(new Widget, cusDel);
	processWidget(spw, computePriority());	// ��������� �Ͼ. ����Ƚ���� ���������� ����
	processWidget(std::move(spw), computePriority());	// �̵������� �Ͼ. ����Ƚ�� ���� �ʿ����.
}

// new�� ���� ����ؼ� std::shared_ptr�� �����ϸ� Widget ��ü�� ���� �޸� �Ҵ�� �������� ����
// �� �ٸ� �޸� �Ҵ��� �Ͼ��.
// std::make_shared�� ����ϸ� �޸� ������ �ѹ��� �Ҵ�

// ����(make_unique, make_shared)
// 1. make �Լ��� �߿��� Ŀ���� �����ڸ� ������ �� �ִ� ���� ����.
// 2. make �Լ����� ���������� �Ű��������� �Ϻ� ������ �� �߰�ȣ�� �ƴ� ��ȣ�� �����. �߰�ȣ �ʱ�ġ�� �����Ϸ��� �ݵ�� new�� ���� ����ؾ���.
auto upv = std::make_unique<std::vector<int>>(10, 20);
auto spv = std::make_shared<std::vector<int>>(10, 20);
// 20�� ��� ����¥�� std::vector�� ����
// �׸� 30 �� ��ȸå�� ����.
auto initList = { 10, 20 };
auto spve = std::make_shared<std::vector<int>>(initList);

// make_shared��
// 3. Ŭ���� ���� operator new�� operate delete�� �ִ� ������ ��ü�� make�Լ��� �����ϴ� ���� ��ü�� �ٶ������� �ʴ�.
// ���� ��� WidgetŬ������ ���� operator new�� operator delete��� ũ�Ⱑ ��Ȯ�� sizeof(Widget)�� �޸� ��������
// �Ҵ�� ������ ó���ϴ� �� Ưȭ�� ��찡 ����. �׷��� std::allocate_shared�� �䱸�ϴ� �޸� ������ ũ���
// �������� �Ҵ�Ǵ� ��ü�� ũ�Ⱑ �ƴ϶� �� ũ�⿡ �������� ũ�⸦ ���� ��.
// 4. new�� ���� ��뿡 ���� std::make_shared�� ũ�� �� �ӵ����� ������ std::shared_ptr�� ���� ����� ���� ��� ��ü�� ������
// �޸� ������ ���δٴ� ������ ��Եȴ�.
// �� ��ü�� ���� Ƚ���� 0�� �Ǹ� ��ü�� �ı��ȴ�. �׷��� ��ü�� �����ϰ� �ִ� �޸𸮴� �ش� �������� �ı��Ǳ� ��������
// ������ �� ����. ���� �޸� ������ ����ֱ� ������...
// �������� �����ϴ� std::weak_ptr���� �����ϴ� �� �������� �Լ��ؼ� �����ؾ� �Ѵ�.
// ���� std::shared_ptr�� make�Լ��� �Ҵ��� �޸� ������ �װ��� �����ϴ� ������ std::shared_ptr�� ������ std::weak_ptr
// �Ѵ� �ı��� �Ŀ��� ����.
// ��ü ������ ����� ũ�� ������ std::shared_ptr�� �ı��� ������ std::weak_ptr�� �ı� ������ �ð� ������ �� ��ٸ�, ��ü��
// �ı��� ������ ��ü�� �����ϴ� �޸𸮰� �����Ǵ� ���� ���̿� �ð� ������ ���� �� �ִ�.
class ReallyBigType{};
auto pBigObj = std::make_shared<ReallyBigType>();	// ���� ū ��ü�� std::make_shared�� �̿��ؼ� ����
// ū ��ü�� ����Ű�� std::shared_ptr��� std::weak_ptr���� �����ؼ� ���
// ���⼭ ��ü�� ����Ű�� ������ std::shared_ptr�� �ı��ǳ�, std::weak_ptr���� ������ ��������
// �� �κп��� ū ��ü�� �����ϴ� �޸𸮴� ������ �Ҵ�
// ���⼭ ��ü�� ����Ű�� ������ std::weak_ptr�� �ı��ȴ�. ���� �����ϰ� ��ü�� �����ϴ� �޸𸮰� ����

// new�� ���� ����ϴ� ��쿡�� ReallyBigType ��ü�� ����Ű�� ������ std::shared_ptr�� �ı��Ǵ� ��� �� ��ü�� �޸𸮰� ����
std::shared_ptr<ReallyBigType> pBigObjnew(new ReallyBigType); // ���� ū ��ü�� new�� �̿��ؼ� ����
// ��ü�� ����Ű�� std::shared_ptr��� std::weak_ptr ���� �����ؼ� ���
// ��ü�� ����Ű�� ������ std:shared_ptr�� �ı��ǳ�, std::weak_ptr���� ������ �����ִ�. ��ü�� �޸� ����
// �������� ���� �޸𸮸� �Ҵ�
// ��ü�� ����Ű�� ������ std::weak_ptr�� �ı��ȴ�. �������� �����ϴ� �޸� ����

// new�� ����ϸ鼭 ���� ������ ������ ���� �ʴ� �ּ��� ��å�� new�� ����� �ٸ� ���� ���� ���� �ʴ� ���忡��
//����Ʈ �������� �����ڿ� ��� �Ѱ��ִ� ��.(���� ���� ����)

// 1. new�� ���� ��뿡 ����, make �Լ��� ����ϸ� �ҽ��ڵ� �ߺ��� ������ ��������, ���� �������� ���ǰ�, std::make_shared�� 
// std::allocate_shared�� ��� �� �۰� ���� �ڵ尡 ����ȴ�.
// 2. make�Լ��� ����� �Ұ��� �Ǵ� �������� ���δ� Ŀ���� �����ڸ� �����ؾ� �ϴ� ���� �߰�ȣ �ʱ�ġ�� �����ؾ� �ϴ� ��찡 �ִ�.
// 3. std::shared_ptr�� ���ؼ��� make �Լ��� �������� ��찡 �� �ִµ�, �ΰ��� ���� ���ڸ� (1) Ŀ���� �޸� ���� ����� ���� Ŭ������
// �ٷ�� ���� (2) �޸𸮰� �˳����� ���� �ý��ۿ��� ū ��ü�� ���� �ٷ��� �ϰ� std::weak_ptr���� �ش� std::shared_ptr�麸��
// �� ���� ��Ƴ��� ����̴�.
