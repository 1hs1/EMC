// 보편 참조에 대한 중복적재 대신 사용할 수 있는 기법 들을 알아두라

#include <string>
#include <set>
#include <type_traits>

// 1. 중복적재를 포기한다
// 항목26의 logAndAdd 예제의 두 중복적재를 logAndAddNAme과 logAndAddNameIdx로 나눈다.
// Person의 생성자 예제에는 통하지 않음.

// 2. const T& 매개변수를 사용한다.
// 보편 참조 매개변수 대신 const에 대한 왼값 참조 매개변수를 사용하는 것이다.
// 효율성을 포기하더라도 에상치 않은 문제를 피하는 것이 더 매력적인 절충안

// 3. 값 전달 방식의 매개변수를 사용한다.
std::string nameFromIdx(int idx) {
	return "";
}

class Person {
public:
	explicit Person(std::string n)
		: name(std::move(n)) {}	// 항목41 참고
	explicit Person(int idx) : name(nameFromIdx(idx)) {}
private:
	std::string name;
};
// Person의 생성자 예제에 적용, 복사될 것이 확실한 객체는 값으로 전달

// 4. 꼬리표 배분을 사용한다.
// logAndAdd가 호출을 다른 두 함수로 위임하게 한다.
// 하나는 정수값을 위한 함수이고, 다른 하나는 그 외의 모든 것을 위한 함수이다.
// logAndAdd는 모든 인수 형식을 받는다.
// 실제 작업을 수행하는 두 함수의 이름은 logAndAddImpl, 이들에 대해서는 중복적제 사용
// 애초에 전달된 인수가 정수 형식인지 아닌지를 뜻하는 또 다른 매개변수를 받아 원하는 방식으로 제어.
// std::true_type, std::false_type 꼬리표(실행시점에서 쓰이지 않음), 템플릿 메타프로그래밍
std::multiset<std::string> names;
template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
	names.emplace(std::forward<T>(name));
}

template<typename T>
void logAndAddImpl(int idx, std::true_type)
{
	logAndAdd(nameFromIdx(idx));
}

template<typename T>
void logAndAdd(T&& name)
{
	logAndAddImpl(std::forward<T>(name), std::is_integral<T>());	// 문제발생 : 인수가 왼값이면, T는 왼값참조형식으로 연역(예:int&)
																	// 인수가 실제로 정수값을 나타내어도 std::is_integral은 거짓이된다.
	logAndAddImpl(													
		std::forward<T>(name),
		std::is_integral<typename std::remove_reference<T>::type>()	// 수정 완료.
	);
}

// 5. 보편 참조를 받는 템플릿을 제한한다.
// 보편참조를 받는 중복적재 버전이 단일한 배분 함수로 적용할 정도로 탐욕스럽지(?) 않다면 꼬리표 배분 설계가 적합하지 않다.
// 보편 참조 매개변수를 포함하는 함수 템플릿이 중복적재 해소의 후보가 되는 조건들을 적절히 제한할 수 있는 std::enable_if가 필요하다.
class PersonE {
public:
	template<typename T,
		typename = typename std::enable_if<
		!std::is_same<Person, 
					typename std::decay<T>::type	// T에서 모든 참조 한정사와 const, volatile을 제거하는 수단이 필요. std::decay
					>::value
			>::type
		>
		explicit PersonE(T&& n);
};
// Person을 다른 어떤 Person으로 생성하는 경우, 그것이 왼값이든 오른값이든, const이든 비const이든, volatile이든 비 volatile이든,
// 보편참조를 받는 생성자는 절대 호출되지 않는다.

// 인수 형식이 단지 Person이 아니면 템플릿화된 생성자를 활성화하는 것이 아니라, Person도 아니고
// Person에서 파생된 형식도 아닐 때에만 활성화해야 한다. is_same -> is_base_of로 변경

// C++14 버전
class Person14 {
public:
	template<
		typename T,
		typename = typename std::enable_if_t<
		!std::is_base_of<Person,
		std::decay_t<T>
		>::value
		>
	>
		explicit Person14(T&& n);
};

// 정수 인수들을 처리하는 Person 생성자 중복적재를 추가하고, 
// 그런 인수들에 대해서는 템플릿화된 생성자가 비활성화되도록 하는 조건을 추가

class PersonF {
public:
	template<
		typename T,
		typename = std::enable_if_t<
			!std::is_base_of<Person, std::decay_t<T>>::value
			&&
			!std::is_integral<std::remove_reference_t<T>>::value
			>
		>
		explicit PersonF(T&& n)
		: name(std::forward<T>(n))
	{
		// 아래 참조
		static_assert(
			std::is_constructible<std::string, T>::value,
			"Parameter n can't be used to construct a std::string"
			);
	}

	explicit PersonF(int idx)
		: name(nameFromIdx(idx)) {}

private:
	std::string name;
};

// 절충안
// 중복적재를 포기하는 것과 const T& 전달하는 것, 값을 전달하는 것
// 호출되는 함수들의 각 매개변수에 대해 형식을 지정
// 꼬리표 배분과 템플릿 활성화 제한은 완벽 전달을 사용. 매개변수들의 형식을 지정하지 않는다.
// 완벽전달이 더 효율적이라는 점은 하나의 규칙이다.
// 단지 선언된 매개변수 형식을 만족하기 위해 임시 객체를 생성하는 비효율성이 없기 때문이다.
// 완벽전달 단점 : 완벽 전달이 불가능한 인수들이 있다.
// 구체적인 형식을 받는 함수에는 전달할 수 있어도 완벽 전달은 불가능할 수 있다.(항목30)
// 또다른 단점 : 유효하지 않은 인수를 전달했을 때 나오는 오류 메시지가 난해하다.

// Person의 경우 전달 함수의 보편 참조 매개변수가 std::string에 대한 초기치로 쓰일 것을 알고 있으므로,
// 그런 초기치로 사용하는 것이 가능한지를 미리 ststic_assert를 이용해서 점검
// std::is_constructible이라는 형식특질은 한 형식의 개체를 다른 한 형식(또는 형식들의 집합)의 객체(또는 객체들의 집합)
// 로부터 생성할 수 있는지를 컴파일 시점에서 판정(위의 예제 참조)

int main(){}

// 1. 보편참조와 중복적재의 조합에 대한 대안으로는 구별되는 함수 이름 사용, 매개 변수를 const에 대한 왼값 참조로 전달,
// 매개변수를 값으로 전달, 꼬리표 배분 사용들이 있다.
// 2. std::enable_if를 이용해서 템플릿의 인스턴스화를 제한함으로써 보편 참조와 중복적재를 함께 사용할 수 있다.
// std::enable_if는 컴파일러가 보편 참조 중복적재를 사용하는 조건을 프로그래머가 직접 제어하는 용도로 쓰인다.
// 3. 보편 참조 매개변수는 효율성 면에서 장점인 경우가 많지만, 대체로 사용성 면에서는 단점이 된다.