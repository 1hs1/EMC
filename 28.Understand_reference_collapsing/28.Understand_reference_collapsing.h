#pragma once
//�׸� 28: ���� ����� �����϶�

template<typename T>
void func(T&& param);

//���ø� �Ű����� T�� ���� ������ ������ param�� �ް����� ���������� ���� ��ȣȭ�� ���� �� �� �ִ�.
//�ް� �μ��� ���޵Ǹ� �ް� ������ ����, �������� ���޵Ǹ� ������ �������� �����ȴ�.

Widget&& widgetFactory(); //�������� �����ִ� �Լ�
Widget w; //����(�ް�)
func(w); //�ް� ȣ�� Widget&�� ����
func(widgetFactory()); //������ ȣ�� Widget���� ����

//�ް� �������� ������ �������� ���еǴ� ���� std::forward�� ���ۿ����� ����ϴ�.

int x;
auto& & rx = x; //error ������ ���� ������ ���� �Ұ�

//���� ������ �޴� �Լ� ���ø��� �ް� ����
template<typename T>
void func(T&& param);
func(w); //func�� �ް����� ȣ���Ѵ�. T�� Widget&�� �����ȴ�.

//T�� ���� ������ �������� ���ø��� �ν��Ͻ�ȭ�� ���
void func(Widget& && param);
//������ ��������� ���� �Լ�
void func(Widget& param);

//������ �Լ��� ������� ����� ���� ���(reference collapsing)�̶� �Ѵ�.

//������ ���� ������ ���������� �������� ������, 
//Ư�� ���ƿ����� �����Ϸ��� ������ ���� ������ �����ϴ� ���� ���ȴ�.

//"���� �� ���� �� �ϳ��� �ް� �����̸� ����� �ް� �����̴�. 
//�׷��� ������(��, �Ѵ� ������ �����̸�)����� ������ �����̴�."

//�⺻������ std::forward�� �۵��ϴ� ���� ���� ��� �����̴�.
//�������� �Ű������� ����

template<typename T>
T&& forward(typename remove_reference<T>::type& param)
{
	return static_cast<T&&>(param);
}

//���޵� �μ��� Widget������ �ް��� ��� : T - Widget& ����
Widget& && forward(typename remove_reference<Widget&>::type& param)
{
	return static<Widget& &&>(param);
}
//std::remove_reference<Widget&>::type�� Widget���� ����
//����, ��������� ����� ��������
Widget& forward(Widget& param)
{
	return static_cast<Widget&>(param);
}

//���޵� �μ��� Widget������ �������� ��� : T - Widget ����
Widget && forward(typename remove_reference<Widget>::type& param)
{
	return static<Widget &&>(param);
}
//������ ���� ������ �����Ƿ� ����������
//��������
Widget&& forward(Widget& param)
{
	return static_cast<Widget&&>(param);
}

//c++14
template<typename T>
T&& forward(remove_reference_t<T>& param)
{
	return static_cast<T&&>(param);
}


//���� ����� �Ͼ�� ������ �װ�����,
//case1. ���ø� �ν��Ͻ�ȭ
//case2. auto������ ���� ���� ����
//case3. typedef�� ��Ī ������ ���� ���
//case4 decltype�� ���, �����Ϸ��� decltype�� ���� ������ �м��ϴ� ���� ������ ���� ������ �߻��ϸ� ���� ����� �߻�

//case2. auto ���� ���� ������ ��
template<typename T>
void func(T&& param);
Widget widgetFacotry(); //�������� �����ִ� �Լ�
Widget w; //���� �ް�

auto&& w1 = w;
//Widget&�� w1�� ���Խ� ������ ������ �߻�
Widget& && w1 = w; //��������� w1�� �ް� ����

//w2�� ���
auto&& w2 = widgetFactory();
//auto�� ������ ���� Widget���� �����ȴ�.
Widget&& w2 = widgetFactory(); //w2�� ������ �����̴�.

//���������� ���ο� ������ ������ �ƴϴ�.�Ʒ��� �� ���� ������ ���ƿ��� ���� ������ ������ �����̴�.
//- ���� �������� �ް��� �������� ���еȴ�.T������ �ް��� ����T&�� �����ǰ�, T������ �������� ����T�� �����ȴ�.
//- ���� ����� ����ȴ�.

//case3. typedef�� ���� �Ǵ� �򰡵Ǵ� ���߿� ������ ���� ������ �߻��ϸ� ���� ���
template<typename T>
class Widget {
public:
	typedef T&& RvalueReToT;
};
Widget<int&> w;
//Widget�� T�� int&�� ��ü
typedef int& && RvalueRefToT;
//��������� ����ȴ�.
typedef int& RvalueRefToT;
//Widget�� �ް� ���� �������� �ν��Ͻ�ȭ�ϸ�, RvalueRefToT�� ������ ������ �ƴ϶� �ް� ������ ���� typedef�� �ȴ�.

//case4. decltype���
//�����Ϸ��� decltype�� �����ϴ� ������ �м��ϴ� ���߿� ������ ���� ������ �߻��ϸ� ������� (�׸�3 ����)

//- ���� ����� ���ø� �ν��Ͻ�ȭ, auto ���� ����, typedef�� ��Ī ������ ���� �� ���, 
//  decltype�� ���� �� ����̶�� �װ��� ���ƿ��� �Ͼ��.
//- �����Ϸ��� ���� ��� ���ƿ��� ������ ���� ������ ����� ����, �� ����� �ϳ��� ������ �ȴ�.
//  ������ �� ���� �� �ϳ��� �ް� �����̸� ����� �ް� �����̰�, �׷��� ������ ������ �����̴�.
//- ���� ������ �ް��� �������� �����ϴ� ���ư� ���� ����� �Ͼ�� ���ƿ��� ���� ���������� ���� ���̴�.