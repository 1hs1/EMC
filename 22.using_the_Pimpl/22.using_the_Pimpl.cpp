//Pimpl ���뱸�� ����� ������ Ư�� ��� �Լ����� ���� ���Ͽ��� �����϶�.

#include <string>
#include <vector>
#include <memory>

//Pimpl ���뱸("pointer to implementation" idiom)�� Ŭ������ �ڷ� ������� ���� Ŭ����(�Ǵ� ����ü)�� ����Ű�� �����ͷ� ��ü�ϰ�, 
//���� Ŭ������ ���̴� �ڷ� ������� �� ���� Ŭ������ �ű��, �����͸� ���ؼ� �� �ڷ� ����鿡 ���������� �����ϴ� ����̴�.
class Gadget {};

class Widget { //widget.h ��� ���� �ȿ���
public:
	Widget();

private:
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3; //Gadget�� � ����� ���� ����
};

//�� Widget�� �ڷ� ��� std::string, std::vector, Gadget������ ���ǰ� �ʿ��ϴ�. 
//<string>�� <vector>�� ���� ������� ������ gadget.h�� ���� ����� �� �ִ�.c++98���� Pimpl ���뱸�� �����ߴٸ� 
//Wiget�� �ڷ������� ���� �ϰ� �������� �ʴ� ����ü�� ����Ű�� �����ͷ� ��ü �Ѵ�.

class Widget2 {
public:
	Widget2();
	~Widget2();
private:
	struct Impl; //������ ����ü�� �װ���
	Impl *pImpl; //����Ű�� ������ ����
};

//WidgetŬ������ �ʿ������� ������� #include�� �߰��� �ʿ䰡 ������,
//�� ������� ����Ǿ Widget Ŭ���̾�Ʈ���� ������ ��ġ�� �ʴ´�.
//�̴� �����ϰ� ���Ǵ� ���� �ʴ� �ҿ��� ����(incomplete type)���� �Ҹ���, Widget::Impl�� �׷� �����̴�.

//��ó�� Pimpl ���뱸 ������ �ܰ踦 ����
//ù ��°, �ҿ��� ������ ����Ű�� �����͸� �ϳ��� �ڷ� ����� �����ϴ� ��
//�� ��°, ���� Ŭ�������� ����ϴ� �ڷ� ������� ��� ��ü�� �������� �Ҵ�, �����ϴ� �ڵ带 �߰��ϴ� ��(�ش� �ڵ�� �ҽ�����(.cpp)�� �д�)

//widget.cpp
struct Widget2::Impl { //Widget�� �ִ� �ڷ� ������� ����
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;
};
Widget2::Widget2()
	: pImpl(new Impl) //�ڷ��� �Ҵ�
{}
Widget2::~Widget2()
{
	delete pImpl;
} //�ڷ��� ����

//�������� widget.h���� widget.cpp�� �Ű�����.���� ���� ��ü ������ ���� �Ҹ��ڰ� �ʿ��ѵ�, 
//�̶� ������ �ȶ��� �����ʹ� std::unique_ptr�̴�.

//widget.h
class Widget3 {
public:
	Widget3();
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl; //�� ������ ��� ���
};

//widget.cpp
//#include "widget.h"
//#include "gadget.h"

struct Widget3::Impl {
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;
};
Widget3::Widget3()
	: pImpl(std::make_unique<Impl>()) //std::make_unique�� �̿��� std::unique_ptr�� ����
{}

//�Ҹ��ڰ� ���� ������ std::unique_ptr�� �ı��ɶ� �ڵ����� ��ü�� �����ϱ� ������, 
//�Ҹ��ڸ� ���� �ʿ䰡 ����.������ �Ʒ��� ���� ��� ������ �߻��Ѵ�.

//#include "widget.h"
//Widget w;

//�� ������ w�� �ı��� ��, w�� �Ҹ��ڰ� ȣ��Ǵµ�, std::unique_ptr�� �̿��ϴ� WidgetŬ������ ���� �Ҹ��ڰ� ������� �ʾҴ�.
//�̶� �����Ϸ��� �Ҹ��ڸ� �����ϰ�, ��κ��� ǥ�� ������ �Լ��� delete���� �� �ҿ����� ������ ����Ű�� �ʴ��� �����Ѵ�.
//c++11�� static_assert�� �̿��� �����ϸ�, �̶� ���� �ƴ� ������ �����Ǿ� ���� �޽����� �߻��Ѵ�.

//�ذ����� std::unique_ptr<Widget::Impl>�� �ı��ϴ� �ڵ尡 ��������� �������� Widget::Impl�� ������ ������ �ǰ� �ϸ� ������ �ٷ� �ذ�ȴ�.

//widget.h�� �߰�
//~Widget(); // ����

//widget.cpp������
//Widget::~Widget() {}//~Widget�� ����
//Widget::~Widget() = default;//~Widget�� ����


//���� �Ҹ��� ������ �ذ������, �׸� 17���� ���� Widget�� �Ҹ��ڸ� �����ϸ� �����Ϸ��� �̵� ������� �ۼ����� �ʴ´�.���� �̵� ������� ���� �����ؾ� �Ѵ�.
//�Ϲ������� �����Ϸ��� �̵� ������ �ȿ��� ���ܰ� �߻����� �� pImpe�� �ı��ϱ� ���� �ڵ带 �ۼ��ϴµ� �̶�, 
//pImpl�� �ı��Ϸ��� Impl�� ������ �����̾�� �Ѵ�.�̴� ���� �Ҹ��� �����͵� �����ϴ�.��, �̵� ������� ���Ǹ� �������Ϸ� �ű�� �ȴ�.
//widget.h
class Widget4 {
public:
	Widget4();
	~Widget4();

	// fail : �̵� ������ �ȿ� ���� �߻��� Impl �ı��Ϸ��� Impl�� �������� �̿��� �Ѵ�.
	//Widget4(Widget4&& rhs) = default;
	//Widget4& operator=(Widget4&& rhs) = default;

	Widget4(Widget4&& rhs); //���� �ϰ� �������� �ʴ´�.
	Widget4& operator=(Widget4&& rhs);
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

//widget.cpp
#include <string>

struct Widget4::Impl { };
Widget4::Widget4()
	: pImpl(std::make_unique<Impl>())
{}

Widget4::~Widget4() = default;

Widget4::Widget4(Widget4&& rhs) = default; //���⼭ ����
Widget4& Widget4::operator=(Widget4&& rhs) = default;

//����, ���࿡ Widget Ŭ������ std::string, std::vector ó�� Gadget�� ���簡 ������ �����̶��, 
//���� �����ڸ� ���� �����ؾ��Ѵ�.�ֳ��ϸ� std::unique_ptr���� �̵� ���� ������ �ִ� Ŭ������ ���ؼ��� ���� ������� �ۼ��� ���� ������, 
//�ۼ��Ѵ� �ص� ���� ���縦 �����ϱ� �����̴�.
//widget.h
class Widget5 {
public:
	Widget5(const Widget5& rhs);
	Widget5& operator=(const Widget5& rhs);
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

//widget.cpp
//#include "widget.h"

struct Widget5::Impl { };
//Widget5::~Widget5() = default;

Widget5::Widget5(const Widget5& rhs) //���� ������
	: pImpl(nullptr)
{
	if (rhs.pImpl)
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
}

Widget5& Widget5::operator=(const Widget5& rhs) //���� ���� ������
{
	if (!rhs.pImpl)
		pImpl.reset();
	else if (!pImpl)
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	else
		*pImpl = *rhs.pImpl;

	return *this;
}

//Pimpl ���뱸�� ������ ����ϱ� ������ �����ʹ� std::unique_ptr�̴� std::shared_ptr�� ���, �� �׸��� �������� �߻����� �ʴ´�.

//std::shared_ptr�� ����� ���
class Widget6 {
public:
	Widget6();
private:
	struct Impl;
	std::shared_ptr<Impl> pImpl;
};

//����ϴ� ���
//Widget w1;
//auto w2(std::move(w1)); //w2�� �̵� ����
//w1 = std::move(w2); //w1�� �̵� ����

//��� ���� ����� �����ϵǰ� ����ȴ�.std::unique_ptr���� �������� ������ �ش� �ȶ��� ������ ������ �Ϻ��̸�, 
//�� ���п� �����Ϸ��� �� ���� ���� ���� �ڷᱸ���� �� ���� ������� �ڵ带 ����� �� �� �ִ�.
//�ݸ� std::shared_ptr������ �������� ������ �ȶ��� ������ ������ �Ϻΰ� �ƴϴ�.������ �ڷᱸ���� �� Ŀ���� ���� �ڵ尡 �ټ� �� ��������.


int main()
{

	return 0;
}

//- Pimpl ���뱸�� Ŭ���� ������ Ŭ���� Ŭ���̾�Ʈ ������ ������ �������� �������ν� ���� �ð��� �����Ѵ�.
//- std::unique_ptr ������ pImpl �����͸� ����� ������ Ư�� ��� �Լ����� Ŭ���� �ش��� �����ϰ� ���� ���Ͽ��� �����ؾ� �Ѵ�.
//  �����Ϸ��� �⺻���� �ۼ��ϴ� �Լ� �������� ����ϱ⿡ ������ ��쿡�� �׷��� �ؾ� �Ѵ�.
//- ���� ������ std::unique_ptr�� ����� ��, std::shared_ptr���� ������� �ʴ´�.