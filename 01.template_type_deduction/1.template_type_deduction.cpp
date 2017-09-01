// 1.template_type_deduction.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

// case1. ParamType이 포인터 또는 참조형식
template<typename T>
void f(T& param){}

template<typename T>
void cf(const T& param){}

template<typename T>
void pf(T* param){}

// case2. ParamType이 보편참조임(오른값참조나 왼값참조 모두 가능)
template<typename T>
void rf(T&& param){}

// case3. ParamType이 값
template<typename T>
void vf(T param){}

// 배열의 크기를 컴파일 시점 상수로서 돌려주는 템플릿 함수
// constexpr: 함수호출의 결과를 컴파일 도중에 사용
// noexcept: 컴파일러가 더 나은 코드를 산출하도록 도움
template<typename T, size_t N>
constexpr size_t arraySize(T(&)[N]) noexcept
{
	return N;
}

void someFunc(int, double){}

int main()
{
	// 템플릿을 쓰면, 컴파일러에서 두가지 형식(T, ParamType)을 연역한다.
	int x = 27;
	const int cx = x;
	const int& rx = x;
	f(x);	// T는 int, ParamType은 int&
	f(cx);	// T는 const int, ParamType은 const int&
	f(rx);	// T는 const int, ParamType은 const int&

	cf(x);	// T는 int, ParamType은 const int&
	cf(cx);	// T는 int, ParamType은 const int&
	cf(rx);	// T는 int, ParamType은 const int&

	const int* px = &x;
	pf(&x);	// T는 int, ParamType은 int*
	pf(px);	// T는 const int, ParamType은 const int*



	rf(x);	// x는 왼값, T는 int&, ParamType은 int&
	rf(cx);	// cx는 왼값, T는 const int&, ParamType은 const int&
	rf(rx);	// rx는 왼값, T는 const int&, ParamType은 const int&
	rf(27);	// 27은 오른값, T는 int, ParamType은 int&&



	// Param은 인수의 복사본, 완전히 새로운 객체
	const char* const ptr = "Fun with pointers";	// const 객체를 가리키는 const 포인터, 오른쪽 const 다른 장소를 가리키도록 변경안됨(null도 안됨), 왼쪽 const 문자열도 못바꿈.
	const char name[] = "J.P.Briggs";	// name의 형식은 const char[13]
	const char* ptrToName = name;		// 배열이 포인터로 decay
	vf(x);	// T는 int, ParamType은 int
	vf(cx);	// T는 int, ParamType은 int
	vf(rx);	// T는 int, ParamType은 int
	vf(ptr);	// 포인터 자체는 값으로 전달된다. T,ParamType은 const char*(글자는 못바꾸나, 대상은 변경가능)
	vf(name);	// T, ParamType는 const char*
	f(name);	// T는 const char[13], ParamType은 const char(&)[13]
	vf(someFunc);	// ParamType은 void (*)(int, double)
	f(someFunc);	// ParamType은 void (&)(int, double)
	
    return 0;
}

