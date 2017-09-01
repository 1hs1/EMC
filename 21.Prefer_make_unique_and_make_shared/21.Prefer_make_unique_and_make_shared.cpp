// new를 직접 사용하는 것보다 std::make_unique와 make_shared를 선호하라.
// make_shared : C++11
// make_unique : C++14

#include <memory>
#include <vector>

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

// std::make_unique와 std::make_shared는 임의의 개수와 형식의 인수들을 받아서 그것들을 생성자로
// 완벽 전달해서 객체를 동적으로 생성하고, 그 객체를 가리키는 똑똑한 포인터를 돌려주는 세가지
// make함수 중 둘이다.
// 나머지 하나는 std::allocate_shared이다. 첫 인수가 동적 메모리 할당에 쓰일 할당자 객체인 점이 std::make_shared와 다르다.
class Widget{};
auto upw1(std::make_unique < Widget>());
std::unique_ptr<Widget> upw2(new Widget);
auto spw1(std::make_shared<Widget>());
std::shared_ptr<Widget> spw2(new Widget);

void processWidget(std::shared_ptr<Widget> spw, int priority){}
int computePriority() { return 0; }
// 선호 이유
// 1. 코드 중복이 없고, 타자량이 준다.
// 2. 에외 안정성과 관련
// 3. 효율성

void cusDel(Widget *ptr){}
int main() 
{
	processWidget(std::make_shared<Widget>(), computePriority());
	processWidget(std::shared_ptr<Widget>(new Widget), computePriority());
	// 1. 표현식 "new Widget"이 평가된다. 즉, Widget이 힙에 생성된다.(인수부터 먼저 평가)
	// 2. new가 산출한 포인터를 관리하는 std::shared_ptr<Widget>의 생성자가 실행
	// 3. computePriority가 실행(2번과 순서가 바뀔수도 있다.)
	// 만약 computePriority가 먼저 실행되고, 예외가 발생되었다면, 1번에서 생성된 객체는 샌다.

	// 보완책(아래 설명 보고 올것)
	std::shared_ptr<Widget> spw(new Widget, cusDel);
	processWidget(spw, computePriority());	// 복사생성이 일어남. 참조횟수를 원자적으로 증가
	processWidget(std::move(spw), computePriority());	// 이동생성이 일어남. 참조횟수 조작 필요없음.
}

// new를 직접 사용해서 std::shared_ptr를 생성하면 Widget 객체를 위한 메모리 할당과 제어블록을 위한
// 또 다른 메모리 할당이 일어난다.
// std::make_shared를 사용하면 메모리 조각을 한번에 할당

// 단점(make_unique, make_shared)
// 1. make 함수들 중에는 커스텀 삭제자를 지정할 수 있는 것이 없다.
// 2. make 함수들이 내부적으로 매개변수들을 완변 전달할 때 중괄호가 아닌 괄호를 사용함. 중괄호 초기치로 생성하려면 반드시 new를 직접 사용해야함.
auto upv = std::make_unique<std::vector<int>>(10, 20);
auto spv = std::make_shared<std::vector<int>>(10, 20);
// 20인 요소 열개짜리 std::vector를 생성
// 항목 30 에 우회책이 나옴.
auto initList = { 10, 20 };
auto spve = std::make_shared<std::vector<int>>(initList);

// make_shared만
// 3. 클래스 고유 operator new와 operate delete가 있는 형식의 객체를 make함수로 생성하는 것은 대체로 바람직하지 않다.
// 예를 들어 Widget클래스를 위한 operator new와 operator delete라면 크기가 정확히 sizeof(Widget)인 메모리 조각들의
// 할당과 해제를 처리하는 데 특화된 경우가 많다. 그러나 std::allocate_shared가 요구하는 메모리 조각의 크기는
// 동적으로 할당되는 객체의 크기가 아니라 그 크기에 제어블록의 크기를 더한 것.
// 4. new의 직접 사용에 비한 std::make_shared의 크기 및 속도상의 장점은 std::shared_ptr의 제어 블록이 관리 대상 객체와 동일한
// 메모리 조각에 놓인다는 점에서 비롯된다.
// 그 객체의 참조 횟수가 0이 되면 객체가 파괴된다. 그러나 객체가 차지하고 있던 메모리는 해당 제어블록이 파괴되기 전까지는
// 해제될 수 없다. 같은 메모리 조각에 들어있기 때문에...
// 제어블록을 참조하는 std::weak_ptr들이 존재하는 한 제어블록은 게속해서 존재해야 한다.
// 따라서 std::shared_ptr용 make함수가 할당한 메모리 조각은 그것을 참조하는 마지막 std::shared_ptr와 마지막 std::weak_ptr
// 둘다 파괴된 후에만 해제.
// 객체 형식이 상당히 크고 마지막 std::shared_ptr의 파괴와 마지막 std::weak_ptr의 파괴 사이의 시간 간격이 꽤 길다면, 객체가
// 파괴된 시점과 객체가 점유하던 메모리가 해제되는 시점 사이에 시간 지연이 생길 수 있다.
class ReallyBigType{};
auto pBigObj = std::make_shared<ReallyBigType>();	// 아주 큰 객체를 std::make_shared를 이용해서 생성
// 큰 객체를 가리키는 std::shared_ptr들과 std::weak_ptr들을 생성해서 사용
// 여기서 객체를 가리키는 마지막 std::shared_ptr가 파괴되나, std::weak_ptr들은 여전히 남아있음
// 이 부분에서 큰 객체가 차지하던 메모리는 여전히 할당
// 여기서 객체를 가리키는 마지막 std::weak_ptr가 파괴된다. 이제 제어블록과 객체가 차지하던 메모리가 해제

// new를 직접 사용하는 경우에는 ReallyBigType 객체를 가리키던 마지막 std::shared_ptr가 파괴되는 즉시 그 객체의 메모리가 해제
std::shared_ptr<ReallyBigType> pBigObjnew(new ReallyBigType); // 아주 큰 객체를 new를 이용해서 생성
// 객체를 가리키는 std::shared_ptr들과 std::weak_ptr 들을 생성해서 사용
// 객체를 가리키는 마지막 std:shared_ptr가 파괴되나, std::weak_ptr들은 여전히 남아있다. 객체의 메모리 해제
// 제어블록을 위한 메모리만 할당
// 객체를 가리키는 마지막 std::weak_ptr가 파괴된다. 제어블록이 차지하던 메모리 해제

// new를 사용하면서 예외 안정성 문제를 겪지 않는 최선의 방책은 new의 결과를 다른 일은 전혀 하지 않는 문장에서
//스마트 포인터의 생성자에 즉시 넘겨주는 것.(위의 에제 수정)

// 1. new의 직접 사용에 비해, make 함수를 사용하면 소스코드 중복의 여지가 없어지고, 예외 안정성이 향상되고, std::make_shared와 
// std::allocate_shared의 경우 더 작고 빠른 코드가 산출된다.
// 2. make함수의 사용이 불가능 또는 부적합한 경우로는 커스텀 삭제자를 지정해야 하는 경우와 중괄호 초기치를 전달해야 하는 경우가 있다.
// 3. std::shared_ptr에 대해서는 make 함수가 부적합한 경우가 더 있는데, 두가지 에를 들자면 (1) 커스텀 메모리 관리 기능을 가진 클래스를
// 다루는 경우와 (2) 메모리가 넉넉하지 않은 시스템에서 큰 객체를 자주 다루어야 하고 std::weak_ptr들이 해당 std::shared_ptr들보다
// 더 오래 살아남는 경우이다.
