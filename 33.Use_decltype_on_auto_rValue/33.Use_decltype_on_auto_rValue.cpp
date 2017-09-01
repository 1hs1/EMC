// std::forward를 통해서 전달할 auto&& 매개변수에는 decltype을 사용하라

#include <type_traits>

// C++14에서 가장 고무적인 기능은 일반적 람다
// 매개변수 명세에 auto를 사용하는 람다

int main()
{
	auto f = [](auto x) { return normalize(x); };
	// 이 람다가 산출하는 클로저 클래스의 함수 호출 연산자

	// 수정본
	auto fx = [](auto&& x) { return normalize(std::forward<decltype(x)>(x)); };

	// C++14 람다가 가변인수 지원
	auto f14 = [](auto&&... xs)
	{
		return
			normalize(std::forward<decltype<xs>(xs)...);
	};
}

class temp{	// 컴파일러가 만든 어떤 클래스 이름
public:
	template<typename T>
	auto operator() (T x) const { return normalize(x); }
};
// 이 예제에서 람다는 매개변수 x를 그냥 normalize로 전달(왼값으로만)하기만 한다.
// 만일 normalize가 왼값 오른값 다르게 처리한다면 수정이 필요(main 참조)

//  C++14의 std::forward 구현
template<typename T>
T&& forward(std::remove_reference_t<T>& param)
{
	return static_cast<T&&>(param);
}
// T를 오른값 참조 형식으로 std::forward를 인스턴스화한 결과는 비참조 형식으로 인스턴스화한 결과와 같다.
// 람다의 매개변수 x에 주어진 인수가 오른값일 때 decltype(x)가 오른값 참조형식을 산출
// 왼값이든 오른값이든 decltype(x)를 std::forward로 넘겨주면 우리가 원하는 결과가 나온다.
// 원하는 결과 : std::forward 호출시 전달할 인수가 왼값임을 나타내기 위해서는 왼쪽 참조형식 인수를 사용,
// 오른값임을 나타내기 위해서는 비참조 형식 인수를 사용(항목28)


// std::forward를 통해서 전달한 auto&& 매개변수에는 decltype을 사용하라.