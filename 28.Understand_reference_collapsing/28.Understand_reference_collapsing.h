#pragma once
//항목 28: 참조 축약을 숙지하라

template<typename T>
void func(T&& param);

//템플릿 매개변수 T에 대해 연역된 형식의 param이 왼값인지 오른값인지 정보 부호화를 통해 알 수 있다.
//왼값 인수가 전달되면 왼값 참조로 연역, 오른값이 전달되면 비참조 형식으로 연역된다.

Widget&& widgetFactory(); //오른값을 돌려주는 함수
Widget w; //변수(왼값)
func(w); //왼값 호출 Widget&로 연역
func(widgetFactory()); //오른값 호출 Widget으로 연역

//왼값 참조인지 오른값 참조인지 구분되는 것은 std::forward의 동작원리와 비슷하다.

int x;
auto& & rx = x; //error 참조에 대한 참조는 선언 불가

//보편 참조를 받는 함수 템플릿에 왼값 전달
template<typename T>
void func(T&& param);
func(w); //func를 왼값으로 호출한다. T는 Widget&로 연역된다.

//T에 대해 연역된 형식으로 템플릿을 인스턴스화한 결과
void func(Widget& && param);
//실제로 만들어지는 최종 함수
void func(Widget& param);

//실제로 함수가 만들어진 결과를 참조 축약(reference collapsing)이라 한다.

//참조에 대한 참조는 정상적으로 동작하지 않지만, 
//특정 문맥에서는 컴파일러가 참조에 대한 참조를 산출하는 것이 허용된다.

//"만일 두 참조 중 하나라도 왼값 참조이면 결과는 왼값 참조이다. 
//그렇지 않으면(즉, 둘다 오른값 참조이면)결과는 오른값 참조이다."

//기본적으로 std::forward가 작동하는 것이 참조 축약 덕분이다.
//보편참조 매개변수에 적용

template<typename T>
T&& forward(typename remove_reference<T>::type& param)
{
	return static_cast<T&&>(param);
}

//전달된 인수가 Widget형식의 왼값인 경우 : T - Widget& 연역
Widget& && forward(typename remove_reference<Widget&>::type& param)
{
	return static<Widget& &&>(param);
}
//std::remove_reference<Widget&>::type이 Widget으로 산출
//또한, 참조축약이 적용된 최종버전
Widget& forward(Widget& param)
{
	return static_cast<Widget&>(param);
}

//전달된 인수가 Widget형식의 오른값인 경우 : T - Widget 연역
Widget && forward(typename remove_reference<Widget>::type& param)
{
	return static<Widget &&>(param);
}
//참조에 대한 참조가 없으므로 참조축약없다
//최종버전
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


//참조 축약이 일어나는 문맥은 네가지로,
//case1. 템플릿 인스턴스화
//case2. auto변수에 대한 형식 연역
//case3. typedef와 별칭 선언의 지정 사용
//case4 decltype의 사용, 컴파일러가 decltype에 관해 형식을 분석하는 도중 참조에 대한 참조가 발생하면 참조 축약이 발생

//case2. auto 변수 형식 연역의 예
template<typename T>
void func(T&& param);
Widget widgetFacotry(); //오른값을 돌려주는 함수
Widget w; //변수 왼값

auto&& w1 = w;
//Widget&를 w1에 대입시 참조대 참조가 발생
Widget& && w1 = w; //결과적으로 w1은 왼값 참조

//w2의 경우
auto&& w2 = widgetFactory();
//auto는 비참조 형식 Widget으로 연역된다.
Widget&& w2 = widgetFactory(); //w2는 오른값 참조이다.

//보편참조는 새로운 종류의 참조가 아니다.아래의 두 조건 만족시 문맥에서 보편 참조는 오른쪽 참조이다.
//- 형식 연역에서 왼값과 오른값이 구분된다.T형식의 왼값은 형식T&로 연역되고, T형식의 오른값은 형식T로 연역된다.
//- 참조 축약이 적용된다.

//case3. typedef가 지정 또는 평가되는 도중에 참조에 대한 참조가 발생하면 참조 축약
template<typename T>
class Widget {
public:
	typedef T&& RvalueReToT;
};
Widget<int&> w;
//Widget의 T를 int&로 대체
typedef int& && RvalueRefToT;
//참조축약이 적용된다.
typedef int& RvalueRefToT;
//Widget을 왼값 참조 형식으로 인스턴스화하면, RvalueRefToT는 오른값 참조가 아니라 왼값 참조에 대한 typedef가 된다.

//case4. decltype사용
//컴파일러가 decltype에 관련하는 형식을 분석하는 도중에 참조에 대한 참조가 발생하면 참조축약 (항목3 참조)

//- 참조 축약은 템플릿 인스턴스화, auto 형식 연역, typedef와 별칭 선언의 지정 및 사용, 
//  decltype의 지정 및 사용이라는 네가지 문맥에서 일어난다.
//- 컴파일러가 참조 축약 문맥에서 참조에 대한 참조를 만들어 내면, 그 결과는 하나의 참조가 된다.
//  원래의 두 참조 중 하나라도 왼값 참조이면 결과는 왼값 참조이고, 그렇지 않으면 오른값 참조이다.
//- 형식 연역이 왼값과 오른값을 구분하는 문맥과 참조 축약이 일어나는 문맥에서 보면 보편참조는 오른 값이다.