//const ��� �Լ��� �����忡 �����ϰ� �ۼ��϶�

#include <vector>
#include <mutex>
#include <atomic>
#include <math.h> 

//���׽��� ��ǥ�ϴ� Ŭ������ ���� ���׽��� �ٵ�, �� ���׽��� 0���� �򰡵Ǵ� ������ ����ϴ� �Լ��� ���� ��,
//����ϴ� ����� Ŭ �� �����Ƿ�, �ʿ��� �������� ����Ͽ� ĳ�ÿ� �����ϰ� �׷��� ���� ���� ĳ�ÿ� �ִ� ���� �����ֵ��� �ϴ� ���� �ٶ����ϴ�.
class Polynomial {
public:
	using RootsType = std::vector<double>;
	RootsType roots() const
	{
		if (!rootsAreValid) { //ĳ�ð� �������� ������
			//... �ٵ��� ����ؼ� rootVals�� ����
			rootsAreValid = true;
		}
		return rootVals;
	}
private:
	mutable bool rootsAreValid{ false };
	mutable RootsType rootVals{};
};
//�� �� �� �����尡 ���ÿ� roots�� ȣ���ϸ�, �������� �ʴ�.
//roots �ȿ��� �� ������ �� �ϳ��� �� �ٰ� �ڷ� ��� rootsAreValid�� rootVals�� �����Ϸ� ��� �����̴�.
//��, �� Ŭ���̾�Ʈ �ڵ�� ���� �ٸ� ��������� ���� �޸𸮸� ����ȭ ���� �а� ������ �� �� ������, �̴� �ڷ� ����(data race)�� ��ġ�Ѵ�.


//���� �ذ� : ������� ����ȭ ����, ���ؽ�(std::mutex)�� ����ϴ� ��
class Polynomial1 {
public:
	using RootsType = std::vector<double>;
	RootsType roots() const
	{
		std::lock_guard<std::mutex> g(m); //���ؽ��� ��ٴ�.
		if (!rootsAreValid) { //ĳ�ð� �������� ������ ����ؼ� ����
			//...
				rootsAreValid = true;
		}
		return rootVals;
	} //���ؽ��� Ǭ��.
private:
	mutable std::mutex m;
	mutable bool rootsAreValid{ false };
	mutable RootsType rootVals{};
};
//std::mutex�� �����ϰų� �̵��� �� ���� ������, m�� Polynomial�� �߰��ϸ� Polynomial�� ����� �̵� �ɷµ� �������.
//std::mutex��� �ܼ��� ī��Ʈ�� ���� std::atomic ī���͸� �̿��ؼ� ����� ���� �� �ִ�. (but ����� �̵� �Ұ��� ����)

class Point {
public:

	double distanceFromOrigin() const noexcept
	{
		++callCount;
		return std::hypot(x, y);
	}
private:
	mutable std::atomic<unsigned> callCount{ 0 };
	double x, y;
};

//����ȭ�� �ʿ��� ���� �ϳ� �Ǵ� �޸� ��� �ϳ��� ���ؼ��� std::atomic�� ����ϴ� ���� ����������, 
//�� �̻��� ������ �޸� ��Ҹ� �ϳ��� �����μ� �����ؾ� �� ������ ���ؽ��� ������ ���� �ٶ� ���ϴ�.
int expensiveComputation1();
int expensiveComputation2();
class Widget1 {
public:
	int magicValue() const
	{
		if (cacheValid) return cachedValue;
		else {
			auto val1 = expensiveComputation1();
			auto val2 = expensiveComputation2();
			cachedValue = val1 + val2; // �� ����
			cacheValid = true;
			return cachedValue;
		}
	}
private:
	mutable std::atomic<bool> cacheValid{ false }; // atomic
	mutable std::atomic<int> cachedValue; // atomic
};
class Widget2 {
public:
	int magicValue() const
	{
		if (cacheValid) return cachedValue;
		else {
			auto val1 = expensiveComputation1();
			auto val2 = expensiveComputation2();
			cacheValid = true;
			return cachedValue = val1 + val2; // �� ����
		}
	}
private:
	mutable std::atomic<bool> cacheValid{ false }; // atomic
	mutable std::atomic<int> cachedValue; // atomic
};
class Widget3 {
public:
	int magicValue() const
	{
		std::lock_guard<std::mutex> guard(m); // lock m
		if (cacheValid) return cachedValue;
		else {
			auto val1 = expensiveComputation1();
			auto val2 = expensiveComputation2();
			cachedValue = val1 + val2; // �� ����
			cacheValid = true;
			return cachedValue;
		}
	} // unlock m
private:
	mutable std::mutex m;
	mutable int cachedValue; // no longer atomic
	mutable bool cacheValid{ false }; // no longer atomic
};

//- ������ ���ƿ��� ������ ���� ���� Ȯ���� ��찡 �ƴ϶��, const ��� �Լ��� �����忡 �����ϰ� �ۼ��϶�.
//- std::atomic ������ ���ؽ��� ���� ���ɻ��� ������ ������, �ϳ��� ���� �Ǵ� �޸� ��Ҹ� �ٷ� ������ �����ϴ�.

int main()
{
	return 0;
}