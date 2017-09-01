// decltype은 주어진 이름이나 표현식의 형식을 알려준다.
#include <deque>

class Widget{};

const int i = 0;			// decltype(i)는 const int
bool f(const Widget& w) { return true; };	// decltype(w)는 const Widget&
							// decltype(f)는 bool(const Widget&)

struct Point {
	int x, y;		// decltype(Point::x)는 int
};					// decltype(Point::y)는 int

Widget w;			// decltype(w)는 Widget

template<typename T>
class vector {
public:
	T& operator[](std::size_t index) { return a; }
	T a;
};

vector<int> v;	// decltype(v)는 vector<int>

int main()
{
	if (f(w))		// decltype(f(w))는 bool
	{

	}

	if (v[0] == 0)	// decltype(v[0])은 int&
	{

	}
}

template<typename Container, typename Index>
auto authAndAccess1(Container& c, Index i)
-> decltype(c[i])	// 후행 반환 형식,
{					// 함수의 반환 형식을 이 위치가 아니라 매개변수 목록 다음에 선언
	return c[i];	// 반환형식을 매개변수들을 이용해서 지정
}					// C++ 14에서는 필요없음

// 문제 발생
std::deque<int> d;
//authAndAccess1(d, 5) = 10;	// d[5]를 돌려주고, 10을 d[5]에 배정한다. 컴파일 안됨
// d[5]는 int&를 돌려주나, auto 반환 형식 연역 과정에서 참조가 제거되어 결국 반환형식은 int
// 수정방법: 위의 예제에서 auto -> decltype(auto)



const Widget& cw = w;
auto myWidget1 = cw;			// auto 형식 연역 : myWidget1의 형식은 Widget
decltype(auto) myWidget2 = cw;	// decltype 형식 연역 : myWidget2의 형식은 const Widget&

// authAndAccess의 최종상태
template<typename Container, typename Index>
auto authAndAccess2(Container&& c, Index i)	// 보편참조 매개변수를 받도록 수정(항목24)
-> decltype(/*std::forward<Container>*/c[i])	// C++ 11 형식
{											// 컨테이너 형식과 색인 객체의 형식 모름. 표준라이브러리가 색인 값을 사용.
	return /*std::forward<Container>*/c[i];		// 색인 매개변수에 대해서는 값 전달방식 고수, 보편참조에 std::forward 적용 (항목25)
}														

std::deque<std::string> makeStringDeque()
{
	std::deque<std::string> d;
	return d;
}
auto s = authAndAccess2(makeStringDeque(), 5);	// 가능, authAndAccess1은 불가능

int x = 0;	// decltype(x)는 int
			// decltype((x))는 int&		복잡한 표현식
decltype(auto) f1()
{
	int x = 0;
	return x;	// f1은 int 반환
}

decltype(auto) f2()
{
	int x = 0;
	return (x);	// f2는 int& 반환 (주의) 지역변수에 대한 참조
}

// decltype은 항상 변수나 표현식의 형식을 아무 수정없이 보고한다.
// 형식이 T이고 이름이 아닌 왼값 표현식에 대해서는 항상 T& 형식을 보고한다.
// C++14는 decltype(auto)를 지원
// auto처럼 초기치로부터 형식을 연역하지만, 그 형식연역과정에서 decltype의 규칙을 적용