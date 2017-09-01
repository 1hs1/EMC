
//재정의 함수들을 override로 선언하라

#include <memory>
#include <vector>

//가상 함수 재정의는 파생 클래스 함수를 기반 클래스의 인터페이스를 통해서 호출할 수 있게 만드는 메커니즘이다.

/*
재정의가 일어나려면 다음과 같은 조건을 충족해야 한다.
- 기반 클래스 함수가 반드시 가상 함수이어야 한다.
- 기반 함수와 파생 함수의 이름이 반드시 동일해야 한다.(소멸자 제외)
- 기반 함수와 파생 함수의 매개변수 형식들이 반드시 동일해야 한다.
- 기반 함수와 파생 함수의 const성이 반드시 동일해야 한다.
- 기반 함수와 파생 함수의 반환 형식과 예외 명세가 반드시 호환되어야 한다.
//c++11에 추가된 제약
- 멤버 함수들의 참조 한정사들이 반드시 동일해야 한다.참조 한정사 기능을 사용하면 멤버 함수를 왼값에만 또는 오른값에만 사용할 수 있게 제한할 수 있다.
*/

class Base {
public:
	virtual void doWork() {}; //기반 클래스 가상 함수
};
class Derived : public Base {
public:
	virtual void doWork() {}; //base::doWork를 재정의
};


class Widget {
public:
	void doWork() & {}; //*this가 왼값일 때 적용
	void doWork() && {}; //*this가 오른값일 때 적용
};
Widget w;
//Widget makeWidget(); //팩토리 함수, 오른값 돌려줌


//기반 클래스에 참조 한정사가 선언되어 있을 경우, 파생 클래스에도 해당 함수가 정의 되어 있어야 한다.
class Base_fail {
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	void mf4() const;
};
class Derived_fail : public Base_fail {
public:
	virtual void mf1(); // const
	virtual void mf2(unsigned int x); // unsigned int
	virtual void mf3() && ; // &, &&
	void mf4() const; // virtual
};
/*
class Base_fail_2 {
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	void mf4() const;
};
class Derived_fai_2 : public Base_fail_2 {
public:
	virtual void mf1() override; // const
	virtual void mf2(unsigned int x) override; // unsigned int
	virtual void mf3() && override; // &, &&
	void mf4() const override; // virtual
};
*/

//c++11, override 예제
class Base2 {
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	virtual void mf4() const;
};

class Derived2 : public Base2 {
public:
	virtual void mf1() const override;
	virtual void mf2(int x) override;
	virtual void mf3() & override;
	void mf4() const override; //virtual을 생략해도 된다.
};

//파생 클래스의 모든 재정의 함수를 override로 선언하는 것은, 기반 클래스의 한 가상함수를 변경 했을 때 도움이 된다.


//참조 한정자의 장점 data가 오른값 Widget의 호출된 경우 오른값 리턴, 왼값 Widget 호출된 경우 왼값
//data를 왼값 Widget과 오른값 Widget에 대해 개별적 중복 적재
class Widget2 {
public:
	using DataType = std::vector<double>;

	DataType& data() & { return values; } //왼값 Widget에 대해서 왼값 반환
	DataType&& data() && { return std::move(values); } //오른값 Widget에 대해서 오른값 반환
private:
	DataType values;
};
Widget2 w2;
//Widget2 makeWidget(); //팩토리 함수, 오른값 돌려줌

int main()
{
	//파생 클래스 객체를 가리키는 기반 클래스 포인터
	std::unique_ptr<Base> upb = std::make_unique<Derived>();
	upb->doWork(); //기반 클래스 포인터로 doWork()를 호출, 파생 클래스의 함수가 호출된다.

	w.doWork(); //왼값용, Widget::doWork&를 호출
	//makeWidget().doWork(); //오른값용, Widget::doWork&&를 호출

	auto vals1 = w2.data(); //왼값 중복적재 호출, 복사 생성됨
  //auto vals2 = makeWidget2().data(); //오른값 중복적재 호출, 이동 생성됨
}

//멤버 함수에 참조 한정사가 붙어 있으면 그 함수의 모든 중복적재에도 참조 한정사를 지정해야 한다.
//이는 참조 되지 않은 중복적재가 왼값 객체와 오른값 객체 모두 호출될 수 있기 때문이다.

//- 재정의 함수는 override로 선언하라
//- 멤버 함수 참조 한정사를 이용하면 멤버 함수가 호출되는 객체(*this)의 왼값 버전과 오른값 버전을 다른 방식으로 처리할 수 있다.