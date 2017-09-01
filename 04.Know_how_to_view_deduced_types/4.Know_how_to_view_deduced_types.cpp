// 연역된 형식을 파악하는 방법을 알아두라

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>


// 형식 연역 정보를 얻는 방법
// 코드 작성, 수정 시점과 컴파일 시점 나머지는 실행 시점이다.

// IDE편집기
// IDE(통합 개발 환경)의 코드 편집기 중에는 프로그램 개체위에 마우스 오버시 노출해 주는 경우가 많다.
// 이런 기능이 가능하려면 편집기의 코드가 어느정도 컴파일 가능한 상태이어야 한다.
// 이런 기능이 가능한 이유는 IDE안에서 c++컴파일러가 실행되기 때문이다.


// 컴파일러의 진단 메시지
// 컴파일 오류가 발생하도록 만드는 것
template<typename T>
class TD; //TD를 선언만 해둔다.


// 실행시점 출력
// printf를 이용해 표시하는 방법
//하지만 컴파일러 오류 메시지를 통해서 정확히 알기는 어렵다.
class Widget {};
std::vector<Widget> createVec()
{
	std::vector<Widget> vw;
	Widget w;
	vw.push_back(w);
	return vw;
};
template<typename T>
void f(const T& param)
{
using std::cout;
cout << "T = " << typeid(T).name() << '\n'; //T를 표시 
cout << "param = " << typeid(param).name() << '\n'; //param의 형식을 표시
}
// std::type_info::name의 정보는 param의 형식을 틀리게 보고 했다. (표준을 준수 : 값 전달 매개변수로 취급 )
// 값 전달의 경우 만일 형식이 참조이면 참조성이 무시되며, 참조를 제거한 후의 형식이 const이면 해당 const성 역시 무시된다.
// 그래서 실제로 const Widget* const&인 param의 형식이 const Widget*으로 출력된 것이다.
// const Widget*는 원래의 형식에서 먼저 참조성이 제거되고, 그래서 나온 포인터의 const성이 제거된 결과이다.
// 마찬가지로 IDE편집기가 표시하는 정보 또한 유용하지 않다.

//라이브러리를 신뢰한다면 Boost TypeIndex라이브러리가 있다.
#include <boost/type_index.hpp>
template<typename T>
void fb(const T& param)
{
using std::cout;
using boost::typeindex::type_id_with_cvr;
using boost::typeindex::type_id;
using boost::typeindex::type_id_runtime;

//T를 표시
cout << "T = " << type_id_with_cvr<T>().pretty_name() << '\n';
//param의 형식을 표시
cout << "param = " << type_id_with_cvr<decltype(param)>().pretty_name() << '\n';

//type_id의 형식을 표시
cout << "type_id param = " << type_id<decltype(param)>().pretty_name() << '\n';
//type_id_runtime의 형식을 표시
cout << "type_id_runtime param = " << type_id_runtime(param).pretty_name() << '\n';
}
// 함수 템플릿 boost::typeindex::type_id_with_cvr은 자신에게 전달된 형식 인수 const나 volatile, 참조 한정사들을 그대로 보존한다.
// type_id_with_cvr은 하나의 boost::typeindex::type_index객체를산출하며, 그 객체의 pretty_name멤버함수는 std::string객체를 돌려준다.

// Boost.TypeIndex는 정확한 출력을 낸다.IDE, 컴파일러 오류, Boost.TypeIndex는 컴파일러가 연역하는 형식을 파악하기 위한 도구일 뿐이다.
// 궁극적으로 중요한 것은 항목 1~3의 형식 연역 규칙의 숙지이다.

//- 컴파일러가 연역하는 형식을 IDE 편집기나 컴파일러 오류 메시지, Boost.TypeIndex 라이브러리를 이용해서 파악할 수 있는 경우가 많다.
//- 일부 도구의 결과는 유용하지도 않고 정확하지도 않을 수 있으므로, c++의 형식 연역 규칙들을 제대로 이해하는 것은 여전히 필요한 일이다.*/


int main()
{
	// IDE편집기
	const int theAnswer = 7;
	auto x = theAnswer;
	auto y = &theAnswer;

	// 컴파일러의 진단 메시지
	//TD<decltype(x)> xType;
	//TD<decltype(y)> yType;
	//error : 'xType'은(는) 정의되지 않은 class 'TD<int>'을(를) 사용합니다.
	//error : 'yType'은(는) 정의되지 않은 class 'TD<const int *>'을(를) 사용합니다.

	//실행시점 출력
	//typeid와 std::type_info객체에 name이라는멤버 함수 사용
	std::cout << typeid(x).name() << '\n'; //x의 형식은 i( int )
	std::cout << typeid(y).name() << '\n'; //y의 형식은 PKi(int const * __ptr64)
	const auto vw = createVec();
	if (!vw.empty())
	{
		// 템플릿 f에서 param의 선언된 형식은 const T&이다.그런데도 T와 param 같은 형식 출력
		// T가 int였다면, param의 형식은 그와는 다른 constint&가 되어야 한다.
		f(&vw[0]);
		// Boost.TypeIndex는 정확한 출력
		fb(&vw[0]);
	}

	return 0;
}