// 오른값 참조에는 std::move를, 보편참조에는 std::forward를 사용하라.

// 오른값 참조는 이동할 수 있는 객체에만 묶인다. 어떤 매개변수가 오른값 참조라면,
// 그 참조에 묶인 객체를 이동할 수 있음이 확실하다.

#include <string>
#include <memory>

class SomeDataStructure{};
class Widget {
public:
	Widget() {}
	Widget(Widget&& rhs)	//  rhs는 이동이 가능한 객체를 참조함이 확실
		: name(std::move(rhs.name)),
		p(std::move(rhs.p)){}
private:
	std::string name;
	std::shared_ptr<SomeDataStructure> p;
};

// 보편 참조는 이동에 적합한 객체에 묶일 수도 있고 아닐 수도 있다.
// 보편 참조는 오른값으로 초기화되는 경우에만 오른값으로 캐스팅되어야 한다.
class WidgetF {
public:
	template<typename T>
	void setName1(T&& newName)
	{
		name = std::forward<T>(newName);	// newName은 보편참조
	}

	// 아래 보고 오기
	template<typename T>
	void setName2(T&& newName)
	{
		name = std::move(newName);
	}

	// 아래 보고 오기2
	void setName3(const std::string& newName)
	{
		name = newName;
	}
	void setName3(std::string&& newName)
	{
		name = std::move(newName);
	}
	// 작성하고 유지보수해야할 소스코드의 양이 늘어났다. 템플릿 함수 하나가 비템플릿 함수 두개로 변했다.)
	// 효율성이 떨어진다.
	// w.setName("Adela Novak");
	// 보편 참조를 받는 버전의 setName1에서는 문자열 리터럴이 그대로 setName으로 넘어가서 w안의 std::string 자료 멤버에 대한 배정 연산자의 인수로 쓰인다.
	// 그 문자열 리터럴이 w의 자료멤버 name에 직접 배정하며, 임시 std::string객체는 생성되지 않는다.
	// 중복적재된 setName3 에서는 문자열 리터럴로부터 임시 std::string 객체가 생성되어서 setName3의 매개변수에 묶이고, 그 임시 std::string이 w의 자료멤버로 이동한다.
	// setName3 한번 호출했을 때, std::string 생성자 한번(임시 객체 생성), std::string 이동 배정 연산자 한번(newName을 w.name으로 이동), std::string 소멸자 한번(임시객체 파괴)
	// 실행 비용이 const char* 포인터를 받는 std::string 배정 연산자의 실행 한번보다 크다.
	// 설계의 규모가변성이 나쁘다.매개변수를 하나만 받으므로 중복적재가 두개, 매개변수가 더 많고 각 매개변수가 왼값일 수도 있고, 오른값일 수도 있다면
	// 중복적재의 수가 기하급수적으로 증가.

private:
	std::string name;
};

// 오른값 참조로 다른 함수로 전달할 때에는 오른값으로의 무조건 캐스팅을 적용해야 한다.(std::move를 통해서). 참조는 항상 오른값에 묶임
// 보편 참조를 다른 함수로 전달할 때에는 오른값으로의 조건부 캐스팅을 적용해야 한다.(std::forward를 통해서). 특정 조건하에서 오른값에 묶임.
// 오른값 참조에 std::forward를 사용해도 원하는 행동이 일어나게 하는것이 가능.
// 하지만 소스코드가 장황하고 실수의 여지가 있으며 관용구에서 벗어난 모습
// 따라서 오른값 참조에 std::forward를 사용하는 것은 피한다.
// 보편참조에 std::move를 사용하는 것은 더 나쁘다. 왼값이 의도치 않게 수정되는 결과가 빚어질 수 있다.	(위에 참조)

std::string getWidgetName() { return "Adela Novak"; }
int main()
{
	WidgetF f;
	auto n = getWidgetName();	// n은 지역변수
	f.setName2(n);				// n을 f로 이동한다. 이제 n의 값은 알 수 없다.
								// setName은 자신의 매개변수를 수정하지 말아야 하므로 const를 명시하는 것이 바람직, 보편참조는 const일 수 없다.
								// const 왼값 버전과 오른값 버전 두가지로 중복적재. (위에 참조2)
}

// std::make_shared와 std::make_unique 같은 함수의 내부에서 보편참조 매개변수를 다른함수에 전달할 때에는 std::forward를 적용.
// 오른값 참조나 보편 참조에 묶인 객체를 한 함수 안에서 여러 번 사용하기도 한다.
// 그런 경우, 그 객체를 다 사용하기 전에 다른 객체로 이동하는 일은 피해야한다.
// 즉, std::move(오른값 참조의 경우)나 std::forward(보편참조의 경우)를 적용하는 것은 해당 참조의 마지막 사용이어야 한다.

// 함수가 결과를 값으로 돌려준다면, 그리고 그것이 오른값 참조나 보편 참조에 묶인 객체라면, 해당 참조를 돌려주는 return문에서 
// std::move나 std::forward를 사용하는 것이 바람직하다.
class Matrix{};
Matrix operator+(Matrix&& lhs, const Matrix& rhs)	// 결과를 값으로 반환
{
	lhs = rhs;
	return std::move(lhs);	// lhs를 반환값으로 이동한다. std::move 호출이 없다면, 복사한다.
}
// 결과를 값으로 돌려주는 함수의 반환값에 오른값 참조로의 std::move를 적용해서 잃을 것은 없으며, 오히려 얻을 것이 많을 수 있다.
Widget makeWidgeto()
{
	Widget w;
	return w;	// 복사버젼
}
Widget makeWidgetm()
{
	Widget w;
	return std::move(w);	// 이동버젼
}
// 복사버전에서 만일 지역변수 w를 함수의 반환값을 위해 마련한 메모리 안에 생성한다면 w의 복사를 피할 수 있다는 점은 이미 오래 전부터 알려져있다.
// 이러한 복사제거는 소프트웨어의 관찰 가능한 행동에 영향을 주지 않는 경우에만 허용
// 컴파일러가 결과를 값 전달 방식으로 반환하는 함수의 어떤 지역 객체의 복사(또는 이동)을 제거 할 수 있으려면 그 지역 객체의 형식이 함수의 반환 형식과 같아야 하고,
// 그 지역 객체가 바로 함수의 반환값이어야 한다.
// 실질적으로 복사버전에서 복사는 하지 않는다.
// 이동버전의 경우 지역객체에 대한 참조를 돌려주는 것은 반환값 최적화의 필수조건을 만족하지 않는다.
// 반환값 최적화가 허용되는 경우 복사 제거가 일어나거나, 아니면 반환되는 객체에 암묵적으로 std::move가 적용되어야 한다.
// 이 경우는 값 전달 방식의 함수 매개변수의 경우와 비슷. 그 매개변수는 함수의 반환값 최적화를 위한 복사 제거의 대상이 아니지만, 함수가 그런 매개변수를 반환하는 경우
// 컴파일러는 그것을 반드시 오른값으로 취급해야 한다.
Widget makeWidgetp(Widget w)
{
	return w;	// 컴파일러는 오른값으로 취급(std::move(w))
}
// 지역 객체에 std::move를 적용하는 것이 합당한 상황도 있지만, 반환값 최적화가 적용될 수 있는 return 문이나 반환값 최적화의 대상이 아닌 값 전달 방식의 매개변수를 돌려주는 return 문에서
// std::move를 적용하는 것은 합당한 상황이 아니다.

// 1. 오른값 참조나 보편 참조가 마지막으로 쓰이는 지점에서, 오른값 참조에는 std::move를, 보편 참조에는 std::forward를 적용하라.
// 2. 결과를 값 전달 방식으로 돌려주는 함수가 오른값 참조나 보편 참조를 돌려줄 때에도 각각 std::move나 std::forward를 적용하라.
// 3. 반환값 최적화의 대상이 될 수 있는 지역 객체에는 절대로 std::move나 std::forward를 적용하지 말아야 한다.