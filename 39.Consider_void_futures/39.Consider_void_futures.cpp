// 단발성 사건 통신에는 void 미래 객체를 고려하라.

#include <mutex>
#include <atomic>
#include <future>
#include <thread>

// 스레드간 통신 방법
// 1. 조건 변수 사용 : 반응 과제는 하나의 조건 변수를 기다리고, 검출 과제는 사건이 발생하면 그 조건 변수를 통지.
std::condition_variable cv;	// 사건을 위한 조건 변수
std::mutex m;				// cv와 함께 사용할 뮤텍스

void notify_temp() {
	cv.notify_one();	// 반응과제에게 알린다.
}

void response() {	// 반응과제 처리
	std::unique_lock<std::mutex> lk(m);	// mutex가 필요한 단점(필요없는 상황에서도 무조건 있어야 함)
	cv.wait(lk);
}
// 단점 추가 1. 만일 반응 과제가 wait를 실행하기 전에 검출 과제가 조건 변수를 통지하면 반응 과제가 멈추게 된다.(hang)
//			 2. wait 호출문은 가짜 기상을 고려하지 않는다.(기다리는 코드가 조건변수가 통지되지 않았는데도, 깨어날 수 있는 경우)
//				기다리던 조건을 판정하는 람다를 wait에 넘겨준다. cv.wait(lk, []{return  사건 발생여부});

// 2. 공유 bool 플래그 사용
std::atomic<bool> flag(false);
void check() {
	flag = true;	// 사건 검출, 반응 과제에 통지
}
void responseflag() {
	while (!flag) {	// 사건을 기다린다.
					// 사건을 반응한다.
	}				// 폴링 비용을 깎아 먹는다.
}

// 3. 혼합방식
namespace mix {
	std::condition_variable cv;
	std::mutex m;
	bool flag(false);

	void notify_temp()
	{
		{
			std::lock_guard<std::mutex> g(m);
			flag = true;
		}
		cv.notify_one();
	}

	void response()
	{
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [] { return flag; });
	}
}

// 조건변수와 무텍스, 플래그를 아예 사용할 필요가 없는 한가지 대안은,
// 검출 과제가 설정한 미래 객체를 반응 과제가 기다리게(wait를 통해서) 하는 것이다.
namespace promise {
	std::promise<void> p;	// 통신채널에서 사용할 약속 객체, 딱히 전송할 자료가 없고, 그저 미래 객체가 설정되었는지만 알면 될 때 void 사용

	void nodify_temp()
	{
		p.set_value();			// 반응 과제에게 통지
	}

	void response()
	{
		p.get_future().wait();	// p에 해당하는 미래 객체를 기다린다.
	}
}
// std::promise와 미래 객체 사이에는 공유 상태가 있으며, 대체로 공유 상태는 동적으로 할당된다.
// 따라서 이 설계는 힙 기반 할당 및 해제 비용을 유발한다.
// std::promise를 한번만 설정할 수 있다. std::promise와 미래 객체 사이의 통신 채널은 여러번 되풀이해서 사용할 수 없는 단발성
// 스레드를 한번만 유보한다면(생성 시점과 해당 스레드 함수의 실행시점 사이에서), void 미래객체를 이용하는 설계가 합리적인 선택이다.
namespace threadonce {
	std::promise<void> p;
	void react() {}	// 반응과제
	void detect()	// 검출과제
	{
		std::thread t([]	// 쓰레드 생성
		{
			p.get_future().wait();	// 미래 객체가 설정될때까지 t를 유보
			react();
		});

		p.set_value();	// t의 유보를 푼다.
		
		t.join();		// t를 합류 불가능으로 만든다.
	}
}

namespace threadshare {
	std::promise<void> p;
	void react() {}	// 반응과제
	void detect()	// 검출과제 - 여러개의 반응과제에 통지한다.
	{
		auto sf = p.get_future().share();	// sf의 형식은 std::shared_future<void>
		
		std::vector<std::thread> vt;		// 반응 스레드들을 담는 컨테이너

		int threadToRun = 8;
		for (int i = 0; i < threadToRun; ++i)
		{
			vt.emplace_back([sf]	// sf의 지역 복사본을 기다린다.
			{sf.wait();
			 react();
			});
		}

		// 만일 여기서 예외가 발생하면 합류 가능한 std::thread들이 파괴되어서 프로그램이 종료된다.
		// 여기에 대한 처리는 신성한 연습문제? ㅍㅍㅍ
		// http://scottmeyers.blogspot.kr/2013/12/threadraii-thread-suspension-trouble.html
		// http://scottmeyers.blogspot.kr/2015/04/more-on-threadraii-and-thread-suspension.html
			
		p.set_value();	// 모든 스레드의 유보를 푼다.

		for (auto& t : vt) {	// 모든 스레드를 합류 불가능으로 만든다.
			t.join();
		}
	}
}

// 1. 간단한 사건통신을 수행할 때, 조건 변수 기반 설계에는 여분의 뮤텍스가 필요하고,
// 검출 과제와 반응 과제의 진행 순서에 제약이 있으며, 사건이 실제로 발생했는지를 반응 과제가 다시 확인해야 한다.
// 2. 플래그 기반 설계를 사용하면 그런 단점들이 없지만, 대신 차단이 아니라 폴링이 일어난다는 단점이 있다.
// 3. 조건 변수와 플래그를 조합할 수도 있으나, 그런 조합을 이용한 통신 메커니즘은 필요 이상으로 복잡하다.
// 4. std::promise와 미래 객체를 사용하면 이러한 문제점들을 피할 수 있지만,
// 그런 접근방식은 공유 상태에 힙 메모리를 사용하며, 단발성 통신만 가능하다.

int main(){}