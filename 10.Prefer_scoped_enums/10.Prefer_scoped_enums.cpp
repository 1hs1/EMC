//범위 없는 enum보다 범위 있는 enum을 선호하라.

#include <vector>
#include <tuple>


std::vector<std::size_t> primeFactors(std::size_t x)
{
	std::vector<std::size_t> v;
	return v;
};

// template
template<typename E1>
constexpr typename std::underlying_type<E1>::type
toUType_1(E1 enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E1>::type>(enumerator);
}
// underlying_type_t
template<typename E2> // C++14
constexpr std::underlying_type_t<E2>
toUType_2(E2 enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E2>>(enumerator);
}
// auto
template<typename E3> // C++14
constexpr auto toUType_3(E3 enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E3>>(enumerator);
}

int main()
{
	//범위가 없는 enum
	enum Color { green, blue };
	//auto green = false; //error 이미 white가 범위안에 선언됨

	//범위가 있는 enum(enum class라고 부르기도 한다)의 경우 이름 누수가 발생하지 않는다.
	enum class Scoped_Color { black, white, red };
	auto white = false; //ok 범위에 다른 white가 없음
	//Scoped_Color c = white; //error 범위에 다른 white가 있음
	Scoped_Color c = Scoped_Color::white; //ok
	auto c1 = Scoped_Color::white; //ok

  //범위가 없는 enum의 경우 암묵적으로 정수 형식으로 변환 (원치 않는 결과)
	Color c2 = Color::green;
	if (c2 < 14.5) {
	auto factors = primeFactors(c2);
	}
	//범위가 있는 enum의 경우 암묵적으로 다른 형식으로 변환되지 않는다.
	/*
	std::vector<std::size_t> primeFactors(std::size_t x);
	Scoped_Color c = Scoped_Color::red;
	if (c < 14.5) { //error c와 double 비교 불가
	auto factors = primeFactors(c); //error std::size_t를 기대하는 함수에 color 전달 불가
	}
	*/
	//color을 다른 형식으로 변환하고 싶은 경우는 캐스팅을 사용한다
	Scoped_Color c3 = Scoped_Color::red;
	if (static_cast<double>(c3) < 14.5) {
		auto factors = primeFactors(static_cast<size_t>(c3));
	}

	//또 다른 장점은 범위가 있는 enum의 경우 전방 선언이 가능하다는 것이다.
	//범위가 있는 enum의 경우 기본적으로 정수형을 선택한다.
	//enum을 전방선언할 수 없으면 몇가지 단점이 생긴다.
	//예로 컴파일 의존 관계가 늘어나기 때문에 새로운 열거자가 추가될 경우 시스템 전체를 다시 컴파일 해야 한다.

	enum Status { good = 0, failed = 1, incomplete = 100, aduited = 500, indeterminate = 0xFFFFFFFF };

	void continueProcessing(Status s);

	//전방선언
	enum class Status1;
	//전방선언의 경우 Status의 정의가 바뀌어도 재 컴파일 할 필요가 없다.Status가 수정이 되어도, 
	//continueProcessing에 영향을 주지 않는다면 다시 컴파일 할 필요가 없다.

	//범위가 있는 enum 기본 형식 int
	//범위 없는 enum에는 기본 바탕 형식이 없다

	//바탕 형식을 지정하는 방법
	//다른 형식을 명시적으로 지정
	enum class Status2 : std::uint32_t;

	//범위가 없는 enum의 바탕형식 지정
	enum Color2 : std::uint8_t;

	//enum 정의 시에도 가능
	enum class Status3 : std::uint32_t { good = 0, failed = 1 };


	// tuple
	using UserInfo =
		std::tuple < std::string, // name
		std::string, // email
		std::size_t>; // reputation

	UserInfo uInfo;
	auto val_1 = std::get<1>(uInfo); // ?? 1

	enum UserInfoFields { uiName, uiEmail, uiReputation };
	auto val_2 = std::get<uiEmail>(uInfo);

	enum class Scoped_UserInfoFields { uiName, uiEmail, uiReputation };
	auto val_3 = std::get<static_cast<size_t>(Scoped_UserInfoFields::uiEmail)>(uInfo);

	auto val = std::get<toUType_3(Scoped_UserInfoFields::uiEmail)>(uInfo);

	return 0;
}


//- c++98 스타일의 enum을 이제는 범위 없는 enum이라고 부른다.
//- 범위 있는 enum의 열거자들은 그 안에서만 보인다.이 열거자들은 오직 캐스팅을 통해서만 다른 형식으로 변환된다.
//- 범위 있는 enum과 범위없는 enum모두 바탕 형식 지정을 지원한다.범위 있는 enum의 기본 바탕 형식은 int 범위 없는 enum에는 기본 바탕 형식이 없다.
//- 범위 있는 enum은 항상 전방 선언이 가능하다.범위 없는 enum은 해당 선언에 바탕 형식을 지정하는 경우에만 전방 선언이 가능하다.