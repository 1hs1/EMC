//const 멤버 함수를 스레드에 안전하게 작성하라

#include <vector>
#include <mutex>
#include <atomic>
#include <math.h> 

//다항식을 대표하는 클래스를 통해 다항식의 근들, 즉 다항식이 0으로 평가되는 값들을 계산하는 함수가 있을 때,
//계산하는 비용이 클 수 있으므로, 필요한 시점에만 계산하여 캐시에 저장하고 그렇지 않을 때는 캐시에 있는 값을 돌려주도록 하는 것이 바람직하다.
class Polynomial {
public:
	using RootsType = std::vector<double>;
	RootsType roots() const
	{
		if (!rootsAreValid) { //캐시가 유요하지 않으면
			//... 근들을 계산해서 rootVals에 저장
			rootsAreValid = true;
		}
		return rootVals;
	}
private:
	mutable bool rootsAreValid{ false };
	mutable RootsType rootVals{};
};
//이 때 두 스레드가 동시에 roots를 호출하면, 안전하지 않다.
//roots 안에서 두 스레드 중 하나나 둘 다가 자료 멤버 rootsAreValid와 rootVals를 수정하려 들기 때문이다.
//즉, 이 클라이언트 코드는 서로 다른 스레드들이 같은 메모리를 동기화 없이 읽고 쓰려고 할 수 있으며, 이는 자료 경쟁(data race)와 일치한다.


//문제 해결 : 통상적인 동기화 수단, 뮤텍스(std::mutex)를 사용하는 것
class Polynomial1 {
public:
	using RootsType = std::vector<double>;
	RootsType roots() const
	{
		std::lock_guard<std::mutex> g(m); //뮤텍스를 잠근다.
		if (!rootsAreValid) { //캐시가 유요하지 않으면 계산해서 저장
			//...
				rootsAreValid = true;
		}
		return rootVals;
	} //뮤텍스를 푼다.
private:
	mutable std::mutex m;
	mutable bool rootsAreValid{ false };
	mutable RootsType rootVals{};
};
//std::mutex를 복사하거나 이동할 수 없기 때문에, m을 Polynomial에 추가하면 Polynomial의 복사와 이동 능력도 사라진다.
//std::mutex대신 단순한 카운트를 위해 std::atomic 카운터를 이용해서 비용을 줄일 수 있다. (but 복사와 이동 불가능 동일)

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

//동기화가 필요한 변수 하나 또는 메모리 장소 하나에 대해서는 std::atomic을 사용하는 것이 적합하지만, 
//둘 이상의 변수나 메모리 장소를 하나의 단위로서 조작해야 할 때에는 뮤텍스를 꺼내는 것이 바람 직하다.
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
			cachedValue = val1 + val2; // 값 배정
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
			return cachedValue = val1 + val2; // 값 배정
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
			cachedValue = val1 + val2; // 값 배정
			cacheValid = true;
			return cachedValue;
		}
	} // unlock m
private:
	mutable std::mutex m;
	mutable int cachedValue; // no longer atomic
	mutable bool cacheValid{ false }; // no longer atomic
};

//- 동시적 문맥에서 쓰이지 않을 것이 확실한 경우가 아니라면, const 멤버 함수는 스레드에 안전하게 작성하라.
//- std::atomic 변수는 뮤텍스에 비해 성능상의 이점이 있지만, 하나의 변수 또는 메모리 장소를 다룰 때에만 적합하다.

int main()
{
	return 0;
}