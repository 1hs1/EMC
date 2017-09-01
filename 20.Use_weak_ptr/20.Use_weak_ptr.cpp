
//std::shared_ptr처럼 작동하되 대상을 잃을 수도 있는 포인터가 필요하면 std::weak_ptr를 사용하라.

#include<memory>
#include<unordered_map>

class Widget {};

using WidgetID = int32_t;
std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
	static std::unordered_map<WidgetID, std::weak_ptr<const Widget>> cache;

	auto objPtr = cache[id].lock(); //objPtr은 캐시 객체를 가리키는 std::shared_ptr

	if (!objPtr) {
		//objPtr = loadWidget(id); //캐시에 없으면 적재하고 캐시에 저장
		cache[id] = objPtr;
	}
	return objPtr;
}

int main()
{
	//자신이 가리키게 되어 있는 객체의 존재 유무를 검출할 수있는 포인터가 std::weak_ptr이다.
	//std::weak_ptr은 std::shared_ptr를 보강하는 포인터이다.std::weak_ptr은 참조횟수에 영향을 미치지 않는다.
	
	auto spw = std::make_shared<Widget>(); //spw 생성 후 참조횟수 1
	std::weak_ptr<Widget> wpw(spw); //wpw는 spw와 같은 참조횟수 1
	spw = nullptr; //참조가 0, wpw는 대상을 잃는다.

	if (wpw.expired()) {} //wpw가 객체를 가르키지 않으면..

	//expired 호출과 역참조 사이에서 다른 어떤 스레드가 해당 객체를 가리키는 마지막 std::shared_ptr를 재배정 혹은 파괴할 수 있다.
	//때문에 std::weak_ptr의 만료 여부를 점검하고 아직 만료되지 않았으면, 피지칭 객체에 대한 접근을 돌려주는 연산을 하나의 원자적 연산으로 수행한다.

	//생성방법은 이미 만료된 std::weak_ptr로부터 std::shared_ptr를 생성했을 때의 행동방식에 따라 두 가지로 나뉜다.

	//case1. std::weak_ptr::lock을 사용하는 것으로, 이는 std::shared_ptr객체를 돌려주며, 만료되었을 시, null을 돌려준다.
	std::shared_ptr<Widget> spw1 = wpw.lock(); //wpw가 만료이면 spw1은 null
	auto spw2 = wpw.lock(); //위와 동일. auto 사용

	//case2., std::weak_ptr를 인수로 받는 std::shared_ptr 생성자를 사용하는 것.이 경우, std::weak_ptr가 만료되었다면 예외가 발생
	std::shared_ptr<Widget> spw3(wpw); //wpw가 만료면 bad_weak_ptr발생

	//std::weak_ptr이 유용하게 사용되는 경우를 살펴보자.
	//첫 번째
	//어떤 팩토리 함수, 주어진 고유 ID에 해당하는 읽기 전용 객체를 가리키는 포인터를 돌려줌
	//std::unique_ptr<const Widget> loadWidget(WidgetID id);

	//여기서 loadWidget은 비용이 클 경우, 이를 해결하기 위해 호출 결과들을 캐싱하는 함수를 작성해야 한다.
	//또한 쓰이지 않으면 삭제하며, 반환 형식인 std::unique_ptr는 바람직하지 않다.캐시에 있는 포인터들은 자신이 대상을 잃었음을 검출해야 한다.
	//즉, 대상을 잃었음을 감지할 포인터 std::weak_ptr이어야 한다.	
	//loadWidget의 캐싱 버전
	WidgetID wid = 1;
	auto w = fastLoadWidget(wid);
	
	//두 번째, Observer설계 패턴, 관찰자들을 가리키는 std::weak_ptr들의 컨테이너를 자료 멤버로 두는 것이다.
	//예로, 객체 a, b, c로 이루어진 자료구조에서, a와 c가 b의 소유권을 공유하는, 
	//그래서 b를 가리키는 std::shared_ptr을 가진 상황에서 b에서 다시 a를 가리키는 포인터가 필요한 경우

	//선택지는 3종류이다.
	//- 생포인터 : 포인터가 파괴되는 경우 미정의 상황 발생
	//- std::shared_ptr : a가 b를 가리키고, b는 a를 가리키는 순환고리로 인해 a, b둘다 파괴되지 못하며, a, b에 접근할 수 없다.
	//- std::weak_ptr : 위 2 문제를 모두 해결한다.

	//std::weak_ptr과 std::shared_ptr은 본질적으로 동일하며, 그 크기또한 같다.std::weak_ptr는 객체의 소유권 공유에 참여하지 않으며, 따라서 피지칭 객체의 참조횟수에 영향을 미치지 않는다.제어 블록에는 '두 번째' 참조 횟수가 있으며, 그것이 std::weak_ptr이다.

	return 0;
}

//- std::shared_ptr처럼 작동하되 대상을 잃을 수도 있는 포인터가 필요하면 std::weak_ptr을 사용하라.
//- std::weak_ptr의 잠재적인 용도로는 캐싱, 관찰자 목록, 그리고 std::shared_ptr 순환고리 방지가 있다