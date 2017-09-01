// 명시적 선언보다는 auto를 선호하라

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

class Widget
{
public:
	bool operator < (Widget const& rhs)
	{
		return true;
	}
};

int main()
{
	int x1{};		// 문맥에 따라서 초기화 되지 않을 수 있음.
	//auto x2;		// 오류! 초기치가 꼭 필요함
	auto x3 = 0;	// 양호
	
	std::unordered_map<std::string, int> m;
	for (const std::pair<std::string, int>& p : m) // const auto& p : m
	{
		// m은 std::pair<const std:string, int> 객체, p는 std::pari<std::string, int> 형식, 변환 필요.
		// p를 묶고자 하는 형식의 임시 객체를 생성하고, m의 각 객체를 복사하고, 참조 p를 그 임시객체에 묶음으로써 변환 수행
		// auto를 사용하면 p는 임시 객체가 아닌 실제 m 안의 요소를 가리키는 포인터를 얻음.
	}
}

template<typename It>
void dwim(It b, It e)
{
	for (; b != e; ++b)
	{
		auto currValue = *b;	 //typename std::iterator_traits<It>::value_type currValue = *b;	//간단하지 않음
	}
}

// 컴파일러만 알던 형식을 지정
// std::unique_ptr들이 가리키는 Widget객체들을 비교하는 함수
// C++11 버전
auto derefUPLess = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2)
{return *p1 < *p2; };
// C++14 버전
auto derefLess = [](const auto& p1, const auto& p2)
{return *p1 < *p2; };
// auto 대신 std::function을 사용할 경우
std::function<bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&)>
derefUPFLess = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2)
{return *p1 < *p2; };
// auto 대신 std::function을 사용시 안좋은점
// 1. auto로 선언된 클로져를 담는 변수는 그 클로져와 같은 형식, 그 클로져에 요구되는 만큼의 메모리만 사용.
//	  std::function으로 선언된 클로져를 담은 변수는 std::function 템플릿의 한 인스턴스
//    그 크기는 임의의 주어진 서명에 대해 고정, 클로져를 저장하기에 부족할 수도 있고,
//    이 경우 힙메모리를 할당하여 클로져를 저장. 결과적으로 메모리를 더 많이 소비
// 2. std::function은 인라인화를 제한하고 간접 함수 호출을 산출하는 구현 세부사항 때문에,
//    auto로 선언된 객체를 통해 호출하는 것보다 느리다.
// * std::bind 대신 람다 사용이 좋다. 항목 34

std::vector<int> v;
unsigned sz = v.size();
// v.size()의 공식 반환형식 : std::vector<int>::size_type
// 32비트 Windows에서는 unsigned와 std::vector<int>::size_type와 같은 크기
// 64비트 Windows에서는 unsigned는 32비트, std::vector<int>::size_type은 64비트
// auto선언으로 해결가능 auto sz = v.size();

// 결론
// auto 변수는 반드시 초기화해야 하며, 이식성 또는 효율성 문제를 유발할 수 있는 형식 불일치가 발생하는 경우가 거의 없다.
// 대체로 변수의 혀식을 명시적으로 지정할 때보다 타자량도 더 적다.
