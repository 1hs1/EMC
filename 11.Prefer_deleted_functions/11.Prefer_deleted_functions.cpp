// 정의되지 않은 비공개함수보다 삭제된 함수를 선호하라
// C++이 필요에 따라 자동으로 작성하는 멤버함수들이 있다.(항목17)

// C++98에서는 사용을 금지하려는 멤버 함수가 복사 생성자 또는 복사 배정 연산자였다.
// 방법 : private로 선언하고 정의하지 않는다.
template <class charT, class traits = char_traits<chatT>>
class basic_ios98 : public ios_base {
public:
private:
	basic_ios98(const basic_ios98&);			// not defined
	basic_ios98& operator=(const basic_ios98&);	// nod defined
};

//  C++11에서는 사용을 금지하려는 멤버함수 선언 끝에 "=delete"를 붙이는 것이다. (삭제된 함수)
template <class charT, class traits = char_traits<chatT>>
class basic_ios11 : public ios_base {
public:
	basic_ios11(const basic_ios11&) = delete;
	basic_ios11& operator=(const basic_ios11&) = delete;
};
// 멤버 함수나 friend함수에서 basic_ios11객체를 복사하려 하면 컴파일이 실패한다.
// 이는 부적절한 용례가 링크 시점에 가서야 발견되는 C++98방식에 비해 개선된 사항이다.

// 삭제된 함수는 private가 아니라 public로 선언
// C++은 먼저 그 함수의 접근성을 점검한 후에야 삭제 여부를 점검. 더 나은 오류메시지를 보기 위해서...

// 삭제된 함수의 또다른 장점 : 그 어떤 함수에도 삭제할 수 있다.
// 반면 private은 멤버 함수에만 적용할 수 있다.
bool isLucky(int number);		// 원래 구현하고자하는 함수(숫자만 입력 가능)
bool isLucky(char) = delete;	// char 배제	// if(isLucky('a'))...	오류
bool isLucky(bool) = delete;	// bool 배제	// if(isLucky(true)...	오류
bool isLucky(double) = delete;	// double와 float 배제	// if(isLucky(3.5f))... 오류

// 삭제된 함수로 원치 않는 템플릿 인스턴스화를 방지할 수 있다
template<typename T>
void processPointer(T* ptr);
// void* 역참조나 증가, 감소가 아예 불가능하다.
// char* C 스타일 문자열을 나타낸다.
// 이러한 포인터들은 특별한 처리가 필요한 경우가 많은데, 호출을 거부하기 위한 방법으로 delete를 사용한다.
template<>
void processPointer<void>(void*) = delete;

template<>
void processPointer<char>(char*) = delete;

// 클래스 안의 함수 템플릿의 일부 인스턴스화를 방지하려는 목적으로 private선언은 적용 안됨
class Widget {
public:
	template<typename T>
	void processPointer(T* ptr) {}

private:
	template<>							// 오류
	void processPointer<void>(void*);	
};

// private으로 선언하되 정의를 생략하는 C++98 방식은 클래스 바깥에서는 작동하지 않으며, 클래스 안에서도 항상 작동하지는 않는다.
// 작동한다고 해도 링크시점에 가서야 작동하는 경우도 있다.

int main(){}

// 정의되지 않은 비공개 함수보다 삭제된 함수를 선호하라.
// 비멤버 함수와 템플릿 인스턴스를 비롯한 그 어떤 함수도 삭제할 수 있다.