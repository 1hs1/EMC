#pragma once

// 비동기성이 필수일 때에는 std::launch::async를 지정하라

// std::launch::async 시동 방침

// - std::launch::async : 비동기적, 다른 스레드에서 실행

// - std::launch::deferred :  std::async가 돌려준 미래객체(std::future)에 대해 get, wait가 호출될 때에만 실행,
//	 위와 같은 호출이 일어날 때까지 지연된다.
  
// 기본 시동 방침
auto fut1 = std::async(f);

// 비동기 또는 지연 실행
auto fut2 = std::async(std::launch::async |
	std::launch::deferred,
	f);
// 유연성 작용 : 스레드 생성과 파괴, 과다구독 회피, 부하 균형화 
// - f가 지연 실행될 수도 있으므로, f가 t와 동시에 실행될지 예측불가
// - f가 fut에 대해 get, wait를 호출하는 스레드와는 다른 스레드에서 실행될지 예측불가
// - 프로그램의 모든 가능경로에서 fut에 대한 get, wait호출 보장 없다,
//   f가 반드시 실행될 것인지 예측하는 것이 불가

// 기본 시동 방침 스케줄링 유연성 thread_local 변수들의 사용과 잘 맞지 않는 경우도 많다.
// f에 그런 스레드 지역 저장소(tread-local storage)를 읽거나 쓰는 코드가 어떤 스레드의 지역변수에 접근하는지 예측 불가

// 유연성은 wait 기반 루프에 영향
// wait_for, wait_until 호출시 std::future_status::deferred 값 반환
// 무한루프 가능성
using namespace std::literals;
void f()
{
	std::this_thread::sleep_for(1s);
}
auto fut = std::async(f);
while (fut.wait_for(100ms) !=
	std::future_status::ready)
{
	...
}
// 해결방안
if (fut.wait_for(0s) ==
	std::future_status::deferred)
{
	...
}
else {
	while (fut.wait_for(100ms) !=
		std::future_status::ready) {
		…
	}
	…
}

// 기본 시동 방침과 함께 std::async를 사용하는 것은 다음 조건들이 모두 성립할때 적합
// - 과제가 get이나 wait를 호출하는 스레드와 반드시 동시적으로 실행되어야 하는 것은 아니다.
// - 여러 스레드 중 어떤 스레드의 thread_local 변수들을 읽고 쓰는지가 중요하지 않다.
// - std::async가 돌려준 미래객체에 대해 get,wait가 반드시 호출된다는 보장, 과제가 전혀 실행되지 않아도 괜찮다.
// - 과제가 지연된 상태일 수도 있다는 점이 wait_for, wait_until을 사용하는 코드에 반영


// 위 조건 중 하나라도 성립하지 않는다면 std::async가 주어진 과제를 비동기적으로 실행하도록 강제
auto fut = std::async(std::launch::async, f);

// std::launch::async 명시적으로 지정하지 않도록
// C++11
template<typename F, typename... Ts>
inline
std::future<typename std::result_of<F(Ts...)>::type>
reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async,
		std::forward<F>(f),
		std::forward<Ts>(params)...);
}

auto fut = reallyAsync(f);

// C++14
template<typename F, typename... Ts>
inline
auto // C++14
reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async,
		std::forward<F>(f),
		std::forward<Ts>(params)...);
}

// - std::async의 기본 시동 방침은 과제의 비동기적 실행과 동기적 실행을 모두 허용한다.
// - 그러나 이러한 유연성 때문에 thread_local 접근의 불확실성이 발생하고, 과제가
//   절대로 실행되지 않을 수도 있고, 시간 만료 기반 wait 호출에 대한 프로그램 논리에도 영향을 미친다.
// - 과제를 반드시 비동기적으로 실행하야 한다면 std:launch::async를 지정하라.