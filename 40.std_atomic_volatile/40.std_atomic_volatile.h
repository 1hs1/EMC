#pragma once
// 동시성에는 std::atomic을 사용하고, volatile은 특별한 메모리에 사용하라.

// std::atomic
std::atomic<int> ai(0);
ai = 10;
std::cout << ai;
++ai;
--ai;
// 전체 문장이 원자적으로 처리된다는 보장은 없다
// 읽는 시점만 원자적이다


//volatile
volatile int vi(0);
vi = 10;
std::cout << vi;
++vi;
--vi;
// 다중 스레드 문맥에서 아무것도 보장하지 않는다.


// 다중 스레드 프로그램 카운터 예
std::atomic<int> ac(0);
volatile int vc(0);

/*----- Thread 1 ----- */ /*------- Thread 2 ------- */
++ac;											++ac;
++vc;											++vc;

// ac : 반드시 2
// vc : 2일 수도 아닐 수도 있다.


// atomic 순차적 일관성
// 코드 재배치 제약
std::atomic<bool> valAvailable(false);
auto imptValue = computeImportantValue();
valAvailable = true;

ex)
a = b;
x = y;
// 컴파일러가 순서 바꿀수 있다.
x = y;
a = b;

// volatile
volatile bool valAvailable(false);
auto imptValue = computeImportantValue();
valAvailable = true;

// 1. 연산의 원자성을 보장하지 않는다
// 2. 코드 재배치에 대한 제약이 충분하지 않다.


// 남아도는 적재, 죽은 저장
int x;
auto y = x;
y = x;

x = 10;
x = 20;

// 컴파일러 변환 ( 최적화 )
auto y = x;
x = 20;

// volatile
// 해당 메모리에 대한 연산들에는 그 어떤 최적화도 수행하지 말라
volatile int x;
auto y = x;
y = x; // 최적화로 제거할 수 없음
x = 10;
x = 20; // 최적화로 제거할 수 없음
// 메모리 대응 입출력(memory-mapped I/O) 유효


// atomic
std::atomic<int> x;
auto y = x;
y = x;
x = 10;
x = 20;

// 최적화 ok, 컴파일 error
auto y = x;
y = x;

// 복사 생성/ 복사 배정 삭제 ( x읽고 y를 기록하는 작업을 하나의 원자적 연산으로 수행 불가 )
// 항목 17 특수 멤버 함수 생성 규칙 : 이동 생성, 이동 배정 제공하지 않는다.


std::atomic<int> y(x.load());
y.store(x.load());

// 컴파일러 : 레지스테에 저장해 최적화 할수도
register = x.load();
std::atomic<int> y(register);
y.store(register);

// volatile, std::atomic 함께 사용 가능
volatile std::atomic<int> vai;

// - std::atomic은 뮤텍스 보호 없이 여러 스레드가 접근하는 자료를 위한 것으로, 동시적 소프트웨어의 작성을 위한 도구이다.
// - volatile은 읽기와 기록을 최적화로 제거하지 말아야 하는 메모리를 위한 것으로, 특별한 메모리를 다룰 때 필요한 도구이다.
