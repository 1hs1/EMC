// �ܹ߼� ��� ��ſ��� void �̷� ��ü�� ����϶�.

#include <mutex>
#include <atomic>
#include <future>
#include <thread>

// �����尣 ��� ���
// 1. ���� ���� ��� : ���� ������ �ϳ��� ���� ������ ��ٸ���, ���� ������ ����� �߻��ϸ� �� ���� ������ ����.
std::condition_variable cv;	// ����� ���� ���� ����
std::mutex m;				// cv�� �Բ� ����� ���ؽ�

void notify_temp() {
	cv.notify_one();	// ������������ �˸���.
}

void response() {	// �������� ó��
	std::unique_lock<std::mutex> lk(m);	// mutex�� �ʿ��� ����(�ʿ���� ��Ȳ������ ������ �־�� ��)
	cv.wait(lk);
}
// ���� �߰� 1. ���� ���� ������ wait�� �����ϱ� ���� ���� ������ ���� ������ �����ϸ� ���� ������ ���߰� �ȴ�.(hang)
//			 2. wait ȣ�⹮�� ��¥ ����� ������� �ʴ´�.(��ٸ��� �ڵ尡 ���Ǻ����� �������� �ʾҴµ���, ��� �� �ִ� ���)
//				��ٸ��� ������ �����ϴ� ���ٸ� wait�� �Ѱ��ش�. cv.wait(lk, []{return  ��� �߻�����});

// 2. ���� bool �÷��� ���
std::atomic<bool> flag(false);
void check() {
	flag = true;	// ��� ����, ���� ������ ����
}
void responseflag() {
	while (!flag) {	// ����� ��ٸ���.
					// ����� �����Ѵ�.
	}				// ���� ����� ��� �Դ´�.
}

// 3. ȥ�չ��
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

// ���Ǻ����� ���ؽ�, �÷��׸� �ƿ� ����� �ʿ䰡 ���� �Ѱ��� �����,
// ���� ������ ������ �̷� ��ü�� ���� ������ ��ٸ���(wait�� ���ؼ�) �ϴ� ���̴�.
namespace promise {
	std::promise<void> p;	// ���ä�ο��� ����� ��� ��ü, ���� ������ �ڷᰡ ����, ���� �̷� ��ü�� �����Ǿ������� �˸� �� �� void ���

	void nodify_temp()
	{
		p.set_value();			// ���� �������� ����
	}

	void response()
	{
		p.get_future().wait();	// p�� �ش��ϴ� �̷� ��ü�� ��ٸ���.
	}
}
// std::promise�� �̷� ��ü ���̿��� ���� ���°� ������, ��ü�� ���� ���´� �������� �Ҵ�ȴ�.
// ���� �� ����� �� ��� �Ҵ� �� ���� ����� �����Ѵ�.
// std::promise�� �ѹ��� ������ �� �ִ�. std::promise�� �̷� ��ü ������ ��� ä���� ������ ��Ǯ���ؼ� ����� �� ���� �ܹ߼�
// �����带 �ѹ��� �����Ѵٸ�(���� ������ �ش� ������ �Լ��� ������� ���̿���), void �̷���ü�� �̿��ϴ� ���谡 �ո����� �����̴�.
namespace threadonce {
	std::promise<void> p;
	void react() {}	// ��������
	void detect()	// �������
	{
		std::thread t([]	// ������ ����
		{
			p.get_future().wait();	// �̷� ��ü�� �����ɶ����� t�� ����
			react();
		});

		p.set_value();	// t�� ������ Ǭ��.
		
		t.join();		// t�� �շ� �Ұ������� �����.
	}
}

namespace threadshare {
	std::promise<void> p;
	void react() {}	// ��������
	void detect()	// ������� - �������� ���������� �����Ѵ�.
	{
		auto sf = p.get_future().share();	// sf�� ������ std::shared_future<void>
		
		std::vector<std::thread> vt;		// ���� ��������� ��� �����̳�

		int threadToRun = 8;
		for (int i = 0; i < threadToRun; ++i)
		{
			vt.emplace_back([sf]	// sf�� ���� ���纻�� ��ٸ���.
			{sf.wait();
			 react();
			});
		}

		// ���� ���⼭ ���ܰ� �߻��ϸ� �շ� ������ std::thread���� �ı��Ǿ ���α׷��� ����ȴ�.
		// ���⿡ ���� ó���� �ż��� ��������? ������
		// http://scottmeyers.blogspot.kr/2013/12/threadraii-thread-suspension-trouble.html
		// http://scottmeyers.blogspot.kr/2015/04/more-on-threadraii-and-thread-suspension.html
			
		p.set_value();	// ��� �������� ������ Ǭ��.

		for (auto& t : vt) {	// ��� �����带 �շ� �Ұ������� �����.
			t.join();
		}
	}
}

// 1. ������ �������� ������ ��, ���� ���� ��� ���迡�� ������ ���ؽ��� �ʿ��ϰ�,
// ���� ������ ���� ������ ���� ������ ������ ������, ����� ������ �߻��ߴ����� ���� ������ �ٽ� Ȯ���ؾ� �Ѵ�.
// 2. �÷��� ��� ���踦 ����ϸ� �׷� �������� ������, ��� ������ �ƴ϶� ������ �Ͼ�ٴ� ������ �ִ�.
// 3. ���� ������ �÷��׸� ������ ���� ������, �׷� ������ �̿��� ��� ��Ŀ������ �ʿ� �̻����� �����ϴ�.
// 4. std::promise�� �̷� ��ü�� ����ϸ� �̷��� ���������� ���� �� ������,
// �׷� ���ٹ���� ���� ���¿� �� �޸𸮸� ����ϸ�, �ܹ߼� ��Ÿ� �����ϴ�.

int main(){}