#pragma once
// 삽입 대신 생성 삽입을 고려하라

// std::string객체를 담는 삽입 함수(insert, push_front, push_back, std::forward_list 경우 insert_after)에
// 넘겨주는 요소 형식은 std::string이어야 하지만 아닌 경우도 있다.
std::vector<std::string> vs;
vs.push_back("xyzzy"); // 문자열 리터럴

// push_back 중복적재
template <class T,
	class Allocator = allocator<T>>
	class vector {
	public:
		void push_back(const T& x);
		void push_back(T&& x);
};


// 컴파일러 (임시 객체 생성 )
vs.push_back("xyzzy");  -> vs.push_back(std::string("xyzzy"));

// 1. "xyzzy" 부터 임시 std::string 객체 생성 : temp라 칭하고 임시 객체이므로 오른값
// 2. temp가 push_back의 오른값 중복적재로 전달. temp는 오른값 참조 매게변수 x에 묶인다.
//	  std::vector를 위한 메모리 안에서 x의 복사본이 생성된다. (복사하는 데 쓰인 생성자는 이동 생성자)
//		이때 두번째 std::string 생성 std::vector안에 실제 새 객체 생성
// 3. push_back이 반환된 즉시 temp 파괴 std::string 소멸자가 실행

// 주어진 인수를 이용해 std::vector안에서 직접 std::string을 생성
vs.emplace_back("xyzzy");
// push_back : emplace_back
// push_front : emplace_front
// insert : emplace ( std::forward_list, std::array 제외 )
// 연관 컨테이너 insert : emplace_hint
// forward_list intsert : emplace_after
// 생성 삽입 함수는 삽입 함수보다 유연 :삽입 함수는 삽입 객체를 받지만 생성 삽입 함수들은 삽입할 객체의 생성자를 위한 인수들을 받는다.
// 때문에 생성 삽입은 임시 객체의 생성과 파괴를 피한다

// 컨테이너에 담긴 요소의 형식과 동일한 형식의 인수 ( 복사 생성 또는 이동 생성 )
std::string queenOfDisco("Donna Summer");
vs.push_back(queenOfDisco); // 복사 생성
vs.emplace_back(queenOfDisco); // 복사 생성

// 거의 항상 생성 삽입이 삽입의 성능을 능가하는 경우
// 세가지 모두 충족
// 1. 추가할 값이 컨테이너에 배정되는 것이 아니라 컨테이너 안에서 생성된다.
// 2. 추가할 인수 형식(들)이 컨테이너가 담은 형식과 다르다.
// 3. 컨테이너가 기존 값과의 중복 때문에 새 값을 거부할 우려가 별로 없다.

// 생성 삽입 함수를 사용 결정시 고려 사항
// 1. 자원 관리
std::list<std::shared_ptr<Widget>> ptrs;
void killWidget(Widget* pWidget);
ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
ptrs.push_back({ new Widget, killWidget });
// 1-1. 두 호출 모두, 'new Widget'으로 만들어진 생 포인터를 담는 임시 std::shared_ptr<Widget> 객체 생성( temp )
// 1-2. push_back은 그 temp를 참조로 받는다. temp은 복사본을 담을 목록 노드를 할당하는 도중에
// 메모리 부족 예외가 발생한다.
// 1-3. 예외가 push_back 바깥으로 전파되면서 temp가 파괴된다. temp는 자신이 관리하는
// Widget을 참조하는 마지막 std::shared_ptr 객체였으므로, temp가 파괴되면서
// 해당 Widget도 자동으로 해제( killWidget 호출에 의해 )
// 메모리 누수 없음

ptrs.emplace_back(new Widget, killWidget);
// 1-1. 'new Widget'으로 만들어진 생 포인터가 emplace_back으로 완벽 전달.
// emplace_back은 새 값을 담을 목록 노드를 할당한다. 그럼데 할당 실패하고 메모리 부족 예외 발생한다.
// 1-2. 'emplace_back' 밖으로 전파되면서, 힙에 있는 Widget 객체에 도달하는 유일한 수단인 생 포인터가 사라진다.
// 결국 그 Widget이 그리고 그 객체가 소유한 모든 자원이 샌다 
// 메모리 누수

// 자원 관리 객체를 애초에 'new Widget'을 전달하려 했던 함수에 오른값으로 전달해야한다. (항목 21)
std::shared_ptr<Widget> spw(new Widget, killWidget);
ptrs.push_back(std::move(spw));

std::shared_ptr<Widget> spw(new Widget, killWidget);
ptrs.emplace_back(std::move(spw));

// 2. explicit 생성자들과의 상호 작용 방식이다.
std::vector<std::regex> regexes;

regexes.emplace_back(nullptr); // 컴파일 ok ( 미정의 )
// std::regex 객체로 변환할 무엇이 아니라 std::regex 객체의 생성자에 전달한 인수.
// std::regex r(nullptr)

std::regex r = nullptr; // 컴파일 error

regexes.push_back(nullptr); // 컴파일 error

std::regex upperCaseWord("[A-Z]+");
// const char* 받는 std:::regex 생성자는 explicit로 선언

// 복사 초기화
std::regex r1 = nullptr; // 컴파일 error
// 직접 초기화
std::regex r2(nullptr); // 컴파일 ok ( 미정의 )

// explicit 생성자 : 복사 초기화 사용 불가, 직접 초기화 사용

// 생성 함수를 사용할 때에는 제대로 된 인수를 넘겨주어야 함

// - 이론적으로, 생성 삽입 함수들은 종종 해당 삽입 버전보다 더 효율적이어야 하며,
// 덜 효율적인 경우는 절대로 없어야 한다.
// - 실질적으로, 만일 (1) 추가하는 값이 컨테이너로 배정되는 것이 아니라 생성되고,
// (2) 인수 형식(들)이 컨테이너가 담는 형식과 다르고, (3) 그 값이 중복된 값이어도
// 컨테이너가 거부하지 않는다면, 생성 삽입 함수가 삽입 함수보다 빠를 가능성이 아주 크다.
// - 생성 삽입 함수는 삽입 함수라면 거부당했을 형식 변환들을 수행할 수 있다.
