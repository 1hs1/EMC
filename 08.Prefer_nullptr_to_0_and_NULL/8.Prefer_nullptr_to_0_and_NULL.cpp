// 0�� NULL���� nullptr�� ��ȣ�϶�

#pragma once

#include <mutex>

// 0�� NULL�� ����Ʈ ������ �ƴϴ�. 0�̳� NULL�� �Լ��� ȣ�� ���� ��, 
// �����͸� �޴� �Լ��� ȣ�� �Ǵ� ���� ����.
void f(int);
void f(bool);
void f(void*);

//f(0); //f(int)ȣ��
//f(NULL); //f(int)ȣ��, ������ ������ �߻��Ǳ⵵ �Ѵ�.

//nullptr�� ������ �������İ� ����Ʈ ������ �ƴϴ�.���� ������ std::nullptr_t�̸�, �̴� �ٽ� "nullptr�� ����"���� ���ǵȴ�.
//std::nullptr_t�� ��� �� ������ �������� �Ϲ������� ��ȯ�Ǹ�, 
//������ nullptr�� ��ġ ��� ������ ������ó�� �ൿ�Ѵ�.
//f(nullptr); //f(void*) ȣ��

//nullptr�� ��� auto������ ����� �� �����ϴ�.
//auto result = findRecode(...);
//if (result == 0) {}; //result�� ����Ʈ���� �������� ��Ȯ�� �� �� ����.
//if (result == nullptr); //result�� ������ �������� ����ϴ�.

//nullptr�� ���ø� ���� �����ϴ�
class Widget {};
std::mutex f1m, f2m, f3m; //f1, f2, f3�� ���ؽ�
int			f1(std::shared_ptr<Widget> spw);
double	f2(std::unique_ptr<Widget> upw);
bool		f3(Widget* pw) { return true; };
/*
using MuxGuard =
std::lock_guard<std::mutex>;
{
	MuxGuard g(f1m);
	auto result = f1(0);
}
{
	MuxGuard g(f2m);
	auto result = f2(NULL);
}
{
	MuxGuard g(f3m);
	auto result = f3(nullptr);
}
*/
template<typename FuncType, typename MuxType, typename PtrType>
decltype(auto) lockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
{
	using MuxGuard = std::lock_guard<MuxType>;

	MuxGuard g(mutex);
	return func(ptr);
}

//auto result1 = lockAndCall(f1, f1m, 0); //error
//auto result2 = lockAndCall(f2, f2m, NULL); //error
auto result3 = lockAndCall(f3, f3m, nullptr); //ok

//result1�� 0�� �Ű������� int�̴�.������ std::shared_ptr<Widget>�Ű������� ȣȯ���� �����Ƿ� ������ ������ ����.
//result2�� NULL���� �����ϴ�. �ݸ�, nullptr�� �̿��� ȣ�⿡�� ������ ����.
//ptr�� ������ std::nullptr_t�� �����Ǿ� ptr�� f3�� �����ϸ� std::nullptr_t���� Widget*�� �Ϲ��� ��ȯ�� �Ͼ��.
//��ó�� �������͸� ������ ������ 0�̳� NULL�� �ƴ϶� nullptr�� ����϶�.


//����� �� ���׵�
//- 0�� NULL���� nullptr�� ��ȣ�϶�.
//- ���� ���İ� ������ ���Ŀ� ���� �ߺ����縦 ���϶�.


int main()
{
	return 0;
}