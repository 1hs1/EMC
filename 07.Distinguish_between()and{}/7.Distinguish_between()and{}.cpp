// 객체 생성 구문 소개

#include <vector>
#include <atomic>

// 비 정적 멤버 초기화 가능(클래스 선언부에서 초기화 가능)
class Widget {
private:
	int x{ 0 };	// OK
	int y = 0;	// OK
	//int z(0);	// 오류
};

int main()
{
	int x(0);	// 초기치를 괄호로 감싼 예
	int y = 0;	// 초기치를 "=" 다음에 지정한 예
	int z{ 0 };	// 초기치를 중괄호로 감싼 예
	int ze = { 0 };	// "="와 중괄호로 초기치를 지정한 예

	// 등호를 이용한 초기화 구문을 배정(assignment)한다고 오해
	Widget w1;	// 기본 생성자를 호출
	Widget w2 = w1;	// 배정이 아님, 복사생성자 호출
	w1 = w2;	// 배정, 복사 배정 연산자(operator=)를 호출

	// 중괄호 초기화, 균일 초기화
	std::vector<int> v{ 1,2,3 };

	// 복사할 수 없는 객체 std::atomic 항목 40
	std::atomic<int> ai1{ 0 };	// OK
	std::atomic<int> ai2(0);	// OK
	//std::atomic<int> ai3 = 0;	// 오류
}

// 어떤 표현식의 값을 초기화하려는 객체의 형식으로 온전하게 표현할 수 있는지를 보장해야함
double x, y, z;
//int sum1{ x + y + z };	// 오류 double의 합을 int로 표현하지 못할 수 있음
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
WidgetParam wp2();	// Widget을 반환하는 w2라는 이름의 함수를 선언
WidgetParam wp3{};	// 인수없이 Widget의 생성자를 호출

class Widgeti
{
public:
	Widgeti(){}
	Widgeti(int i, bool b){}
	Widgeti(int i, double d){}
	Widgeti(std::initializer_list<long double> il){}
	operator float() const {}
};

Widgeti wi1(10, true);	// 첫 생성자 호출
Widgeti wi2{ 10, true };	// std::initializer_list 생성자 호출(10과 true가 long double로 변환)
Widgeti wi3(10, 5.0);	// 둘째 생성자 호출
Widgeti wi4{ 10, 5.0 };		// std::initializer_list 생성자 호출(10과 5.0이 long double로 변환)
Widgeti wi5(wi4);		// 복사 생성자 호출
Widgeti wi6{ wi4 };		// std::initializer_list 생성자 호출 (w4가 float로 변환되고, float가 long double로 변환)
Widgeti wi7(std::move(wi4));	// 이동생성자 호출
Widgeti wi8{ std::move(wi4) };	// std::initializer_list 생성자 호출 (w4가 float로 변환되고, float가 long double로 변환)
// 위의 생성자를 Widgeti(std::initializer_list<bool> il) 변경시 wi4는 오류 발생
// bool은 int나 double값을 온전히 담지 못함
// 위의 생성자를 Widgeti(std::initializer_list<std::string> il) 변경시 wi2는 첫 생성자를 wi4는 둘째 생성자를 호출한다.

Widgeti wi9;	// 기본 생성자 호출
Widgeti wi10{};	// 기본 생성자 호출
Widgeti wi11();	// 함수 선언
Widgeti wi12({});	// std::initializer_list 생성자 호출
Widgeti wi13 {{}};	// std::initializer_list 생성자 호출

std::vector<int> v1(10, 20);	// 비 std::initializer_list 생성자를 사용.
								// 모든 요소의 값이 20인 요소 10개짜리 vector가 생성
std::vector<int> v2{ 10, 20 };	// std::initializer_list 생성자를 사용.
								// 10, 20 두 요소를 담은 vector 생성

// 1. 중괄호 초기화는 가장 광범위하게 적용할 수 있는 초기화 구문이며, 좁히기 변환을 방지하며, C++의 가장 성가신 구문해석에서 자유롭다.
// 2. 생성자 중복적재 해소 과정에서 중괄호 초기화는 가능한 한 std::initializer_list 매개변수가 있는 생성자와 부합한다.
//    (심지어 겉으로 보기에 그보다 인수들에 더 잘 부합하는 생성자들이 있어도.)
// 3. 괄호와 중괄호의 선택이 의미 있는 차이를 만드는 예는 인수 두 개로 std:vector<수치형식>을 새성하는 것이다.
// 4. 템플릿 안에서 객체를 생성할 때 괄호를 사용할 것인지 중괄호를 사용할 것인지 선택하기가 어려울 수 있다.