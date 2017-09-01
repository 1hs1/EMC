// 가능하면 항상 constexpr을 사용하라
// constexpr은 어떠한 값이 단지 상수일 뿐만 아니라 컴파일 시점에서 알려진다.

#include <array>

// 1. constexpr이 객체에 적용된 경우
// 객체는 실제로 const, 그 값은 실제로 컴파일 시점에서 알려진다.
// 컴파일 시점에 알려진 값들의 특별한 권한 :	읽기 전용 메모리에 배치될 수 있다. (내장형 시스템 개발자)
//												정수 상수 표현식이 요구되는 문맥에 사용
//												(배열 크기, 정수 템플릿 인수, 열거자 값)

int sz1;	// 비 constexpr 변수
//constexpr auto arraySize1 = sz1;	// 오류! sz1의 값이 컴파일 도중에 알려지지 않음
//std::array<int, sz1> data1;		// 오류!
constexpr auto arraySize2 = 10;		// 10은 확실히 컴파일 시점 상수
std::array<int, arraySize2> data2;	// arraySize2는 constexpr 객체

// const가 constexpr에서와 동일한 보장을 제공하지 않음. 
// const 객체가 반드시 컴파일 시점에서 알려지는 값으로 초기화 되지 않음
int sz2;
const auto arraySize = sz2;		// arraySize는 sz의 const 복사본
//std::array<int arraySize> data;	// 오류! arraySize의 값은 컴파일 시점에서 알려지지 않음 

// 모든 constexpr 객체는 const이지만 모든 const 객체가 constexpr은 아니다.

// 2. constexpr이 함수에 적용된 경우
// 컴파일 시점 상수를 인수로 해서 호쵤된 경우, 컴파일 시점 상수를 산출.
// 실행시점이 되어서야 알려지는 값으로 호출하면 실행시점 값을 산출.
constexpr
int powO(int base, int exp) noexcept { return 0; }	// pow는 결코 예외를 던지지 않는 constexpr 함수
constexpr auto numConds = 5;						// 조건들의 개수
std::array<int, powO(3, numConds)> results;			// result는 3^numConds 개의 요소들을 담는다.
// pow앞에 constexpr이 있다고 해서 pow가 반드시 const 값을 돌려주는 것은 아니고,
// 오직 base와 exp가 컴파일 시점 상수일 때에만 pow의 결과를 컴파일 시점 상수로 사용할 수 있다.
// base나 exp중 하나라도 컴파일 시점 상수가 아니면 pow의 결과가 실행시점에서 계산될 수 있다.

int readFromDB(std::string const& name) { return 0; }
auto base = readFromDB("base");		// 실행시점에서 값들을 구한다.
auto exps = readFromDB("exponent");
auto baseToExp = powO(base, exps);	// 실행시점에서 pow함수를 호출

// constexpr 함수는 컴파일 시점 값들로 호출했을 때 반드시 컴파일 시점 결과를 산출할 수 있어야 하므로, 구현에 일정한 제약이 따름
// C++11 버전 : 실행 가능 문장이 많아야 하나, return 문
constexpr int pow11(int base, int exp) noexcept
{
	return (exp == 0 ? 1 : base * pow11(base, exp - 1));	// 삼항연산자"?:", 재귀 사용
}

// C++14 버전 : 실행 가능 문장 제약이 느슨해짐 // VS2015에서는 안됨
//constexpr int pow14(int base, int exp) noexcept
//{
//	int result = 1;
//	for (int i = 0; i < exp; ++i) result *= base;
//
//	return result;
//}
//https://msdn.microsoft.com/ko-kr/library/hh567368.aspx

// constexpr 함수는 반드시 리터럴 형식들을 받고 돌려줘야 한다.(컴파일 도중에 값을 결정할 수 있는 형식)
// C++11에서 void를 제외한 모든 내장 형식이 리터럴 형식
// 생성자와 적절한 멤버함수들이 constexpr인 사용자 정의 형식도 리터럴 형식이 될 수 있다.

class Point {
public:
	constexpr Point(double xVal = 0, double yVal = 0) noexcept
		: x(xVal), y(yVal) {}

	constexpr double xValue() const noexcept { return x; }
	constexpr double yValue() const noexcept { return y; }

	void setX(double newX) noexcept { x = newX; }
	void setY(double newY) noexcept { y = newY; }
	//  C++11 에서는 constexpr 멤버함수는 암묵적으로 const로 선언하여, 대상 객체를 수정할 수 없다.
	// 반환형식이 void 인데, C++11에서 void는 리터럴 형식이 아니다.
	// C++14 에서는 위의 두 제약이 사라졌다.

private:
	double x, y;
};

// Point의 조회 함수들을 호출한 결과들로 또 다른 constexpr 객체를 초기화하는 constexpr 함수를 작성할 수 있다.
constexpr
Point midPoint(const Point& p1, const Point& p2) noexcept
{
	// constexpr 멤버 함수들을 호출
	return {(p1.xValue() + p2.yValue()) / 2, (p1.yValue() + p2.yValue()) / 2};
}

// C++14 버전에서만 가능
//constexpr Pointreflection(const Point& p) noexcept
//{
//	Point result;
//	result.setX(-p.xValue());
//	result.setY(-p.yValue());
//
//	return result;
//}

int main()
{
	constexpr Point p1(9.4, 27.7);	// constexpr 생성자가 컴파일 시점에서 실행됨
	constexpr Point p2(28.8, 5.3);

	constexpr auto mid = midPoint(p1, p2);	// constexpr 함수의 결과를 이용해서 constexpr 객체를 초기화

	// C++14에서만 가능
	//constexpr auto reflectedMid = Pointreflection(mid);
}



// 1. constexpr 객체는 const이며, 컴파일 도중에 알려지는 값들로 초기화 된다.
// 2. constexpr 함수는 그 값이 컴파일 도중에 알려지는 인수들로 호출하는 경우에는 컴파일 시점 결과를 산출한다.
// 3. constexpr 객체나 함수는 비constexpr 객체나 함수보다 광범위한 문맥에서 사용할 수 있다.
// 4. constexpr 객체나 함수의 인터페이스의 일부이다.