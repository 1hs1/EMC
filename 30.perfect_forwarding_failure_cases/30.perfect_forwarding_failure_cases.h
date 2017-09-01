#pragma once
//완벽 전달이 실패하는 경우들을 잘 알아두라.


//완벽전달은 단순히 객체들을 전달하는 것만이 아니라, 
//그 객체들의 주요 특징, 즉 그 형식, 왼값 또는 오른값 여부, const나 volatile 여부까지도 전달하는 것을 말한다.
//이를 위해 참조 매개변수가 필요하며, 구체적으로는 보편 참조 매개변수가 필요하다.


//f라는 함수에 전달하는 기초적인 형태
//임의의 형식과 개수들의 인수들을 받는 템플릿이기 때문에 가변 인수 템플릿으로 지정
template<typename... T>
void fwd(T&&... param)
{
	f(std::forward<T>(param)...);
}

//이러한 대상 함수 f와 전달 함수 fwd가 있다고 할 때, 
//만일 어떤 인수로 f를 호출했을 때 일어나는 일과 같은 인수로 fwd를 호출했을 때 일어나는 일이 다르다면 완벽 전달은 실패한 것이다.

//f(표현식);  //f와
//fwd(표현식);  //fwd가 다르다면 완벽전달은 실패한 것


//완벽하게 전달할수 없는 인수들의 종류
//case1. 중괄호 초기치
void f(const std::vector<int>& v); //f선언
f({ 1,2,3 }); //ok, 암묵적으로 std::vector<int>로 변환된다
fwd({ 1,2,3 }); //error, 컴파일 되지 않음

//컴파일러가 fwd의 호출 지점에서 전달된 인수들과 f에 선언된 매개 변수를 직접 비교할 수 없다.
//대신 컴파일러는 fwd에 전달되는 인수들의 형식을 연역하고, 연역된 형식들을 f의 매개변수 선언들과 비교한다.
//이때 다음 두 조건중 하나라도 만족되면 완벽 전달이 실패한다.

//- fwd의 매개변수들 중 하나 이상에 대해 컴파일러가 형식을 연역하지 못한다.
//- fwd의 매개변수들 중 하나 이상에 대해 컴파일러가 형식을 잘못 연역한다.

//fwd({ 1,2,3 }) 호출 문제는 std::initializer_list가 될 수 없는 형태로 선언된 함수 템플릿 매개변수에 중괄호 초기치를 넘겨주는 것으로, 
//fwd의 매개변수가 std::initializer_list가 될 수 없는 형태로 선언되어 있어 컴파일러가 연역하는 것이 금지되어 있다.

//우회책 : 항목2
auto il = { 1,2,3 }; //il의 형식 결과는 std::initializer_list<int>이다
fwd(il);


//case2. 널 포인터를 뜻하는 0 또는 NULL
//0과 NULL은 널 포인터로서 완벽하게 전달되지 못한다.때문에 해결책으로 nullptr을 사용한다.


//case3. 선언만 된 정수 static const 및 constexpr 자료 멤버
//일반적인 규칙으로, 정수 static const 자료 멤버와 정수 static constexpr 자료 멤버는 클래스 안에서 정의할 필요가 없다 
//컴파일러가 const 전파를 적용해서 따로 메모리를 마련할 필요가 없기 때문이다.

class Widget {
public:
	static constexpr std::size_t MinVals = 28; //MinVals의 선언
	//...
};
//... //MinVals의 정의는 없음
std::vector<int> widgetData;
widgetData.reserve(Widget::MinVals); //MinVals를 사용

//위 코드는 MinVals의 정의가 없어도 잘 동작하는데 MinVals가 언급된 모든곳에 28이라는 값을 배치함으로써 누락된 정의를 처리한다.
//하지만 MinVals를 가리키는 포인터를 만든다면 MinVals의 정의가 없어서 링크에 실패한다.

//동일한 예 f(fwd가 인수들을 전달하는 대상 함수)
void f(std::size_t val);
f(Widget::MinVals); //ok, f(28)로 처리됨
fwd(Widget::MinVals); //error, 링크에 실패한다.

//MinVals를 참조로 전달하는 것은 사실상 포인터로 넘겨주는 것이며, 그러면 포인터가 가리킬 뭔가가 필요하다.
//해당 정수를 static const 또는 constexpr 자료멤버의 정의를 제공하면 된다.

//Widget의 .cpp파일에서
constexpr std::size_t Widget::MinVals;


//case4. 중복적재된 함수 이름과 템플릿
//함수 f의 행동방식을 커스텀화하기 위해, f가 하나의 함수를 받아서 그 함수를 호출한다고 하자.
//그리고 그 함수가 int를 받고 int를 돌려준다고 하자.

void f(int(*pf)(int)); //pf는 처리함수를 뜻함

//f를 더간단한 비 포인터 구문으로 선언할수 있다.
void f(int pf(int)); //위와 동일한 f를 선언한다.

//중복 적재된 processVal함수
int processVal(int value);
int processVal(int value, int priority);
f(processVal); //ok

//이는 다소 놀라운데, f는 함수를 가리키는 포인터를 기대하지만 processVal은 함수 포인터가 아니다.
//하지만 매개변수 형식과 일치한다.그래서 컴파일러는 int를 받는 processVal을 선택해서 그 함수의 주소를 f에 넘겨준다.

//fwd는 호출 형식에 대한 정보가 없어 오류가 발생
fwd(processVal); //error, 어떤 processVal인지?

template<typename T>
T workOnVal(T param)
{
	//…
}
fwd(workOnVal);// 오류

//fwd 같은 완벽 전달 함수가 중복적재된 함수 이름이나 템플릿 이름을 받아들이게 하려면, 
//전달하고자 하는 중복 적재나 템플릿 인스턴스를 명시적으로 지정하면 된다.

using ProcessFuncType = int(*)(int); //typedef들을 만든다

ProcessFuncType processValPtr = processVal; //processVal에 필요한 서명을 명시

fwd(processValPtr); //ok

fwd(static_cast<ProcessFuncType>(workOnVal)); //ok


//case5. 비트필드
//비트필드가 함수 인수로 쓰일때 실패한다.

//IPv4헤더
struct IPv4Header {
	std::uint32_t version : 4, 
								IHL : 4, 
								DSCP : 6, 
								ECN : 2, 
								totalLength : 16;
};

//함수 f(전달 함수 fwd의 전달 대상)이 이번에는 std::size_t 형식의 매개변수를 받는다고 하자.

void f(std::size_t sz); //호출할 함수
IPv4Header h;
...
f(h.totalLength); //ok
fwd(h.totalLength); //error

//fwd의 매개변수가 참조이고, h.totalLength는 비const 비트필드라는 점이다.
//c++표준은 둘의 조합에 대해 "비const 참조는 절대로 비트필드에 묶이지 않아야 한다"라고 명시되어 있다.
//하지만 우회책이 있다.비트필드를 인수로 받는 임의의 함수는 그 비트필드의 값의 복사본을 받게 된다는 점을 이용한다.
//비트필드의 값 전달 방식은 값 전달 방식과 const에 대한 참조로 전달하는 것이다.
//즉, 그 const 참조는 비트필드 자체에 묶이는 것이 아니라 비트필드 값이 복사된 '보통'객체에 묶인다.

//비트필드 값을 복사한다
auto length = static_cast<std::uint16_t>(h.totalLength);
fwd(length); //복사본을 전달한다.


//- 완벽 전달은 템플릿 형식 연역이 실패하거나 틀린 형식을 연역했을 때 실패한다.
//- 인수가 중괄호 초기치이거나 0또는 NULL로 표현된 널 포인터, 
//  선언만 된 정수 static const 및 constexpr 자료 멤버, 템플릿 및 중복적재된 함수 이름, 비트필드이면 완벽 전달이 실패한다.