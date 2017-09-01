// std::thread���� ��� ��ο��� �շ� �Ұ����ϰ� ������

#include <functional>
#include <vector>
#include <thread>

// ��� std::thread ��ü�� �շ� ���� �����̰ų� �շ� �Ұ��� �����̴�.
// �շ� ���� : ���� ���� ������ �� ���� ���� ���̰ų� ���� �� ���·� ������ �� �ִ� �����忡 ����
// ���� ��� ���ܵ� �����̰ų� ���� ������ ��ٸ��� ���� ���� ������, ������ �Ϸ�� ���� ������

// �շ� �Ұ��� : 1. �⺻ ������ std::thread : ������ �Լ��� �����Ƿ� ���� ���� ������� �������� �ʴ´�.
//				 2. �ٸ� std::thread ��ü�� �̵��� ���� std::thread ��ü : �̵��� �����, ���� std::thread�� �����Ǵ� ���� �������
//																		   ��� std::thread�� ���� �����尡 �ȴ�.
//				 3. join�� ���� �շ��� std::thread : join ������ std:thread ��ü�� ������ �Ϸ�� ���� ���� �����忡 �������� �ʴ´�.
//				 4. detach�� ���� Ż���� std::thread : detach�� std::thread ��ü�� �׿� �����Ǵ� ���� ������ ������ ������ ���´�.

// std::thread�� �շ� ���ɼ��� �߿��� ���� �ϳ���, ���� �շ� ������ �������� �Ҹ��ڰ� ȣ��Ǹ� ���α׷� ������ ����

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

	if (conditionsAreSatisfied()) {		// false�� �����ְų� ���ܸ� �����ٸ�, std::thread ��ü t�� �Ҹ��ڰ� ȣ��, ���α׷� ���� ����
		t.join();
		performComputation(goodVals);
		return true;
	}

	return false;
}

// �Ϲ��� join : std::thread�� �Ҹ��ڰ� ���� �񵿱� ���� �������� �ϷḦ ��ٸ��� �ϴ� ���̴�.
// ���� ��� conditionsAreSatisfied()�� �̹� false�� �����־��µ� ��� ���� ���Ͱ� ����Ǳ� doWork�� ��ٸ��� ���� ���������� �ʴ�.
// �Ϲ��� detach : std::thread�� �Ҹ��ڰ� std::thread ��ü�� ���� ���� ������ ������ ������ ���� �ϴ� ��.
// �� ��� ���� �����尡 ������ ����� �� �ִ�. ������ �� ���ڴ�.
// ���� ��� doWork���� goodVals�� ���� �����ε�, ���ٰ� ������ ���� �������ؼ� �ڽ��� �����ȿ��� ����.
// ���ٰ� �񵿱������� ����Ǵ� ���߿�, conditionsAreSatisfied()�� false�� �����ָ�, doWork�� ��ȯ�Ǹ�, ������������ �ı�.
// ���� doWork�� ���� �������� ������ ������ �帲�� doWork�� ȣ�� ���� �������� �Ѿ������, �ش� ������� doWork�� ȣ�� �������� ��� ����ȴ�.

// �̷��� �ΰ��� ������ �շ� ���� �����带 �ı����� ���� ����� ����� �������̹Ƿ� �׷� �ı��� �ƿ� ����.

// ���� std::thread ��ü�� ����� �� �� ��ü�� �װ��� ���ǵ� ���� �ٱ��� ��� ��ο��� �շ� �Ұ������� ����� ���� ���α׷����� å��.
// �� ������ �ٱ����� ������ ��� ��ο��� � ������ �ݵ�� ����Ǿ�� �� �� ���� ����ϴ� ���� ����� �� ������ ���� ��ü�� �Ҹ��� �ȿ� �ִ� ���̴�.
// �׷� ��ü�� RAII��ü��� �θ���, �׷� ��ü�� Ŭ������ RAII Ŭ������� �θ���.(STL �����̳�, ����Ʈ������, std::fstream ��)
// std::thread ��ü�� ���� ǥ�� RAII Ŭ������ ����.

// std::thread�� ���� RAII Ŭ���� : ThreadRAII ��ü�� �ı��� �� ���� std::thread ��ü�� ���� join�� ȣ���� �� �ƴϸ� detach�� ȣ������ ����
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

	ThreadRAII(ThreadRAII&&) = default;				// �̵� ���� ����
	ThreadRAII& operator=(ThreadRAII&&) = default;

	std::thread& get() { return t; }

private:
	DtorAction action;
	std::thread t;
};
// 1. �����ڴ� std::thread �������� �޴´�. (std::thread ��ü�� ������ �� ����)
// 2. �������� �Ű��������� ȣ���ڰ� ���������� ����� �� �ִ� ������ ����.
// ��� �ʱ�ȭ ����� �ڷ� ������� ����� ������ ������. �ڷ� ����� �ʱ�ȭ�� �ٸ� ����� ������ �� �ִ�.
// std::thread ��ü�� �ʱ�ȭ�������� �ش��Լ��� ������ ���� �����Ƿ�, Ŭ�������� std::thread �ڷ� ����� �׻� ���� �������� �����ϴ� ���� ����.
// std::thread�� �ٸ� ��� ����� ���������� �ʱ�ȭ�� �Ŀ� �ʱ�ȭ �ϸ�, �ش� �񵿱� �����尡 �ٸ� ��� ����� �����ϰ� ����
// 3. ThreadRAII�� ���� std::thread ��ü�� ������ �� �ִ� get �Լ��� ����. std::thread �������̽� ��ü�� ThreadRAII�� ������ �ʿ䰡 ����.
// ����, std::thread ��ü�� �䱸�ϴ� ���ƿ��� ThreadRAII ��ü�� ����� �� �ִ�.
// 4. ThreadRAII �Ҹ��ڴ� std::thread ��ü t�� ���� ��� �Լ��� ȣ���ϱ� ���� ���� t�� �շ� ������������ ����. 

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

	if (conditionsAreSatisfied()) {		// false�� �����ְų� ���ܸ� �����ٸ�, std::thread ��ü t�� �Ҹ��ڰ� ȣ��, ���α׷� ���� ����
		t.get().join();
		performComputation(goodVals);
		return true;
	}

	return false;
}

int main() {}

// 1. ��� ��ο��� std::thread�� �շ� �Ұ������� ������.
// 2. �Ҹ�� join ����� ������ϱ� ����� ���� �̻����� �̾��� �� �ִ�.
// 3. �Ҹ�� detach  ����� ������ϱ� ����� ������ �ൿ���� �̾��� �� �ִ�.
// 4. �ڷ� ��� ��Ͽ��� std::thread ��ü�� �������� �����϶�.