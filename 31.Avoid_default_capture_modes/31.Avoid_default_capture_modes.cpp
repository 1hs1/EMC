// 기본 갈무리 모드를 피하라

#include <vector>
#include <functional>
#include <memory>

// C++11의 기본 갈무리 모드는 두가지
// 1. 기본 참조 갈무리 모드 : 참조가 대상을 읽을 위험
// 2. 기본 값 갈무리 모드 : 참조가 대상을 잃는 문제가 없을 것 같지만 그렇지 않고,
//							자기 완결적일 것 같지만, 그렇지 않은 경우도 있다.

// 참조 갈무리를 사용하는 클로저는 지역 변수 또는 람다가 정의된 범위에서 볼 수 있는 매개변수에 대한 참조를 가진다.
// 람다에 의해 생성된 클로저의 수명이 그 지역 변수나 매개변수의 수명보다 오래 지속되면 클로저 안의 참조는 대상을 잃는다.
using FilterContainer = std::vector<std::function<bool(int)>>;
int computeSomeValue1() { return 1; }
int computeSomeValue2() { return 2; }
int computeDivisor(int, int) { return 3; }

void addDivisorFilter()
{
	FilterContainer filters;
	filters.emplace_back([](int value) { return value % 5 == 0; });	// OK!!

	auto calc1 = computeSomeValue1();
	auto calc2 = computeSomeValue2();

	auto divisor = computeDivisor(calc1, calc2);

	filters.emplace_back([&](int value) { return value % divisor == 0; });	// 위험!!
	// addDivisorFilter는 filters.emplace_back이 반환된 직후에 반환
	// filters에 추가되는 필터 함수는 사실상 이미 사망 상태로 도착
	// 명시적 갈무리도 같은 문제 발생
	// 그러나 이 람다 표현식의 유효성이 divisor의 수명에 의존한다는 점이 명확히 나타난다는 장점
}

template<typename C>
void workWithContainer(const C& container)
{
	auto calc1 = computeSomeValue1();
	auto calc2 = computeSomeValue2();

	auto divisor = computeDivisor(calc1, calc2);

	using ContElemT = typename C::value_type;	// 컨테이너에 담긴 요소들의 형식

	using std::begin;
	using std::end;
	
	if(std::all_of(begin(container), end(container),				// 한 구간의 모든 요소가 주어진 조건을 만족하는지의 여부를 돌려주는 알고리즘
		[&](const ContElemT& value) { return value % divisor == 0})
		){ }
	else{}

	if (std::all_of(begin(container), end(container),				// C++14 버전
		[&](const auto& value) { return value % divisor == 0})
		) {
	}
	else {}

	// 이 람다가 다른 문맥에서도 유용할 것으로 생각한 어떤 프로그래머가 람다를 복사해서 그 문맥에 붙여넣을 수 있다.
	// 그런데 그 문맥에서 divisor가 클로저보다 먼저 소멸한다면, 참조가 대상을 잃는 문제 발생.
	// 람다의 갈무리 절에 divisor의 수명을 분석해 봐야 한다는 점을 알려주는 힌트가 전혀 없기 때문에...
}

// addDivisorFilter의 문제점 해결 방법 : filters.emplace_back([=](int value) {return value % divisor == 0;});
// 기본 값 갈무리 모드 사용

// 그러나 이 역시 문제가 발생할 수도 있다.
// 포인터를 값으로 갈무리하면 그 포인터는 람다에 의해 생성된 클로저 안으로 복사되는데, 
// 람다 바깥의 어떤 코드가 그 포인터를 delete로 삭제하지 않는다는 보장은 없으며,
// 그런일이 발생하면 포인터 복사본은 지칭 대상을 잃게 된다.

class Widget {
public:
	void addFilter() const;
	void addFilterFix() const;
	void addFilter14() const;
private:
	int divisor;
};
void Widget::addFilter() const {
	FilterContainer filters;
	filters.emplace_back(
		[=](int value) { return value % divisor == 0; }
	);
	// divisor는 지역변수가 아니라 클래스의 한 자료 멤버이므로 갈무리될 수 없다.
	// 그렇지만 만일 기본 갈무리 모드(=)을 제거하면 코드는 컴파일 되지 않는다.
	// divisor를 명시적으로 갈무리하려는 갈무리 절은 컴파일 되지 않는다. [divisor]... 
	// this 포인터로 인해 위 람다는 컴파일 가능
	// 클로저의 유효성이 해당 Widget 객체의 수명에 의해 제한
}

void doSomeWork()
{
	auto pw = std::make_unique<Widget>();
	pw->addFilter();
}
// 여기서 Widget이 파괴된다. 그 때부터 filters는 대상을 잃는 포인터를 가진 상태가 된다.
// 지금 이 문제는 자료멤버의 지역 복사본을 만들어서 그 복사본을 갈무리하면 해결
void Widget::addFilterFix() const
{
	FilterContainer filters;
	auto divisorCopy = divisor;

	filters.emplace_back(
		[divisorCopy](int value)
	{ return value % divisorCopy == 0; }
	);
}
// 사실 이 접근 방식을 사용한다면 기본값 갈무리 모드도 잘 작동.
// 애초에 divisor를 갈무리하려 했는데 사실은 this가 갈무리되게 한 장본인이 기본 갈무리 모드

void Widget::addFilter14() const
{
	FilterContainer filters;

	filters.emplace_back(
		[divisor = divisor](int value)
		{ return value % divisor == 0; }
	);
	// 일반화된 람다 갈무리 (일반화된 람다 갈무리에는 기본 갈무리 모드라는 것이 없음)
}

// 값에 의한 기본 갈무리 모드의 또다른 단점
// 해당 클로저가 자기 완결적이고 클로저 바깥에서 일어나는 자료의 변화로붙터 격리되어 있다는 오해를 부름
// 람다가 지역변수와 매개변수 뿐만 아니라 정적 저장소 수명 기간을 가진 객체에도 의존할 수 있음.
// 전역 범위나 이름 공간 범위에서 정의된 객체와 클래스, 함수, 파일 안에서 static으로 선언된 객체가 그런 객체에 해당
// 그런 객체를 람다 안에서 사용할 수는 있지만, 갈무리 할 수는 없다.
// 기본 값 갈무리 모드의 표기는 마치 그런 객체도 모두 갈무리 된다는 느낌을 준다.

void addDivisorFilter2()
{
	FilterContainer filters;

	static auto calc1 = computeSomeValue1();
	static auto calc2 = computeSomeValue2();

	static auto divisor = computeDivisor(calc1, calc2);

	filters.emplace_back(
		[=](int value)
		{ return value % divisor == 0; }
	);
	// 이 람다는 그 어떤 비정적 지역 변수도 사용하지 않으므로, 아무것도 갈무리하지 않는다.
	// 오히려 이 람다의 코드는 static 변수 divisor를 지칭한다.
	// addDivisorFilter2의 각 호출의 끝에서 divisor가 증가하며, 따라서 이 함수를 통해서 filters에 추가된 람다는
	// 이전과 다른 행동(divisor의 새값에 상응하는)을 보이게 된다.
	// 현실적으로 이 람다는 divisor를 참조로 갈무리한 것과 같으며, 이는 기본 값 갈무리 모드가 뜻하는 바와 직접적으로 모순.
	// 애초에 기본값 갈무리 모드를 사용하지 않는다면 이처럼 오해의 여지가 큰 코드가 만들어질 위험도 사라진다.
}


int main() {}

// 1. 기본 참조 갈무리는 참조가 대상을 잃을 위험이 있다.
// 2. 기본 값 갈무리는 포인터(특히 this)가 대상을 잃을 수 있으며, 람다가 자기 완결적이라는 오해를 부를 수 있다.