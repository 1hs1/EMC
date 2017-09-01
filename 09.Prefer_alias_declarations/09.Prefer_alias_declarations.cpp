// typedef보다 별칭 선언을 선호하라

#include <memory>
#include <unordered_map>

class Widget{};

// 기본적인 사항은 비슷하다
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSStypedef;
using UptrMapSSusing = std::unique_ptr <std::unordered_map<std::string, std::string>>;

// 함수 포인터... 약간 틀림
typedef void(*FP)(int, const std::string&);
using FP = void(*)(int, const std::string&);

// 템플릿에서 많이 차이남. typedef는 템플릿화할수 없지만, 별칭 선언은 템플릿화 할수 있다.
template<typename T>
using MyAllocList1 = std::list<T>;	// 형식 템플릿
MyAllocList1 <Widget> lw1;

// 위의 MyAllocList1을 클래스 멤버로 가지려고 할 때...
template<typename T>
class WidgetUsing {
private:
	MyAllocList1<T> list;
	// MyAllocList1<T>는 반드시 형식의 이름. 비의존적 형식. typename을 붙일 필요가 없다.
};

template<typename T>
struct MyAllocList2 {
	typedef std::list<T> type;
};
MyAllocList2<Widget>::type lw2;

// 위의 MyAllocList2를 클래스 멤버로 가지려고 할 때...
template<typename T>
class WidgetTypedef {
private:
	typename MyAllocList2<T>::type list;                                                                                                                                                     
	// 템플릿 형식 매개변수(T)에 의존적인 형식, 의존적인 형식 앞에 반드시 typename을 붙여야 한다.
};

// MyAllocList2<T>::type가 형식이 아닐 경우가 있음(그래서 typename을 붙여야 한다.)
class Wine{};
template<>
class MyAllocList2<Wine> {	// T가 Wine인 경우에 대한 MyAllocList2의 특수화
private:
	enum class WineType{White, Red, Rose};
	WineType type;	// 이 클래스에서 type은 자료 멤버
};

// 변환을 적용하고자 하는 형식 T가 주어졌을 때, 변환을 적용한 결과로 나온 형식은 항상 std::변환<T>::type이다.
template<class T>
typename std::remove_const<T>::type a;		// const T를 T로 변환
template<class T>
std::remove_const_t<T> b;			// C++14 버젼
template<class T>
typename std::remove_reference<T>::type c;	// T&나 T&&를 T로 변환
template<class T>
std::remove_reference_t<T> d;			// C++14 버젼
template<class T>
typename std::add_lvalue_reference<T>::type e;	// T를 T&로 변환
template<class T>
std::add_lvalue_reference_t<T> f;			// C++14 버젼

// 만약 지금 C++11만 쓰고 있다면...
template<class T>
using remove_const_ty = typename std::remove_const<T>::type;	// 이런식으로 고쳐서 사용해도 좋다.

int main()
{}

// 1. typedef 는 템플릿화를 지원하지 않지만, 별칭 선언은 지원한다.
// 2. 별칭 템플릿에서는 "::type" 접미어를 붙일 필요가 없다. 템플릿 안에서 typedef를 지칭할 때에는 "typename" 접두사를 붙여야 하는 경우가 많다.
// 3. C++14는 C++11의 모든 형식 특질 변환에 대한 별칭 템플릿들을 제공한다.