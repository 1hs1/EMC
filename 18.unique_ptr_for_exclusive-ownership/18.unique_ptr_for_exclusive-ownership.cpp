#include <memory>

//소유권 독점 자원의 관리에는 std::unique_ptr를 사용하라

//std::unique_ptr은 생 포인터 동일한 명령들을 실행한다.
//메모리와 cpu주기가 여유롭지 않아도 사용할 수 있다.
//std::unique_ptr는 독점적 소유권(exclusive ownership)의미론을 재현하고 있다.
//NULL이 아닌 std::unique_ptr은 항상 자신이 가리키는 객체를 소유한다.
//std::unique_ptr은 이동 전용 형식으로 std::unique_ptr 소멸 시 자신이 가리키는 자원을 파괴한다.

//객체를 생성하는 팩토리 반환 함수에서, 팩토리 함수는 힙에 객체를 생성하고 그 객체를 가리키는 포인터를 돌려준다.
//또한 소멸 시 피지칭 객체를 자동으로 삭제해 준다.
class Investment { };
class Stock :
	public Investment {
};
class Bond :
	public Investment {
};
class RealEstate :
	public Investment {
};
template<typename... Ts>
std::unique_ptr<Investment>
makeInvestment(Ts&&... params);
/*
{
	{
		auto pInvestment = makeInvestment(인수);
	} // *pInvestment 가 파괴된다
}
*/

//std::unique_ptr의 파괴는 기본적으로 delete를 통해 일어나지만, 
//std::unique_ptr객체를 생성할 때 커스텀 삭제자를 사용하도록 지정하는 것도 가능하다.

void makeLogEntry(Investment* pInvestment) {};
//makeInvestment 생성 객체를 delete로 삭제하지 않고 로그 항목 기록 후에 삭제
auto delInvmt = [](Investment* pInvestment) // custom
{ 
	makeLogEntry(pInvestment);
	delete pInvestment;
};
template<typename... Ts> // revised
std::unique_ptr<Investment, decltype(delInvmt)> // return type
makeInvestment(Ts&&... params)
{
	std::unique_ptr<Investment, decltype(delInvmt)> // ptr to be
		pInv(nullptr, delInvmt);
	if ( /*Stock object 생성경우*/)
	{
		pInv.reset(new Stock(std::forward<Ts>(params)...));
	}
	else if ( /*Bond object 생성경우*/)
	{
		pInv.reset(new Bond(std::forward<Ts>(params)...));
	}
	else if ( /*RealEstate object 생성경우*/)
	{
		pInv.reset(new RealEstate(std::forward<Ts>(params)...));
	}
	return pInv;
}
//-delInvmt는 커스텀 삭제자이다.makeLogEntry를 호출 후 delete를 적용한다.
//-커스텀 삭제자 사용시 delInvmt을 std::unique_ptr의 둘째 형식 인수로 지정해야 한다.
// makeInvestment의 반환 형식이 std::unique_ptr<Investment, decltype(delInvmt)>이다
//-makeInvestment는 널 std::unique_ptr을 만들어서 적절한 형식의 객체를 가리키게 한 후 돌려준다.
//-생 포인터를 std::unique_ptr에 배정하는 문장은 컴파일되지 않는다.때문에 new로 생성한 객체의 소유권을 부여하기 위해 reset을 호출했다.
//-함수에 전달된 인수들을 new에 완벽하게 전달하기 위해 std::forward를 사용했다.
//-커스텀 삭제자는 Investment* 형식의 매개변수를 받는다.그 객체는 람다식 안에서 Investment* 객체로서 delete된다.
// 이를 위해서 Investment의 소멸자가 virtual 이어야 한다.
/*
class Investment {
public:
	virtual ~Investment();
};
*/
// C++14
template<typename... Ts>
auto makeInvestment(Ts&&... params) // C++14
{
	auto delInvmt = [](Investment1* pInvestment)
	{ // inside
		makeLogEntry(pInvestment); // makedelete
		pInvestment; // Investment
	};
}


//std::unique_ptr의 객체의 크기는 생포인터의 크기와 비슷하나, 커스텀 삭제자의 경우 달라진다.
//삭제자가 함수 객체일 때에는 std::unique_ptr의 크기가 그 함수 객체에 저장된 상태의 크기만큼 증가한다.
//상태 없는 함수 객체의 경우 크기 변화가 없으며, 따라서 보통 함수로도 구현할 수 있지만, 람다 표현식이 더 선호한다.
// 반환 형식의 크기는 Investment*
auto delInvmt1 = [](Investment* pInvestment)
{
	makeLogEntry(pInvestment);
	delete pInvestment;
};
template<typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt1)>
makeInvestment(Ts&&... args);
void delInvmt2(Investment* pInvestment)
{
	makeLogEntry(pInvestment);
	delete pInvestment;
}
// 반환 형식의 크기 : Investment* + 함수 포인터의 크기
template<typename... Ts>
std::unique_ptr<Investment,
void(*)(Investment*)> 
makeInvestment(Ts&&... params);


//상태가 많은 함수 객체 삭제자를 사용한다면 std::unique_ptr의 크기가 허용 수준을 넘어설 수 있으며, 
//이 경우 설계를 변경해야 할수 도 있다.

//std::unique_ptr은 사용 대상에 따라 차이가 있다.객체일 경우 색인 적용 연산(operator[])를 제공하지 않으며, 
//배열일 경우 역참조 연산자들(operator*과 operator->)를 제공하지 않는다.

//std::unique_ptr은 std::shared_ptr로도 변환이 용이하다.때문에 std::unique_ptr는 팩토리 함수의 반환형식으로 적합하다.
//std::shared_ptr<Investment> sp = makeInvestment(args);


//- std::unique_ptr는 독점 소유권 의미론을 가진 자원의 관리를 위한, 작고 빠른 이동 전용 똑똑한 포인터이다.
//- 기본적으로 자원파괴는 delete를 통해 일어나나, 커스텀 삭제자를 지정할 수도 있다.
//  상태가 있는 삭제자나 함수 포인터를 사용하면 std::unique_ptr 객체의 크기가 커진다.
//- std::unique_ptr를 std::shared_ptr로 손쉽게 변환할 수 있다.

int main()
{
	return 0;
}