// 스레드 기반 프로그래밍보다 과제 기반 프로그래밍을 선호하라.

#include <thread>
#include <future>

// doAsyncWork라는 함수를 비동기적으로 실행
int doAsyncWork() { return 0; }

int main()
{
	// 1. std::thread 객체를 생성해서 그 객체에서 doAsyncWork를 실행(스레드 기반)
	std::thread t(doAsyncWork);

	// 2. doAsyncWork를 std::Async에 넘겨주는 것(과제 기반)
	auto fut = std::async(doAsyncWork);

// 스레드 기반 호출에서는 doAsyncWork의 반환값에 접근할 방법이 없다. 그러나 과제 기반 호출에서는
// std::async가 돌려주는 미래객체에 get 멤버함수를 이용한다.
// 이 get 함수는 예외도 접근할 수 있다.
// 스레드 기반 접근 방식에서는 doAsyncWork가 예외를 던지면 프로그램이 죽는다.
}

// 과제 기반 접근방식은 좀 더 높은 수준의 추상을 체현, 프로그래머는 세부적인 스레드 관리에서 벗어날 수 있다.
// 스레드 : 실제 계산을 수행하는 하드웨어 스레드(CPU 코어당 하나 이상의 하드웨어 스레드 제공)
//			운영체제가 하드웨어 스레드들에서 실행되는 모든 프로세서와 일정을 관리하는 데 사용하는 소프트웨어 스레드(OS 스레드, 시스템 스레드)
//			C++ 표준 라이브러리의 std::thread, 하나의 C++ 프로세스 안에서 std::thread 객체는 바탕 소프트웨어 스레드에 대한 핸들로 작용한다.

// 소프트웨어 스레드는 제한된 자원이다. 시스템이 제공할 수 있는 것보다 많은 소프트웨어 스레드를 생성하려 하면 std::system_error 예외가 발생한다.
// 이는 스레드에서 실행하고자 하는 함수가 예외를 던질 수 없는 경우에도 마찬가지이다. noexcept라고 해도...
// 가용 스레드가 모자라지 않는다고 해도 과다구독(oversubscription)때문에 문제가 발생
// 과다구독이란 실행 준비가 된 소프트웨어 스레드가 하드웨어 스레드보다 많은 상황.(context switch 수행)

// 이러한 문제로 인하여 스레드 관리 부담을 표준 라이브러리에서 처리 std::async
// std::async의 경우 가용 소프트웨어 스레드가 부족하면 새 소프트웨어 스레드를 생성하지 않고,
// 기존 스레드(doAsytncWork의 결과가 필요한 스레드)에서 실행 가능

// std::async에서도 GUI 스레드의 반응성이 문제가 될 수 있다. 
// 스케쥴러로서는 독자의 스레드 중 반응성이 좋아야 하는 스레드가 어떤 것인지 알수가 없다.
// 그런 경우 std::launch::async라는 시동 방침을 std::async에 넘겨주는 것이 바람직
// 실행하고자 하는 함수가 실제로 현재 스레드와는 다른 스레드에서 실행(항목 36)

// 스레드를 직접 다루는 게 적합한 경우도 존재
// 1. 바탕 스레드 적용 라이브러리의 API(Windows 스레드 라이브러리)에 접근해아 하는 경우
// 2. 응용 프로그램의 스레드 사용량을 최적화해야 하는, 그리고 할 수 있어야 하는 경우
// 예를 들어 하드웨어 특성들이 미리 정해진 컴퓨터에서 유잃하게 의미 있는 프로세스로 실행될 서버 소프트웨어를 개발한다면...
// 3. C++ 동시성 API가 제공하는 것 이상의 스레드 적용 기술을 구현해야 하는 경우
// 예를 들어 스레드 풀을 제공하지 않는 특정 플랫폼을 위해 스레드 풀을 직접 구현해야 하는 경우

// 1. std::thread API에서는 비동기적으로 실행된 함수의 반환값을 직접 얻을 수 없으며, 만일 그런 함수가 예외를 던지면 프로그램이 종료한다.
// 2. 스레드 기반 프로그램밍에서는 스레드 고갈, 과다구독, 부하 균형화, 새 플랫폼으로의 적응을 독자가 직접 처리해야 한다.
// 3. std::async와 기본 시동 방침(항목36)을 이용한 과제 기반 프로그래밍은 그런 대부분의 문제를 알아서 처리해준다.
