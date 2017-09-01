// 소유권 공유 자원의 관리에는 std::shared_ptr를 사용하라
// 객체를 가리키던 마지막 std::shared_ptr가 객체를 더 이상 가리키지 않게 되면,
// 그 std::shared_ptr는 자신이 가리키는 객체를 파괴한다.
// 객체의 수명에 대해 신경 쓸 필요가 없고, 객체의 파괴 시점은 결정론적이다.

// 참조 횟수는 관리되는 자원에 연관된 값, 그 자원을 가리키는 std::shared_ptr들의 개수
// std::shared_ptr의 생성자는 이 참조횟수를 증가, 소멸자는 감소, 복사 배정 연산자는 증가와 감소를 모두 수행.
// 어떤 std::shared_ptr가 자원의 참조횟수를 감소한 후 그 횟수가 0이 되었을 때,
// 그 자원을 가리키는 std::shared_ptr가 더 이상 필요없다는 뜻. std::shared_ptr는 자원을 파괴한다.

// 1. std::shared_ptr의 크기는 생 포인터의 두 배이다.
// 2. 참조횟수를 담을 메모리를 반드시 동적으로 할당해야 한다.
// std::make_shared를 이용해서 std::shared_ptr를 생성하면 동적 할당의 비용을 피할 수 있다. (항목21)
// 3. 참조횟수의 증가와 감소가 반드시 원자적 연산이어야 한다.

// std::shared_ptr의 생성자가 피지칭 객체의 참조횟수를 증가하지 않는 경우
// 이동 생성일 경우.

// std::unique_ptr처럼 std::shared_ptr는 delete를 기본적인 자원 파괴 메커니즘으로 사용.
// 커스텀 삭제자를 지원하는데, 구체적인 방식은 std::unique_ptr과 다르다.
#include <memory>
#include <vector>
class Widget : public std::enable_shared_from_this<Widget>
{
public:
	template<typename... Ts>
	static std::shared_ptr<Widget> crate(Ts&&... params);

	void process()
	{
		processedWidgets.emplace_back(this);	// 잘못된 방식
		processedWidgets.emplace_back(shared_from_this());
	}

	std::vector<std::shared_ptr<Widget>> processedWidgets;

private:
	// 생성자들
};
auto loggingDel = [](Widget* pw) {
	delete pw;
};
std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel);
std::shared_ptr<Widget> spw(new Widget, loggingDel);

// std::shared_prt의 설계가 더 유연하다.
// 사용하는 커스텀 삭제자의 형식이 서로 다른 두 std::shared_ptr<<Widget>의 경우
auto customDeleter1 = [](Widget* pw) {};
auto customDeleter2 = [](Widget* pw) {};
std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
std::shared_ptr<Widget> pw2(new Widget, customDeleter2);
std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };
// 하나를 다른 하나에 배정할 수도 있고, 둘 다 std::shared<Widget>형식의 매개변수를 받는 함수에 넘겨줄 수 있다.
// std::unique_ptr에서는 불가능. 형식에 커스텀 삭제자의 형식이 영향을 미치기 때문에

// std::unique_ptr과 또 다른 차이점 : 커스텀 삭제자를 지정해도 std::shared_ptr 객체의 크기가 변하지 않는다.
// std::shared_ptr<T>객체의 메모리 : T 지칭 포인터 -> T 객체, 제어블록지칭 포인터 -> 참조횟수, 약한 횟수, 기타 자료(커스텀 삭제자, 할당자 등)

// 객체의 제어블록은 그 객체를 가리키는 최초의 std::shared_ptr가 생성될때 설정된다.
// 생성하는 코드에서 그 객체를 가리키는 다른 std::shared_ptr가 이미 존재하는지를 알아내는 것은 불가능,
// 제어블록의 생성여부에 관해 다음과 같은 규칙 유추 가능
// 1. std::make_shared(항목21)는 항상 제어블록을 생성한다.
// 2. 고유 소유권 포인터(즉 std::unique_ptr나 std::auto_ptr)로부터 std::shared_ptr객체를 생성하면 제어블록이 생성된다.
// 3. 생 포인터로 std::shared_ptr 생성자를 호출하면 제어블록이 생성된다.

// 하나의 생 포인터로 여러개의 std::shared_ptr를 생성하면 피지정 객체에 여러 개의 제어 블록이 만들어지므로,
// 의문의 여지 없이 미저으이 행동이 된다.
auto pw = new Widget;
std::shared_ptr<Widget> spw1(pw, loggingDel);
std::shared_ptr<Widget> spw2(pw, loggingDel);

// std::shared_ptr 생성자에 생 포인터를 넘겨주는 일은 피하라.
// 대안: std::make_shared(항목21)를 사용하라, 단, 이 때 커스텀 삭제자를 지정 못함.
// std::shared_ptr 생성자를 생 포인터로 호출할 수 밖에 없는 상황에서는 
// 생 포인터 변수를 거치지 말고, new의 결과를 직접 전달하라.
std::shared_ptr<Widget> fix_spw1(new Widget, loggingDel);
std::shared_ptr<Widget> fix_spw2(fix_spw1);

// this포인터로 인한 잘못된 예
// 위의 widget클래스 참고 process함수
// std::shared_ptr들의 컨테이너에 this를 넣으면, std::shared_ptr객체가 생성되면서,
// 피지칭 Widget객체(*this)에 대한 새 제어블록이 만들어진다.
// 멤버함수 바깥에 이미 그 Widget을 가리키는 다른 std::shared_ptr들이 있다면, 필연 미정의 행동이 발생
// std::shared_ptr로 관리하는 클래스를 작성할 때, 그 클래스의 this포인터로부터 std::shared_ptr를 안전하게 생성하려면
// std::enable_shared_from_this를 그 클래스의 기반클래스로 삼으면 된다. 그리고 shared_from_this() 사용

// shared::from_this는 현재 객체에 대한 제어블록을 조회하고, 그 제어블록을 지칭하는 새 std::shared_ptr를 생성
// 현재 객체에 이미 제어블록이 연관되어 있다고 가정. 만약 없다면, 함수의 행동은 정의되지 않는다.
// std::shared_ptr가 유효한 객체를 가리키기도 전에 shared_from_this를 호출하는 일을 방지하기 위해,
// std::enble_shared_from_this를 상속받은 클래스는 자신의 생성자들을 private로 선언.
// 외부에서 객체를 생성할 수 있도록 std::shared_ptr를 돌려주는 팩터리 함수를 제공

// std::unique_ptr에서 std::shared_ptr로 업그레이드는 쉽다.
// std::shared_ptr에서 std::unique_ptr로 돌아갈 수는 없다.

// std::shared_ptr가 할 수 없는 일로는 배열관리가 있다.
// std::shared_ptr의 API가 단일 객체를 가리키는 포인터만 염두에 두고 설계.
// std::shared_ptr는 operator[]를 제공하지 않으므로 색인으로 배열원소에 접근하려면 포인터 산술에 기초한 어색한 표현식을 동원
// std::shared_ptr가 지원하는 파생클래스 포인터에서 기반 클래스포인터로의 변환은 단일객체에는 합당, 
// 배열에 적용하면, 형식시스템에 구멍이 생긴다.
// C++11은 내장 배열에 대한 다양한 대안들 std::array, std::vector, std::string를 제공

int main(){}

//1. std::shared_ptr는 임의의 공유자원의 수명을 편리하게 (쓰레기 수거에 맡길때 만큼이나) 관리할 수 있는 수단을 제공
//2. 대체로 std::shared_ptr 객체는 그 크기가 std::unique_ptr객체의 두 배이며, 제어 블록에 관련된 추가 부담을 유발,
// 원자적 참조횟수 조작을 요구한다.
//3. 자원은 기본적으로 delete를 통해 파괴되나, 커스텀 삭제자도 지원한다.
// 삭제자의 형식은 std::shared_ptr의 형식에 아무런 영향도 미치지 않는다.
//4. 생 포인터 형식의 변수로부터 std::shared_ptr를 생성하는 일은 피해야 한다.