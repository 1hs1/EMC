// std::move와 std::forward를 숙지하라.

// std::move가 모든 것을 이동하지는 않는다.
// std::forward가 모든 것을 전달하지는 않는다.
// 실행시점에서 둘 다 아무것도 하지 않는다.
// std::move와 std::forward는 그냥 캐스팅을 수행하는 함수이다.

// std::move는 주어진 인수를 무조건 오른값으로 캐스팅
// std::forward는 특정조건이 만족될 때에만 오른값 캐스팅(조건부 캐스팅)
#include <type_traits>
#include <string>

template<typename T>
typename std::remove_reference<T>::type&& move(T&& param)
{
	using ReturnType = typename remove_reference<T>::type&&;

	return static_cast<ReturnType>(param);
}
// 객체에 대한 참조(정확히 말하면 보편 참조: 항목24)를 받아서 같은 객체에 대한 어떤 참조를 돌려준다.
// 일반적으로 오른값참조를 돌려주나, 형식 T가 하필 왼값 참조이면 T&&는 왼값 참조가 된다.(항목 28)
// 이를 방지하기 위해, 이 구현은 T에 형식 특질 std::remove_reference를 적용
// 반환 형식의 &&는 항상 참조가 아닌 형식에 적용된다. 결과적으로 std::move는 반드시 오른값 참조를 돌려준다.
// std::move는 자신의 인수를 오른값으로 캐스팅한다.

// C++14 버전
template<typename T>
decltype(auto) move(T&& param)
{
	using ReturnType = remove_reference_t<T>&&;
	return static_cast<ReturnType>(param);
}

// std::move는 캐스팅을 수행하지만, 이동은 수행하지 않는다.
// std::move라는 이름은 이동할 수 있는 객체를 좀 더 쉽게 지정하기 위한 함수

// 오른값이 이동의 후보가 아닌 경우
class Annotation {
public:
	explicit Annotation(const std::string text)
		: value(std::move(text)) {}	// 복사할 매개변수 값 전달로 선언(항목41) -> 복사비용을 치르지 않기위해 std::move 적용

private:
	std::string value;
};
// Annotation 생성자의 멤버 초기화 목록에서 std::move(text)의 결과는 const std::string형식의 오른값이다.
// 그 오른값은 std::string의 이동생성자에 전달 할 수 없다.
// 왜냐하면 그 이동생성자는 const가 아닌 std::string에 대한 오른값참조를 받기 때문이다. 그러나 그 오른값을 복사 생성자에 전달 할 수는 있다.
// const에 대한 왼값 참조를 const 오른값에 묶는 것이 허용되기 때문이다.

// 1. 이동을 지원할 객체는 const로 선언하지 말아야 한다. const 객체에 대한 이동 요청은 소리 없이 복사 연산으로 변환
// 2. std::move는 아무것도 실제로 이동하지 않을 뿐만 아니라, 캐스팅되는 객체가 이동 자격을 갖추게  된다는 보장도 제공 안함.
// 확실한 것은 객체에 std::move를 적용한 결과가 하나의 오른값이라는 것뿐.
#include <chrono>
void makeLogEntry(const std::string& msg, const std::chrono::system_clock::time_point& time)	{}

class Widget{};
void process(const Widget& lvalArg) {}
void process(Widget&& rvalArg) {}
template<typename T>
void logAndProcess(T&& param)
{
	auto now = std::chrono::system_clock::now();
	makeLogEntry("Calling 'process'", now);
	process(std::forward<T>(param));
	// std::forwrad는 주어진 인수가 오른값으로 초기화 된 것일 때에만 그것을 오른값으로 캐스팅한다는 점에서 조건부 캐스팅
	// 어떤값으로 초기화 되어있는지의 정보는 logAndProcess의 템플릿 매개변수 T에 부호화(encoding)되어 있다.
	// 그 매개변수는 std::forward로 전달되며, std::forward는 거기서 해당정보를 복원한다.(항목28)
}

int main()
{
	Widget w;
	logAndProcess(w);
	logAndProcess(std::move(w));
}

// std::move의 매력은 사용하기 편하고, 오류의 여지가 줄어들고, 코드의 명확성이 높아진다.
// 어떤 클래스의 이동 생성자가 호출된 횟수를 추적하고 싶다면, 그냥 이동 생성 도중에 클래스 정적(static) 카운터 변수를 증가
// 클래스의 비정적 자료 멤버가 std::string 하나뿐이라 할 때, 이동생성자 구현 방식
class WidgetMove {
public:
	WidgetMove(WidgetMove&& rhs) : s(std::move(rhs.s)) { ++moveCtorCalls; }

private:
	static std::size_t moveCtorCalls;
	std::string s;
};

class WidgetForward {
public:
	WidgetForward(WidgetForward&& rhs) : s(std::forward<std::string>(rhs.s)) { ++moveCtorCalls; }

private:
	static std::size_t moveCtorCalls;
	std::string s;
};

// 첫 버전의 std::move에서는 함수 인수(rhs.s)만 지정하면 되었지만, 둘째 버전의 std::forward에서는
// 함수 인수(rhs.s)와 템플릿 형식 인수(std::string) 둘 다 지정해야 했음을 주목
// 그리고 std::forward에 전달하는 형식이 반드시 참조가 아니어야 한다는 점도 주목(항목28)
// std::move쪽이 std::forward보다 타자량이 적고, 전달하는 것이 오른값이라는 정보를 부호화하는 형식인수를 지정하는 번거로움도 없다.
// 또한 잘못된 형식을 지정하는 실수를 저지를 여지도 없다. (std::string&를 지정하면 자료 멤버s가 이동생성이 아니라 복사생성된다.)

// std::move를 사용한다는 것은 주어진 인수를 무조건 오른값으로 캐스팅하겠다는 뜻. 하나의 이동을 준비
// std::forward는 오른값에 묶인 참조만 오른값으로 캐스팅하겠다는 뜻. 그 객체를 원래의 왼값 또는 오른값 성질을 유지한 채로 다른 함수에 그냥 넘겨주는 전달

// 1. std::move는 오른값으로의 무조건 캐스팅을 수행한다. std::move 자체는 아무것도 이동하지 않는다.
// 2. std::forward는 주어진 인수가 오른값에 묶인 경우에만 그것을 오른값으로 캐스팅한다.
// 3. std::move와 std::forward 둘 다, 실행시점에서는 아무 일도 하지 않는다.