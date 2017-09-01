// 이동이 저렴하고 항상 복사되는 복사 가능 매개변수에 대해서는 값 전달을 고려하라

#include <vector>

namespace base {
	class widget {
	public:
		void addName(const std::string& newName) { names.push_back(newName); }
		void addName(std::string&& newName) { names.push_back(std::move(newName)); }
	private:
		std::vector<std::string> names;
	};
	// 단점 : 함수를 두개 작성, 목적코드에도 함수가 두개 존재(파일 크기 증가)
	// 대안 : 함수 템플릿 작성(보편참조)
	// 비용 : 왼값(복사1회), 오른값(이동1회)
}

namespace functemplate {
	class widget {
	public:
		template<typename T>
		void addName(T&& newName)
		{
			names.push_back(std::forward<T>(newName));
		}
	private:
		std::vector<std::string> names;
	};
	// 단점 : 목적코드에는 이 템플릿의 서로 다른 인스턴스가 여러개 포함될 수 있다. (항목25)
	// 보편 참조로는 전달할 수 없는 인수 형식들이 존재(항목30)
	// 클라이언트가 부적절한 형식의 인수를 전달하면 컴파일러가 난해한 오류 메시지를 출력(항목27)
	// 대안 : 값 전달
	// 비용 : 왼값(복사1회), 오른값(이동1회)
}

namespace passvalue {
	class widget {
	public:
		void addName(std::string newName)
		{
			names.push_back(std::move(newName));
		}
	private:
		std::vector<std::string> names;
	};
	// 비용 : 왼값(복사1회, 이동1회), 오른값(이동2회)
}

// 제목 문구 분석
// 1. 고려하라 : 값 전달 방식은 중복적재나 보편참조보다 비용이 크다.
// 2. 복사 가능 매개 변수 : 복사할 수 없는 매개 변수는 이동 전용형식(std::unique_ptr), 중복적재에서 왼값 인수를 위한 중복적재를 따로 만들 필요 없음.
// 3. 이동이 저렴한 : 이동이 한번 더 일어나기 때문에
// 4. 항상 복사되는 : ?

int main() {}

// 1. 이동이 저렴하고 항상 복사되는 복사 가능 매개변수에 대해서는 값 전달이 참조 전달만큼이나 효율적이고,
// 구현하기가 더 쉽고, 산출되는 목적 코드의 크기도 더 작다.
// 2. 왼값 인수의 경우 값 전달(즉, 복사 생성) 다음의 이동 배정은 참조 전달 다음의 복사 배정보다 훨씬 비쌀 가능성이 있다.
// 3. 값 전달에서는 잘림 문제가 발생할 수 있으므로, 일반적으로 기반 클래스 매개 변수 형식에 대해서는 
// 값 전달이 적합하지 않다.