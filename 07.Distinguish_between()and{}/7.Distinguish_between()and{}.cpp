// ��ü ���� ���� �Ұ�

#include <vector>
#include <atomic>

// �� ���� ��� �ʱ�ȭ ����(Ŭ���� ����ο��� �ʱ�ȭ ����)
class Widget {
private:
	int x{ 0 };	// OK
	int y = 0;	// OK
	//int z(0);	// ����
};

int main()
{
	int x(0);	// �ʱ�ġ�� ��ȣ�� ���� ��
	int y = 0;	// �ʱ�ġ�� "=" ������ ������ ��
	int z{ 0 };	// �ʱ�ġ�� �߰�ȣ�� ���� ��
	int ze = { 0 };	// "="�� �߰�ȣ�� �ʱ�ġ�� ������ ��

	// ��ȣ�� �̿��� �ʱ�ȭ ������ ����(assignment)�Ѵٰ� ����
	Widget w1;	// �⺻ �����ڸ� ȣ��
	Widget w2 = w1;	// ������ �ƴ�, ��������� ȣ��
	w1 = w2;	// ����, ���� ���� ������(operator=)�� ȣ��

	// �߰�ȣ �ʱ�ȭ, ���� �ʱ�ȭ
	std::vector<int> v{ 1,2,3 };

	// ������ �� ���� ��ü std::atomic �׸� 40
	std::atomic<int> ai1{ 0 };	// OK
	std::atomic<int> ai2(0);	// OK
	//std::atomic<int> ai3 = 0;	// ����
}

// � ǥ������ ���� �ʱ�ȭ�Ϸ��� ��ü�� �������� �����ϰ� ǥ���� �� �ִ����� �����ؾ���
double x, y, z;
//int sum1{ x + y + z };	// ���� double�� ���� int�� ǥ������ ���� �� ����
int sum2(x + y + z);	// OK
int sum3 = x + y + z;	// OK

class WidgetParam
{
public:
	WidgetParam() { m = 0; }
	WidgetParam(int a) { m = a; }

private:
	int m;
};

WidgetParam wp1(10);
WidgetParam wp2();	// Widget�� ��ȯ�ϴ� w2��� �̸��� �Լ��� ����
WidgetParam wp3{};	// �μ����� Widget�� �����ڸ� ȣ��

class Widgeti
{
public:
	Widgeti(){}
	Widgeti(int i, bool b){}
	Widgeti(int i, double d){}
	Widgeti(std::initializer_list<long double> il){}
	operator float() const {}
};

Widgeti wi1(10, true);	// ù ������ ȣ��
Widgeti wi2{ 10, true };	// std::initializer_list ������ ȣ��(10�� true�� long double�� ��ȯ)
Widgeti wi3(10, 5.0);	// ��° ������ ȣ��
Widgeti wi4{ 10, 5.0 };		// std::initializer_list ������ ȣ��(10�� 5.0�� long double�� ��ȯ)
Widgeti wi5(wi4);		// ���� ������ ȣ��
Widgeti wi6{ wi4 };		// std::initializer_list ������ ȣ�� (w4�� float�� ��ȯ�ǰ�, float�� long double�� ��ȯ)
Widgeti wi7(std::move(wi4));	// �̵������� ȣ��
Widgeti wi8{ std::move(wi4) };	// std::initializer_list ������ ȣ�� (w4�� float�� ��ȯ�ǰ�, float�� long double�� ��ȯ)
// ���� �����ڸ� Widgeti(std::initializer_list<bool> il) ����� wi4�� ���� �߻�
// bool�� int�� double���� ������ ���� ����
// ���� �����ڸ� Widgeti(std::initializer_list<std::string> il) ����� wi2�� ù �����ڸ� wi4�� ��° �����ڸ� ȣ���Ѵ�.

Widgeti wi9;	// �⺻ ������ ȣ��
Widgeti wi10{};	// �⺻ ������ ȣ��
Widgeti wi11();	// �Լ� ����
Widgeti wi12({});	// std::initializer_list ������ ȣ��
Widgeti wi13 {{}};	// std::initializer_list ������ ȣ��

std::vector<int> v1(10, 20);	// �� std::initializer_list �����ڸ� ���.
								// ��� ����� ���� 20�� ��� 10��¥�� vector�� ����
std::vector<int> v2{ 10, 20 };	// std::initializer_list �����ڸ� ���.
								// 10, 20 �� ��Ҹ� ���� vector ����

// 1. �߰�ȣ �ʱ�ȭ�� ���� �������ϰ� ������ �� �ִ� �ʱ�ȭ �����̸�, ������ ��ȯ�� �����ϸ�, C++�� ���� ������ �����ؼ����� �����Ӵ�.
// 2. ������ �ߺ����� �ؼ� �������� �߰�ȣ �ʱ�ȭ�� ������ �� std::initializer_list �Ű������� �ִ� �����ڿ� �����Ѵ�.
//    (������ ������ ���⿡ �׺��� �μ��鿡 �� �� �����ϴ� �����ڵ��� �־.)
// 3. ��ȣ�� �߰�ȣ�� ������ �ǹ� �ִ� ���̸� ����� ���� �μ� �� ���� std:vector<��ġ����>�� �����ϴ� ���̴�.
// 4. ���ø� �ȿ��� ��ü�� ������ �� ��ȣ�� ����� ������ �߰�ȣ�� ����� ������ �����ϱⰡ ����� �� �ִ�.