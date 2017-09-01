// 0과 NULL보다 nullptr를 선호하라

#pragma once

#include <mutex>

// 0과 NULL은 포인트 형식이 아니다. 0이나 NULL은 함수를 호출 했을 때, 
// 포인터를 받는 함수가 호출 되는 일은 없다.
void f(int);
void f(bool);
void f(void*);

//f(0); //f(int)호출
//f(NULL); //f(int)호출, 컴파일 오류가 발생되기도 한다.

//nullptr의 장점은 정수형식과 포인트 형식이 아니다.실제 형식은 std::nullptr_t이며, 이는 다시 "nullptr의 형식"으로 정의된다.
//std::nullptr_t는 모든 생 포인터 형식으로 암묵적으로 변환되며, 
//때문에 nullptr는 마치 모든 형식의 포인터처럼 행동한다.
//f(nullptr); //f(void*) 호출

//nullptr의 경우 auto변수를 사용할 때 유용하다.
//auto result = findRecode(...);
//if (result == 0) {}; //result가 포인트인지 정수인지 명확히 알 수 없다.
//if (result == nullptr); //result가 포인터 형식임이 명백하다.

//nullptr은 템플릿 사용시 유용하다
class Widget {};
std::mutex f1m, f2m, f3m; //f1, f2, f3용 뮤텍스
int			f1(std::shared_ptr<Widget> spw);
double	f2(std::unique_ptr<Widget> upw);
bool		f3(Widget* pw) { return true; };
/*
using MuxGuard =
std::lock_guard<std::mutex>;
{
	MuxGuard g(f1m);
	auto result = f1(0);
}
{
	MuxGuard g(f2m);
	auto result = f2(NULL);
}
{
	MuxGuard g(f3m);
	auto result = f3(nullptr);
}
*/
template<typename FuncType, typename MuxType, typename PtrType>
decltype(auto) lockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
{
	using MuxGuard = std::lock_guard<MuxType>;

	MuxGuard g(mutex);
	return func(ptr);
}

//auto result1 = lockAndCall(f1, f1m, 0); //error
//auto result2 = lockAndCall(f2, f2m, NULL); //error
auto result3 = lockAndCall(f3, f3m, nullptr); //ok

//result1의 0은 매개변수가 int이다.때문에 std::shared_ptr<Widget>매개변수와 호환되지 않으므로 컴파일 오류가 난다.
//result2의 NULL또한 동일하다. 반면, nullptr를 이용한 호출에는 문제가 없다.
//ptr의 형식은 std::nullptr_t로 연역되어 ptr를 f3에 전달하면 std::nullptr_t에서 Widget*로 암묵적 변환이 일어난다.
//이처럼 널포인터를 지정할 때에는 0이나 NULL이 아니라 nullptr을 사용하라.


//기억해 둘 사항들
//- 0과 NULL보다 nullptr를 선호하라.
//- 정수 형식과 포인터 형식에 대한 중복적재를 피하라.


int main()
{
	return 0;
}