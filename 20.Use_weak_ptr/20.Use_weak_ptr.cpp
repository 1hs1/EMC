
//std::shared_ptró�� �۵��ϵ� ����� ���� ���� �ִ� �����Ͱ� �ʿ��ϸ� std::weak_ptr�� ����϶�.

#include<memory>
#include<unordered_map>

class Widget {};

using WidgetID = int32_t;
std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
	static std::unordered_map<WidgetID, std::weak_ptr<const Widget>> cache;

	auto objPtr = cache[id].lock(); //objPtr�� ĳ�� ��ü�� ����Ű�� std::shared_ptr

	if (!objPtr) {
		//objPtr = loadWidget(id); //ĳ�ÿ� ������ �����ϰ� ĳ�ÿ� ����
		cache[id] = objPtr;
	}
	return objPtr;
}

int main()
{
	//�ڽ��� ����Ű�� �Ǿ� �ִ� ��ü�� ���� ������ ������ ���ִ� �����Ͱ� std::weak_ptr�̴�.
	//std::weak_ptr�� std::shared_ptr�� �����ϴ� �������̴�.std::weak_ptr�� ����Ƚ���� ������ ��ġ�� �ʴ´�.
	
	auto spw = std::make_shared<Widget>(); //spw ���� �� ����Ƚ�� 1
	std::weak_ptr<Widget> wpw(spw); //wpw�� spw�� ���� ����Ƚ�� 1
	spw = nullptr; //������ 0, wpw�� ����� �Ҵ´�.

	if (wpw.expired()) {} //wpw�� ��ü�� ����Ű�� ������..

	//expired ȣ��� ������ ���̿��� �ٸ� � �����尡 �ش� ��ü�� ����Ű�� ������ std::shared_ptr�� ����� Ȥ�� �ı��� �� �ִ�.
	//������ std::weak_ptr�� ���� ���θ� �����ϰ� ���� ������� �ʾ�����, ����Ī ��ü�� ���� ������ �����ִ� ������ �ϳ��� ������ �������� �����Ѵ�.

	//��������� �̹� ����� std::weak_ptr�κ��� std::shared_ptr�� �������� ���� �ൿ��Ŀ� ���� �� ������ ������.

	//case1. std::weak_ptr::lock�� ����ϴ� ������, �̴� std::shared_ptr��ü�� �����ָ�, ����Ǿ��� ��, null�� �����ش�.
	std::shared_ptr<Widget> spw1 = wpw.lock(); //wpw�� �����̸� spw1�� null
	auto spw2 = wpw.lock(); //���� ����. auto ���

	//case2., std::weak_ptr�� �μ��� �޴� std::shared_ptr �����ڸ� ����ϴ� ��.�� ���, std::weak_ptr�� ����Ǿ��ٸ� ���ܰ� �߻�
	std::shared_ptr<Widget> spw3(wpw); //wpw�� ����� bad_weak_ptr�߻�

	//std::weak_ptr�� �����ϰ� ���Ǵ� ��츦 ���캸��.
	//ù ��°
	//� ���丮 �Լ�, �־��� ���� ID�� �ش��ϴ� �б� ���� ��ü�� ����Ű�� �����͸� ������
	//std::unique_ptr<const Widget> loadWidget(WidgetID id);

	//���⼭ loadWidget�� ����� Ŭ ���, �̸� �ذ��ϱ� ���� ȣ�� ������� ĳ���ϴ� �Լ��� �ۼ��ؾ� �Ѵ�.
	//���� ������ ������ �����ϸ�, ��ȯ ������ std::unique_ptr�� �ٶ������� �ʴ�.ĳ�ÿ� �ִ� �����͵��� �ڽ��� ����� �Ҿ����� �����ؾ� �Ѵ�.
	//��, ����� �Ҿ����� ������ ������ std::weak_ptr�̾�� �Ѵ�.	
	//loadWidget�� ĳ�� ����
	WidgetID wid = 1;
	auto w = fastLoadWidget(wid);
	
	//�� ��°, Observer���� ����, �����ڵ��� ����Ű�� std::weak_ptr���� �����̳ʸ� �ڷ� ����� �δ� ���̴�.
	//����, ��ü a, b, c�� �̷���� �ڷᱸ������, a�� c�� b�� �������� �����ϴ�, 
	//�׷��� b�� ����Ű�� std::shared_ptr�� ���� ��Ȳ���� b���� �ٽ� a�� ����Ű�� �����Ͱ� �ʿ��� ���

	//�������� 3�����̴�.
	//- �������� : �����Ͱ� �ı��Ǵ� ��� ������ ��Ȳ �߻�
	//- std::shared_ptr : a�� b�� ����Ű��, b�� a�� ����Ű�� ��ȯ���� ���� a, b�Ѵ� �ı����� ���ϸ�, a, b�� ������ �� ����.
	//- std::weak_ptr : �� 2 ������ ��� �ذ��Ѵ�.

	//std::weak_ptr�� std::shared_ptr�� ���������� �����ϸ�, �� ũ����� ����.std::weak_ptr�� ��ü�� ������ ������ �������� ������, ���� ����Ī ��ü�� ����Ƚ���� ������ ��ġ�� �ʴ´�.���� ��Ͽ��� '�� ��°' ���� Ƚ���� ������, �װ��� std::weak_ptr�̴�.

	return 0;
}

//- std::shared_ptró�� �۵��ϵ� ����� ���� ���� �ִ� �����Ͱ� �ʿ��ϸ� std::weak_ptr�� ����϶�.
//- std::weak_ptr�� �������� �뵵�δ� ĳ��, ������ ���, �׸��� std::shared_ptr ��ȯ�� ������ �ִ�