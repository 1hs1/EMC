// std::thread들을 모든 경로에서 합류 불가능하게 만들어라

#include <functional>
#include <vector>
#include <thread>

// 모든 std::thread 객체는 합류 가능 상태이거나 합류 불가능 상태이다.
// 합류 가능 : 바탕 실행 스레드 중 현재 실행 중이거나 실행 중 상태로 전이할 수 있는 스레드에 대응
// 예를 들어 차단된 상태이거나 실행 일정을 기다리는 중인 바탕 스레드, 실행이 완료된 바탕 스레드

// 합류 불가능 : 1. 기본 생성된 std::thread : 실행할 함수가 없으므로 바탕 실행 스레드와 대응되지 않는다.
//				 2. 다른 std::thread 객체로 이동된 후의 std::thread 객체 : 이동의 결과로, 원본 std::thread에 대응되던 바탕 스레드는
//																		   대상 std::thread의 바탕 스레드가 된다.
//				 3. join에 의해 합류된 std::thread : join 이후의 std:thread 객체는 실행이 완료된 바탕 실행 스레드에 대응되지 않는다.
//				 4. detach에 의해 탈착된 std::thread : detach는 std::thread 객체와 그에 대응되는 바탕 스레드 사이의 연결을 끊는다.

// std::thread의 합류 가능성이 중요한 이유 하나는, 만일 합류 가능한 스레드의 소멸자가 호출되면 프로그램 실행이 종료

constexpr auto temMillion = 10000000;
bool conditionsAreSatisfied() { return true; }
void performComputation(std::vector<int> v) { return; }

bool doWork(std::function<bool(int)> filter, int maxVal = temMillion)
{
	std::vector<int> goodVals;

	std::thread t([&filter, maxVal, &goodVals]
	{
		for (auto i = 0; i <= maxVal; ++i)
		{
			if (filter(i)) goodVals.push_back(i);
		}
	});

	auto nh = t.native_handle();

	if (conditionsAreSatisfied()) {		// false를 돌려주거나 예외를 던진다면, std::thread 객체 t의 소멸자가 호출, 프로그램 실행 종료
		t.join();
		performComputation(goodVals);
		return true;
	}

	return false;
}

// 암묵적 join : std::thread의 소멸자가 바탕 비동기 실행 스레드의 완료를 기다리게 하는 것이다.
// 예를 들어 conditionsAreSatisfied()가 이미 false를 돌려주었는도 모든 값에 필터가 적용되길 doWork가 기다리는 것은 직관적이지 않다.
// 암묵적 detach : std::thread의 소멸자가 std::thread 객체와 바탕 실행 스레드 사이의 연결을 끊게 하는 것.
// 이 경우 바탕 스레드가 실행을 계속할 수 있다. 디버깅시 더 나쁘다.
// 예를 들어 doWork에서 goodVals는 지역 변수인데, 람다가 참조에 의해 갈무리해서 자신의 본문안에서 수정.
// 람다가 비동기적으로 실행되는 도중에, conditionsAreSatisfied()가 false를 돌려주면, doWork가 반환되며, 지역변수들이 파괴.
// 이제 doWork의 스택 프레임이 뽑혀서 실행의 흐림이 doWork의 호출 지점 다음으로 넘어가겠지만, 해당 스레드는 doWork의 호출 지점에서 계속 실행된다.

// 이러한 두가지 이유로 합류 가능 스레드를 파괴했을 때의 결과가 충분히 절망적이므로 그런 파괴를 아예 금지.

// 따라서 std::thread 객체를 사용할 때 그 객체가 그것이 정의된 범위 바깥의 모든 경로에서 합류 불가능으로 만드는 것은 프로그래머의 책임.
// 한 범위의 바깥으로 나가는 모든 경로에서 어떤 동작이 반드시 수행되어야 할 때 흔히 사용하는 접근 방식은 그 동작을 지역 객체의 소멸자 안에 넣는 것이다.
// 그런 객체를 RAII객체라고 부르고, 그런 객체의 클래스를 RAII 클래스라고 부른다.(STL 컨테이너, 스마트포인터, std::fstream 등)
// std::thread 객체에 대한 표준 RAII 클래스는 없다.

// std::thread를 위한 RAII 클래스 : ThreadRAII 객체가 파괴될 때 바탕 std::thread 객체에 대해 join을 호출할 지 아니면 detach를 호출할지 지정
class ThreadRAII {
public:
	enum class DtorAction { join, detach };

	ThreadRAII(std::thread&& t, DtorAction a) : action(a), t(std::move(t)) {}
	~ThreadRAII()
	{
		if (t.joinable()) {
			if (action == DtorAction::join) {
				t.join();
			}
			else {
				t.detach();
			}
		}
	}

	ThreadRAII(ThreadRAII&&) = default;				// 이동 연산 지원
	ThreadRAII& operator=(ThreadRAII&&) = default;

	std::thread& get() { return t; }

private:
	DtorAction action;
	std::thread t;
};
// 1. 생성자는 std::thread 오른값만 받는다. (std::thread 객체는 복사할 수 없음)
// 2. 생성자의 매개변수들은 호출자가 직관적으로 기억할 수 있는 순서로 선언.
// 멤버 초기화 목록은 자료 멤버들이 선언된 순서를 따른다. 자료 멤버의 초기화가 다른 멤버에 의존할 수 있다.
// std::thread 객체는 초기화되지마자 해당함수를 실행할 수도 있으므로, 클래스에서 std::thread 자료 멤버는 항상 제일 마지막에 선언하는 것이 좋다.
// std::thread를 다른 모든 멤버가 성공적으로 초기화된 후에 초기화 하면, 해당 비동기 스레드가 다른 모든 멤버에 안전하게 접근
// 3. ThreadRAII는 바탕 std::thread 객체에 접근할 수 있는 get 함수를 제공. std::thread 인터페이스 전체를 ThreadRAII에 복제할 필요가 없다.
// 또한, std::thread 객체를 요구하는 문맥에서 ThreadRAII 객체를 사용할 수 있다.
// 4. ThreadRAII 소멸자는 std::thread 객체 t에 대해 멤버 함수를 호출하기 전에 먼저 t가 합류 가능인지부터 점검. 

bool doWorkRAII(std::function<bool(int)> filter, int maxVal = temMillion)
{
	std::vector<int> goodVals;

	ThreadRAII t(
		std::thread ([&filter, maxVal, &goodVals]
	{
		for (auto i = 0; i <= maxVal; ++i)
		{
			if (filter(i)) goodVals.push_back(i);
		}
	}), ThreadRAII::DtorAction::join);

	auto nh = t.get().native_handle();

	if (conditionsAreSatisfied()) {		// false를 돌려주거나 예외를 던진다면, std::thread 객체 t의 소멸자가 호출, 프로그램 실행 종료
		t.get().join();
		performComputation(goodVals);
		return true;
	}

	return false;
}

int main() {}

// 1. 모든 경로에서 std::thread를 합류 불가능으로 만들어라.
// 2. 소멸시 join 방식은 디버깅하기 어려운 성능 이상으로 이어질 수 있다.
// 3. 소멸시 detach  방식은 디버깅하기 어려운 미정의 행동으로 이어질 수 있다.
// 4. 자료 멤버 목록에서 std::thread 객체를 마지막에 선언하라.