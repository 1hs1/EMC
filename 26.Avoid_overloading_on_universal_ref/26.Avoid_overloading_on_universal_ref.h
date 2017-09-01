#pragma once

//보편 참조에 대한 중복 적재를 피하라.


//ex)
std::multiset< std::string > names; //전역 자료 구조
void logAndAdd(const std::string& name)
{
	auto now = std::chrono::system_clock::now(); //현재시간을 얻음
	log(now, "logAndAdd"); //로그 저장
	name.emplace(name); //이름을 전역 자료구조에 저장
}

//위 함수에는 비효율성이 많이 남아 있다.예를 들어 아래의 함수 호출을 살펴보자.

std::string petName("Darla");
//case1
logAndAdd(petName); //왼값, std::string을 넘겨줌
//case2
logAndAdd(std::string("Persephone")); //오른값, std::string을 넘겨줌
//case3
logAndAdd("Patty Dog"); //문자열 리터럴을 넘겨줌

//case1 호출, name은 PetName에 묶이며, name이 왼값이므로 emplace는 name에 복사
//case2 호출, name이 오른값에 묶인다.name자체는 왼값으로 names에 복사된다.원칙적으로는 name의 값을 names로 이동하는 것이 가능
//case3 호출, name이 오른값에 묶이며, 암묵적으로 생성된 임시 std::string객체에 묶인다.
//std::string복사 1회 비용을 치르지만, 원칙적으로 복사는 커녕 이동 비용도 치를 필요 없다.


//만약, logAndAdd가 보편참조를 받게하고 그 참조를 std::forward를 이용해 emplace에 전달하면 case2,case3 호출의 비효율성을 제거할 수 있다.
template<typename T>
void logAndAdd(T&& name)
{
	auto now = std::chrono::system_clock::now();
	log(now, "logAndAdd");
	names.emplace(std::forward<T>(name));
}

std::string petName("Darla");
logAndAdd(petName);
logAndAdd(std::string("Persephone")); //오른값을 이동
logAndAdd("Patty Dog"); //임시 string 객체를 복사하는 대신, multiset안에 string을 생성


//logAndAdd는 색인을 이용해 값을 조회하는 함수를 중복 적재한다.
std::string nameFromIdx(int idx); //idx에 해당하는 이름을 돌려줌
void logAndAdd(int idx)
{
	auto now = std::chrono::system_clock::now();
	log(now, "logAndAdd");
	names.emplace(nameFromIdx(idx));
}

//종복적재는 통상적인 예상에 대해서만 예상대로 작동한다 만약 색인값을 담은 short를 넘겨주면 어떻게 될까 ?
short nameIdx;
logAndAdd(nameIdx); //오류

//case1.T를 shor& 연역, case 2.short는 int로 승격하여 호출된다. 승격통한 부합보다 정확한 중복적재 호출
//매개변수 name이 std::froward를 통해서 names의 멤버 함수 emplace에 전달된다.
//이는 std::string에 전달되며, std::string 생성자에 short가 없으므로 생성자 호출이 실패한다.
//보편 참조를 받는 템플릿 함수는 c++에서 가장 욕심 많은 함수이다.
//템플릿의 인스턴스는 거의 모든 형식의 인수와 정확히 부합한다.이러한 문제를 피하는 방법은 완벽 전달 생성자를 작성하는 것이다.

//색인을 받는 자유함수 대신, 같은일을 하는 생성자를 가진 Person 클래스
class Person {
public:
	template<typename T>
	explicit Person(T&& n)
		: name(std::forward<T>(n)) {} //완벽 전달 생성자, 자료멤버를 초기화한다.

	explicit Person(int idx)
		: name(nameFromIdx(idx)) {} //int를 받는 생성자

	//항목17.특정조건에서 c++은 복사생성자와 이동생성자를 자동으로 작성하며,
	//심지어 템플릿화된 생성자가 복사 생성자나 이동 생성자에 해당하는
	//서명으로 인스턴스화되는 경우에도 자동작성된다.
	Person(const Person& rhs); //복사 생성자 ( 컴파일러 작성 )
	Person(Person&& rhs); //이동 생성자 ( 컴파일러 작성 )

private:
	std::string name;

};

Person p("Nancy");
auto cloneOfP(p); //p로부터 새 Person을 생성, 컴파일 실패

//Person객체로부터 새 Person객체를 생성, Person의 std::string 자료멤버를 Person 객체(p)로 생성하려 하는데, 
//std::string에는 Person을 받는 생성자가 없으므로 컴파일러는 오류를 발생시킨다.
//컴파일러의 추론은 cloneOfP는 const가 아닌 왼값(p)으로 초기화되며, 
//따라서 템플릿화된 생성자를 Person 형식의 비const 왼값을 받는 형태로 인스턴스화 할 수 있다.

//인스턴스화가 끝난 Person
class Person {
public:
	explicit Person(Person& n) //완벽 전달 템플릿에서 인스턴스화됨
		: name(std::forward<Person&>(n)) {}
};
//p는 복사 생성자에게 전달될수도, 인스턴스화된 템플릿에 전달될수도 있다.
//하지만 복사 생성자를 호출하려면 p에 const를 추가해야 한다.
//인스턴스화의 경우 아무것도 추가하지 않아도 된다.즉 중복적재가 부합하다.
auto cloneOfP(p);


const Person cp("Nancy"); //객체가 const
auto cloneOfP(cp); //복사 생성자를 호출

class Person {
public:
	explicit Person(const Person& n); //템플릿에서 인스턴스화됨
	Person(const Person& rhs); //복사 생성자 (컴파일러가 생성)
};

//복사할 객체가 const이므로 복사 생성자가 받는 매개변수와 정확히 부합한다.
//그러나 중복적재 해소 결과가 달라지지는 않는다.
//c++의 중복적재 해소 규칙 중에는 어떤 함수 호출이 템플릿 인스턴스와 비템플릿 함수에 똑같이 부합한다면 보통 함수를 우선시한다는 규칙이 있다.
//그래서 복사 생성자가 자신과 같은 서명을 가진 템플릿 인스턴스를 물리치고 호출대상으로 선택된다.

//상속이 관여하는 클래스에서는 완벽 전달 생성자와 컴파일러가 작성한 복사, 이동 연산들 사이의 상호작용의 여파가 더욱 크다.

class SpecialPerson : public Person {
public:
	SpecialPerson(const SpecialPerson& rhs) //복사 생성자
		: Person(rhs) //기반 클래스의 완벽, 전달 생성자를 호출
	{
	}
	SpecialPerson(SpecialPerson&& rhs) //이동 생성자
		: Person(std::move(rhs)) //기반 클래스의 완벽 전달 생성자를 호출
	{
	}
};

//결론은 보편 참조 매개변수에 대한 중복 적재가 가능한한 피해야 한다.


//- 보편 참조에 대한 중복적재는 거의 항상 보편 참조 중복 적재 버전이 예상보다 자주 호출되는 상황으로 이어진다.
//- 완벽 전달 생성자들은 특히나 문제가 많다.그런 생성자는 대체로 비const 왼값에 대한 복사 생성자보다 더 나은 부합이며, 
//  기반 클래스 복사 및 이동 생성자들에 대한 파생 클래스의 호출들을 가로챌 수 있기 때문이다.