
// auto의 형식 연역 규칙을 숙지하라

// auto를 이용하여 변수 선언시 템플릿의 T와 동일한 역할을 하며, 
// 변수의 형식 지정자는 ParamType과 동일한 역할을 한다.

// auto -> 템플릿T
// 변수의 형식 지정자 -> ParamType
auto x = 27;
const auto cx = x;
const auto& rx = x;

// x와 cx, rx의 형식들을 연역할 때, 컴파일러는 마치 선언마다 템플릿 함수 하나와 해당 초기화 표현식으로 
// 그 템플릿 함수를 호출하는 구문이 존재하는 것처럼 행동한다.
template< typename T >
void func_for_x(T param); //x의 형식을 연역하기 위한 개념적인 템플릿
func_for_x(27);
template< typename T >
void func_for_cx(const T param); //cx의 형식을 연역하기 위한 개념적인 템플릿
func_for_cx(x);
template< typename T >
void func_for_rx(const T& param); //rx의 형식을 연역하기 위한 개념적인 템플릿
func_for_rx(x);

// case 1 : 형식 지정자가 포인터나 참조 형식이지만 보편 참조는 아닌 경우
// case 2 : 형식 지정자가 보편 참조인 경우
// case 3 : 형식 지정자가 포인터도 아니고 참조도 아닌 경우
auto x = 27; //경우 3
const auto cx = x; //경우 3
const auto& rx = x; //경우 1

auto&& uref1 = x; //경우 2. x는 왼값, uref1의 형식은 int&
auto&& uref2 = cx; //경우 2. cx는 const int이자 왼값, uref2의 형식은 const int&
auto&& uref3 = 27; //경우 2. 27은 int이자 오른 값이므로 uref3의 형식은 int&&

// 비참조 형식 지정자의 경우 배열과 함수 이름이 포인터로 붕괴하는 예, auto도 동일하다.
const char name[] = "R. N. Briggs"; //name의 형식은 const char[13];
auto arr1 = name; //arr1의 형식은 const char*
auto& arr2 = name; //arr2의 형식은 const char (&)[13]


// auto, 템플릿 형식연혁 다른 점 : 중괄호 초기치가 관여할 때
// int변수를 선언하는 예를 보자.총 4가지 방법이 있다.
int x1 = 27;
int x2(27);
int x3 = { 27 }; //c++11 균일 초기화
int x4{ 27 }; //c++11 균일 초기화

// auto로 사용할 경우
auto x1 = 27;     //int, 27
auto x2(27);     //int, 27
auto x3 = { 27 }; //std::initializer_list<int>, {27}
auto x4{ 27 };     //std::initializer_list<int>, {27}

// auto로 선언된 변수의 초기치가 중괄호 쌍으로 감싸인 형태이면, 연역된 형식은 std::initializer_list이다.
auto x5 = { 1, 2, 3.0 }; //error. std::initializer_list<T>의 T를 연역할 수 없음

// auto로 선언된 변수를 중괄호 초기치로 초기화하는 경우, 연역된 형식은 std::initializer_list의 한 인스턴스이다.

auto x = { 11, 23, 9 }; //x의 형식은 std::initializer_list<int>

template< typename T >
void f(T param); //x의 선언에 해당하는 매개변수 선언을 가진  템플릿
f({ 11, 23, 9 }); //error T에 대한 형식을 연역할 수 없음

// param의 형식이 어떤 알려지지않은 T에 대한 std::initialzer_list<T>인 템플릿에 그 중괄호 초기치를 전달하면
// 템플릿 형식 연역 규칙들에 의해 T의 형식이 제대로 연역된다.

template< typename T >
void f(std::initializer_list<T> initList);
f({ 11, 23, 9 }); //T는 int로, initList의 형식은 std::initializer_list<int>로 연역된다.

// auto 형식 연역과 템플릿 형식 연역의 실질적인 차이는, auto는 중괄호 초기치가 std::initializer_list를 나타낸다고 가정하지만 
// 템플릿 형식 연역은 그렇지 않다는 것뿐이다.


// c++14
// 함수의 반환 형식을 auto로 지정해서 컴파일러가 연역하게 만들 수 있으며, 
// 람다의 매개변수 선언에 auto를 사용하는 것도 가능하다.이는 템플릿 형식 연역의 규칙들이 적용되며, 
// 중괄호 초기치를 돌려주는 함수의 반환 형식을 auto로 지정하면 컴파일이 실패한다.
auto createInitList()
{
	return{ 1, 2, 3 }; //error { 1, 2, 3 }의 형식을 연역할 수 없음
}
// 람다의 매개변수 형식 명세도 동일하다.
auto resetV = [&v](const auto& newValue) { v = newValue; }
resetV({ 1, 2, 3 }); //error { 1, 2, 3 }의 형식을 연역할 수 없음


// - auto형식 연역은 대체로 템플릿 형식 연역과 같지만, 
//   auto 형식 연역은 중괄호 초기치가 std::initializer_list를 나타낸다고 가정하는 반면 템플릿 형식 연역은 그렇지 않다는 차이가 있다.
// - 함수의 반환 형식이나 람다 매개변수에 쓰인 auto에 대해서는 auto 형식 연역이 아니라 템플릿 형식 연역이 적용된다.