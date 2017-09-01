// 1.template_type_deduction.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

// case1. ParamType�� ������ �Ǵ� ��������
template<typename T>
void f(T& param){}

template<typename T>
void cf(const T& param){}

template<typename T>
void pf(T* param){}

// case2. ParamType�� ����������(������������ �ް����� ��� ����)
template<typename T>
void rf(T&& param){}

// case3. ParamType�� ��
template<typename T>
void vf(T param){}

// �迭�� ũ�⸦ ������ ���� ����μ� �����ִ� ���ø� �Լ�
// constexpr: �Լ�ȣ���� ����� ������ ���߿� ���
// noexcept: �����Ϸ��� �� ���� �ڵ带 �����ϵ��� ����
template<typename T, size_t N>
constexpr size_t arraySize(T(&)[N]) noexcept
{
	return N;
}

void someFunc(int, double){}

int main()
{
	// ���ø��� ����, �����Ϸ����� �ΰ��� ����(T, ParamType)�� �����Ѵ�.
	int x = 27;
	const int cx = x;
	const int& rx = x;
	f(x);	// T�� int, ParamType�� int&
	f(cx);	// T�� const int, ParamType�� const int&
	f(rx);	// T�� const int, ParamType�� const int&

	cf(x);	// T�� int, ParamType�� const int&
	cf(cx);	// T�� int, ParamType�� const int&
	cf(rx);	// T�� int, ParamType�� const int&

	const int* px = &x;
	pf(&x);	// T�� int, ParamType�� int*
	pf(px);	// T�� const int, ParamType�� const int*



	rf(x);	// x�� �ް�, T�� int&, ParamType�� int&
	rf(cx);	// cx�� �ް�, T�� const int&, ParamType�� const int&
	rf(rx);	// rx�� �ް�, T�� const int&, ParamType�� const int&
	rf(27);	// 27�� ������, T�� int, ParamType�� int&&



	// Param�� �μ��� ���纻, ������ ���ο� ��ü
	const char* const ptr = "Fun with pointers";	// const ��ü�� ����Ű�� const ������, ������ const �ٸ� ��Ҹ� ����Ű���� ����ȵ�(null�� �ȵ�), ���� const ���ڿ��� ���ٲ�.
	const char name[] = "J.P.Briggs";	// name�� ������ const char[13]
	const char* ptrToName = name;		// �迭�� �����ͷ� decay
	vf(x);	// T�� int, ParamType�� int
	vf(cx);	// T�� int, ParamType�� int
	vf(rx);	// T�� int, ParamType�� int
	vf(ptr);	// ������ ��ü�� ������ ���޵ȴ�. T,ParamType�� const char*(���ڴ� ���ٲٳ�, ����� ���氡��)
	vf(name);	// T, ParamType�� const char*
	f(name);	// T�� const char[13], ParamType�� const char(&)[13]
	vf(someFunc);	// ParamType�� void (*)(int, double)
	f(someFunc);	// ParamType�� void (&)(int, double)
	
    return 0;
}

