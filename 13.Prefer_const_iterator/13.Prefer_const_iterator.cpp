// const_iterator을 선호하라
// 반복자가 가리키는 것을 수정할 필요가 없을 때에는 항상 const_iterator를 사용하는 것이 바람직

#include <vector>

int main() 
{
	// C++98 버전
	std::vector<int> values;
	std::vector<int>::iterator it = std::find(values.begin(), values.end(), 1983);
	values.insert(it, 1998);

	// iterator가 가리키는 것을 전혀 수정하지 않기 때문에 최선의 선택은 아님
	// C++98 버전 const_iterator 사용
	typedef std::vector<int>::iterator IterT;
	typedef std::vector<int>::const_iterator ConstIterT;
	std::vector<int> valuesCo;
	ConstIterT ci = std::find(static_cast<ConstIterT>(valuesCo.begin()),
		static_cast<ConstIterT>(valuesCo.end()),
		1983);
	//valuesCo.insert(static_cast<IterT>(ci), 1998);	Error!!!
	//const_iterator에서 iterator로의 이식성 있는 변환은 존재하지 않는다.

	// C++11 버전
	std::vector<int> values11;
	auto iter = std::find(values11.cbegin(), values11.cend(), 1983);
	values11.insert(iter, 1998);
}

// 일반성을 극대화한 코드는 특정 멤버 함수의 존재를 요구하는 대신,
// 그 멤버 함수에 상응하는 비멤버 함수를 사용한다.
// 위의 검색 및 삽입 코드를 하나의 템플릿으로 일반화 (C++14 버전)
template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
	using std::cbegin;
	using std::cend;

	auto it = std::find(cbegin(container),
		cend(container),
		targetVal);

	container.insert(it.insertVal);
}
// C++11에서는 begin, end는 표준화에 추가했지만, cbegin, cend, rbegin, rend, crbegin, crend는 안하였다.
// C++11에서 비멤버 cbegin 구현방법
template<class C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
	return std::begin(container);
}

// iterator보다 const_iterator를 선호하라.
// 최대한 일반적인 코드에서는 begin, end, rbegin 등의 
// 비멤버 버전들을 해당 멤버 함수들보다 선호하라.
