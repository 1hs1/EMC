#pragma once

//보편 참조와 오른값 참조를 구별하라


//어떤 형식 T에 대한 오른값 참조를 선언할 때 T&&라는 표기를 사용한다.하지만 T&&라고 해서 모두가 오른값 참조는 아니다.
void f(Widget&& param); //오른값 참조
Widget&& var1 = Widget(); //오른값 참조

auto&& var2 = var1; //오른값 참조 아님

template<typename T>
void f(std::vector<T>&& param); //오른값 참조

template<typename T>
void f(T&& param); //오른값 참조 아님

//T&&에는 두가지 행동이 있는데, 오른값 참조와 왼값참조이다.소스코드에서는 오른값 참조처럼 보이지만, 
//때에 따라서 왼값 참조(T&)인 것처럼 행동한다.

//이런  참조는 const 객체에 묶을 수도 있고 비const 객체에 묶을 수도 있으며, 
//마찬가지로 volatile 객체에 묶을 수도, 비volatile객체에 묶을 수도 있다.심지어 const이자 volatile인 객체에도 묶을 수 있다.
//이런 유연한 참조를 보편 참조(universal reference)라고 부른다.

//보편 참조는 두가지 문맥에서 나타난다.가장 흔한것이 함수 템플릿 매개변수, 그리고 auto 선언이다.
template<typename T>
void f(T&& param); //param은 보편 참조

auto&& var2 = var1; //var2는 보편 참조

//두 문맥의 공통점은 형식 연역이 일어난다.템플릿 f에서는 param의 형식 연역, 
//var2선언에서는 var2의 형식이 연역된다.이처럼 형식 연역이 일어나지 않는 문맥에서 "T&&"를 발견했다면, 그것은 오른값 참조이다.

void f(Widget&& param); //형식 연역 없음, param은 오른값 참조
Widget&& var1 = Widget(); //형식 연역 없음, var1은 오른값 참조


//보편 참조의 경우, 참조이므로 반드시 초기화를 해야하며, 오른값 참조인지 왼값 참조인지 보편 참조의 초기치가 결정한다.
template<typename T>
void f(T&& param); //param은 보편 참조
Widget w;
f(w); //f에 왼값이 전달됨, Widget& 왼값 참조
f(std::move(w)); //f에 오른값이 전달됨 Widget&& 오른 값 참조

//하나의 참조가 보편 참조이려면 반드시 형식 연역이 관여해야 한다.참조 선언의 형태도 정확해야 하는데, 그 형태는 상당히 제한적이다.
template<typename T>
void f(std::vector<T>&& param); //param은 오른값 참조

std::vector<int> v;
f(v); //error, 왼값을 오른값 참조에 묶을 수 없음

//f호출 시 형식 T가 연역된다.하지만 param의 형식 선언의 형태가 T&&가 아니라 std::vector<T>&&이며, 
//때문에 보편 참조가 될 수 없다.따라서 param은 오른 값 참조이다.

//그냥 const 한정사 하나만 붙여도 참조는 보편 참조가 되지 못한다.
template<typename T>
void f(const T&& param);


//그런데 T&&라고 다 보편 참조는 아니다.예를 들어 템플릿 안에서 형식이 T&&은 반드시 보편 참조라고 확신할 수 없다.
//템플릿 안에서는 형식 연역이 반드시 일어난다는 보장이 없기 대문이다.
template<class T, class Allocator = allocator<T>>
class vector {
public:
	void push_back(T&& x);
};

//push_back는 보편 참조 형태지만 형식 연역이 일어나지 않는다.push_back은 반드시 구체적으로 인스턴스화된 vector의 일부이어야 하며, 
//그 인스턴스의 형식은 push_back의 선언을 완전하게 결정하기 때문이다.
std::vector<Widget> v; //선언

class vector<Widget, allocator<Widget>> {
public:
	void push_back(Widget&& x); //오른 값 참조
	...
};

//push_back은 그 어떤 형식에도 관여하지 않으며, vector<T>에 대한 push_back은 항상 T에 대한 오른값 참조 형식의 매개변수를 선언한다.
//std::vector의 멤버 함수중 emplace_back는 실제로 형식 연역을 사용한다.
class vector {
	template <class... Args>
	void emplace_back(Args&&... args);
};

//매개변수 Args는 vector의 형식 매개변수 T와 독립 적이다.따라서 Args는 emplace_back가 호출될 때마다 연역되어야 한다.

//auto역시 보편 참조가 될 수 있다. auto&&를 형식으로 해서 선언된 변수는 보편 참조이다.


//- 함수 템플릿 매개변수의 형식이 T&& 형태이고 T가 연역된다면, 또는 객체를 auto&&로 선언한다면, 그 매개변수나 객체는 보편 참조이다.
//- 형식 선언의 형태가 정확히 형식&&가 아니면, 또는 형식 연역이 일어나지 않으면, 형식&&는 오른값 참조를 뜻한다.
//- 오른값으로 초기화되는 보편 참조는 오른값 참조에 해당한다.왼값으로 초기화되는 보편 참조는 왼값 참조에 해당한다.