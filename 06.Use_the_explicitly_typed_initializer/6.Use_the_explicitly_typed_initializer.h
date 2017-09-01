//auto가 원치 않은 형식으로 연역될 때에는 명시적 형식의 초기치를 사용하라

#pragma once

#include <vector>
//항목5 auto를 사용하여 변수를 선언하면 많은 장점이 있다.하지만 auto의 형식 연역이 가끔 예상치 못한 결과

//Widget을 받아 vector<bool>을 돌려주는 함수
std::vector<bool> features(const Widget& w);

//5번째 비트는 Widget의 우선순위를 나타낸다.
Widget w;
bool highPriority = features(w)[5]; //w의 우선순위 높은가?
processWidget(w, highPriority);

//명시적 형식 -> auto 사용
auto highPriority = features(w)[5]; // 미정의 행동
//auto를 사용하는 heightPriority는 bool이 아니다.
//std::vector<bool>의 operator[]가 돌려주는 것은 그 컨테이너의 한 요소에 대한 참조가 아니라,
//std::vector<bool>::reference 형식의 객체이다.


bool highPriority = features(w)[5]; //정상
//features는 std::vector<bool>을 돌려주며, 그 객체에 대한 operator[]가 호출된다. 
//operator[]는 std::vector<bool>::reference객체를 돌려주며, 
//그 객체가 암묵적으로 bool로 변환되어서 heightPriority초기화에 쓰이며, 
//결과적으로 std::vector<bool>의 5번 비트의 값을 가지게 된다.

auto highPriority = features(w)[5]; //예상못한 값
//이번에도 features는 std::vector<bool>객체를 돌려주며, 
//그 객체에 대해 operator[]가 호출되고 그 operator[]가 std::vector<bool>::reference객체를 돌려준다.
//이후 auto에 의해 hightPriority의 형식이 연역된다.


// 예상치 못한 결과가 도출되는 이유
//features 호출은 임시 std::vector<bool> 객체를 돌려주며, 
//이 임시객체 temp에 대해 호출된 operator[]는 std::vector<bool>::reference 객체를 돌려주며, 
//그 객체에는 temp가 관리하는 비트들을 담은 자료구조의 한 워드를 가리키는 포인터와 
//그 워드에서 참조된 비트에 해당하는 비트 오프셋이 담겨 있다.
//따라서 highPriority 역시 temp 안의 해당 워드를 가리키는 포인터와 5번 비트의 오프셋을 담게된다.
//그런데 temp는 임시객체로 문장의 끝에서 파괴되며, 결과적으로 highPriority의 포인터는 댕글링 포인터가 된다.


// 대리자 클래스
// std::shared_ptr -> 드러난
// std::vector<bool>::reference -> 은밀한

// 대리자 형식 추론
// 헤더파일, 컴파일문제 디버깅, 단위 검사 디버깅...


//결론
//std::vector<bool>::reference처럼 '보이지 않는' 대리자 클래스는 auto와 잘 맞지 않는다.
//따라서 이 문제를 해결하기 위해서는 auto가 다른 형식을 연역하도록 강제하는 것이다.형식 명시 초기치 관용구가 그러한 해법의 하나이다.
//형식 명시 초기치 관용구에서는 변수를 auto로 선언하되, 초기화 표현식의 형식을 auto가 연역하길 원하는 형식으로 캐스팅한다.
Matrix sum = m1 + m2 + m3 + m4;
Sum<Sum<Sum<Matrix, Matrix>, Matrix>, Matrix>;
auto sumMatrix = static_cast<Matrix>(m1 + m2 + m3 + m4);

auto hightPriority = static_cast<bool>(features(w)[5]);


//- "보이지 않는" 대리자 형식 때문에 auto가 초기화 표현식의 형식을 "잘못" 연역 할 수 있다.
//- 형식 명시 초기치 관용구는 auto가 원하는 형식을 연역하도록 강제한다.