//Pimpl 관용구를 사용할 때에는 특수 멤버 함수들을 구현 파일에서 정의하라.

#include <string>
#include <vector>
#include <memory>

//Pimpl 관용구("pointer to implementation" idiom)는 클래스의 자료 멤버들을 구현 클래스(또는 구조체)를 가리키는 포인터로 대체하고, 
//일차 클래스에 쓰이는 자료 멤버들을 그 구현 클래스로 옮기고, 포인터를 통해서 그 자료 멤버들에 간접적으로 접근하는 기법이다.
class Gadget {};

class Widget { //widget.h 헤더 파일 안에서
public:
	Widget();

private:
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3; //Gadget은 어떤 사용자 정의 형식
};

//위 Widget의 자료 멤버 std::string, std::vector, Gadget형식의 정의가 필요하다. 
//<string>과 <vector>는 자주 변경되지 않지만 gadget.h는 자주 변경될 수 있다.c++98에서 Pimpl 관용구를 적용했다면 
//Wiget의 자료멤버들을 선언만 하고 정의하지 않는 구조체를 가리키는 포인터로 대체 한다.

class Widget2 {
public:
	Widget2();
	~Widget2();
private:
	struct Impl; //구현용 구조체와 그것을
	Impl *pImpl; //가리키는 포인터 선언
};

//Widget클래스는 필요형식의 헤더들을 #include로 추가할 필요가 없으며,
//그 헤더들이 변경되어도 Widget 클라이언트에는 영향을 미치지 않는다.
//이는 선언만하고 정의는 하지 않는 불완전 형식(incomplete type)으로 불리며, Widget::Impl은 그런 형식이다.

//이처럼 Pimpl 관용구 적용의 단계를 보면
//첫 번째, 불완전 형식을 가리키는 포인터를 하나의 자료 멤버로 선언하는 것
//두 번째, 기존 클래스에서 사용하던 자료 멤버들을 담는 객체를 동적으로 할당, 해제하는 코드를 추가하는 것(해당 코드는 소스파일(.cpp)에 둔다)

//widget.cpp
struct Widget2::Impl { //Widget에 있던 자료 멤버들을 정의
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;
};
Widget2::Widget2()
	: pImpl(new Impl) //자료멤버 할당
{}
Widget2::~Widget2()
{
	delete pImpl;
} //자료멤버 제거

//의존성이 widget.h에서 widget.cpp로 옮겨졌다.또한 동적 객체 해지를 위해 소멸자가 필요한데, 
//이때 유용한 똑똑한 포인터는 std::unique_ptr이다.

//widget.h
class Widget3 {
public:
	Widget3();
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl; //생 포인터 대신 사용
};

//widget.cpp
//#include "widget.h"
//#include "gadget.h"

struct Widget3::Impl {
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;
};
Widget3::Widget3()
	: pImpl(std::make_unique<Impl>()) //std::make_unique를 이용해 std::unique_ptr을 생성
{}

//소멸자가 없는 이유는 std::unique_ptr이 파괴될때 자동으로 객체를 삭제하기 때문에, 
//소멸자를 넣을 필요가 없다.하지만 아래와 같은 경우 오류가 발생한다.

//#include "widget.h"
//Widget w;

//위 문제는 w가 파괴될 때, w의 소멸자가 호출되는데, std::unique_ptr을 이용하는 Widget클래스에 따로 소멸자가 선언되지 않았다.
//이때 컴파일러는 소멸자를 생성하고, 대부분의 표준 삭제자 함수는 delete적용 전 불완전한 형식을 가리키지 않는지 점검한다.
//c++11의 static_assert를 이용해 점검하며, 이때 참이 아닌 것으로 판정되어 오류 메시지가 발생한다.

//해결방법은 std::unique_ptr<Widget::Impl>을 파괴하는 코드가 만들어지는 지점에서 Widget::Impl이 완전한 형식이 되게 하면 문제가 바로 해결된다.

//widget.h에 추가
//~Widget(); // 선언만

//widget.cpp에정의
//Widget::~Widget() {}//~Widget의 정의
//Widget::~Widget() = default;//~Widget의 정의


//위의 소멸자 문제는 해결됐지만, 항목 17에서 말한 Widget에 소멸자를 선언하면 컴파일러는 이동 연산들을 작성하지 않는다.따라서 이동 연산들을 직접 선언해야 한다.
//일반적으로 컴파일러는 이동 생성자 안에서 예외가 발생했을 때 pImpe을 파괴하기 위한 코드를 작성하는데 이때, 
//pImpl을 파괴하려면 Impl도 완전한 형식이어야 한다.이는 위에 소멸자 문제와도 동일하다.즉, 이동 연산들의 정의를 구현파일로 옮기면 된다.
//widget.h
class Widget4 {
public:
	Widget4();
	~Widget4();

	// fail : 이동 생성자 안에 예외 발생시 Impl 파괴하려면 Impl이 완전형식 이여야 한다.
	//Widget4(Widget4&& rhs) = default;
	//Widget4& operator=(Widget4&& rhs) = default;

	Widget4(Widget4&& rhs); //선언만 하고 정의하지 않는다.
	Widget4& operator=(Widget4&& rhs);
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

//widget.cpp
#include <string>

struct Widget4::Impl { };
Widget4::Widget4()
	: pImpl(std::make_unique<Impl>())
{}

Widget4::~Widget4() = default;

Widget4::Widget4(Widget4&& rhs) = default; //여기서 정의
Widget4& Widget4::operator=(Widget4&& rhs) = default;

//또한, 만약에 Widget 클래스에 std::string, std::vector 처럼 Gadget이 복사가 가능한 형식이라면, 
//복사 연산자를 직접 정의해야한다.왜냐하면 std::unique_ptr같은 이동 전용 형식이 있는 클래스에 대해서는 복사 연산들을 작성해 주지 않으며, 
//작성한다 해도 얕은 복사를 수행하기 때문이다.
//widget.h
class Widget5 {
public:
	Widget5(const Widget5& rhs);
	Widget5& operator=(const Widget5& rhs);
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

//widget.cpp
//#include "widget.h"

struct Widget5::Impl { };
//Widget5::~Widget5() = default;

Widget5::Widget5(const Widget5& rhs) //복사 생성자
	: pImpl(nullptr)
{
	if (rhs.pImpl)
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
}

Widget5& Widget5::operator=(const Widget5& rhs) //복사 배정 연산자
{
	if (!rhs.pImpl)
		pImpl.reset();
	else if (!pImpl)
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	else
		*pImpl = *rhs.pImpl;

	return *this;
}

//Pimpl 관용구의 구현에 사용하기 적합한 포인터는 std::unique_ptr이다 std::shared_ptr의 경우, 이 항목의 문제점은 발생되지 않는다.

//std::shared_ptr을 사용한 경우
class Widget6 {
public:
	Widget6();
private:
	struct Impl;
	std::shared_ptr<Impl> pImpl;
};

//사용하는 경우
//Widget w1;
//auto w2(std::move(w1)); //w2를 이동 생성
//w1 = std::move(w2); //w1을 이동 배정

//모든 것이 제대로 컴파일되고 실행된다.std::unique_ptr에서 삭제자의 형식은 해당 똑똑한 포인터 형식의 일부이며, 
//이 덕분에 컴파일러는 더 작은 실행 시점 자료구조와 더 빠른 실행시점 코드를 만들어 낼 수 있다.
//반면 std::shared_ptr에서는 삭제자의 형식이 똑똑한 포인터 형식의 일부가 아니다.때문에 자료구조가 더 커지고 실행 코드가 다소 더 느려진다.


int main()
{

	return 0;
}

//- Pimpl 관용구는 클래스 구현과 클래스 클라이언트 사이의 컴파일 의존성을 줄임으로써 빌드 시간을 감소한다.
//- std::unique_ptr 형식의 pImpl 포인터를 사용할 때에는 특수 멤버 함수들을 클래스 해더에 선언하고 구현 파일에서 구현해야 한다.
//  컴파일러가 기본으로 작성하는 함수 구현들이 사용하기에 적합한 경우에도 그렇게 해야 한다.
//- 위의 조건은 std::unique_ptr에 적용될 뿐, std::shared_ptr에는 적용되지 않는다.