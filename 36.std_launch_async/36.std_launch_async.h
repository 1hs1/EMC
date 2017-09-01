#pragma once

// �񵿱⼺�� �ʼ��� ������ std::launch::async�� �����϶�

// std::launch::async �õ� ��ħ

// - std::launch::async : �񵿱���, �ٸ� �����忡�� ����

// - std::launch::deferred :  std::async�� ������ �̷���ü(std::future)�� ���� get, wait�� ȣ��� ������ ����,
//	 ���� ���� ȣ���� �Ͼ ������ �����ȴ�.
  
// �⺻ �õ� ��ħ
auto fut1 = std::async(f);

// �񵿱� �Ǵ� ���� ����
auto fut2 = std::async(std::launch::async |
	std::launch::deferred,
	f);
// ������ �ۿ� : ������ ������ �ı�, ���ٱ��� ȸ��, ���� ����ȭ 
// - f�� ���� ����� ���� �����Ƿ�, f�� t�� ���ÿ� ������� �����Ұ�
// - f�� fut�� ���� get, wait�� ȣ���ϴ� ������ʹ� �ٸ� �����忡�� ������� �����Ұ�
// - ���α׷��� ��� ���ɰ�ο��� fut�� ���� get, waitȣ�� ���� ����,
//   f�� �ݵ�� ����� ������ �����ϴ� ���� �Ұ�

// �⺻ �õ� ��ħ �����ٸ� ������ thread_local �������� ���� �� ���� �ʴ� ��쵵 ����.
// f�� �׷� ������ ���� �����(tread-local storage)�� �аų� ���� �ڵ尡 � �������� ���������� �����ϴ��� ���� �Ұ�

// �������� wait ��� ������ ����
// wait_for, wait_until ȣ��� std::future_status::deferred �� ��ȯ
// ���ѷ��� ���ɼ�
using namespace std::literals;
void f()
{
	std::this_thread::sleep_for(1s);
}
auto fut = std::async(f);
while (fut.wait_for(100ms) !=
	std::future_status::ready)
{
	...
}
// �ذ���
if (fut.wait_for(0s) ==
	std::future_status::deferred)
{
	...
}
else {
	while (fut.wait_for(100ms) !=
		std::future_status::ready) {
		��
	}
	��
}

// �⺻ �õ� ��ħ�� �Բ� std::async�� ����ϴ� ���� ���� ���ǵ��� ��� �����Ҷ� ����
// - ������ get�̳� wait�� ȣ���ϴ� ������� �ݵ�� ���������� ����Ǿ�� �ϴ� ���� �ƴϴ�.
// - ���� ������ �� � �������� thread_local �������� �а� �������� �߿����� �ʴ�.
// - std::async�� ������ �̷���ü�� ���� get,wait�� �ݵ�� ȣ��ȴٴ� ����, ������ ���� ������� �ʾƵ� ������.
// - ������ ������ ������ ���� �ִٴ� ���� wait_for, wait_until�� ����ϴ� �ڵ忡 �ݿ�


// �� ���� �� �ϳ��� �������� �ʴ´ٸ� std::async�� �־��� ������ �񵿱������� �����ϵ��� ����
auto fut = std::async(std::launch::async, f);

// std::launch::async ��������� �������� �ʵ���
// C++11
template<typename F, typename... Ts>
inline
std::future<typename std::result_of<F(Ts...)>::type>
reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async,
		std::forward<F>(f),
		std::forward<Ts>(params)...);
}

auto fut = reallyAsync(f);

// C++14
template<typename F, typename... Ts>
inline
auto // C++14
reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async,
		std::forward<F>(f),
		std::forward<Ts>(params)...);
}

// - std::async�� �⺻ �õ� ��ħ�� ������ �񵿱��� ����� ������ ������ ��� ����Ѵ�.
// - �׷��� �̷��� ������ ������ thread_local ������ ��Ȯ�Ǽ��� �߻��ϰ�, ������
//   ����� ������� ���� ���� �ְ�, �ð� ���� ��� wait ȣ�⿡ ���� ���α׷� ������ ������ ��ģ��.
// - ������ �ݵ�� �񵿱������� �����Ͼ� �Ѵٸ� std:launch::async�� �����϶�.