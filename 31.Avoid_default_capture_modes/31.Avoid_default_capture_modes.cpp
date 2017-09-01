// �⺻ ������ ��带 ���϶�

#include <vector>
#include <functional>
#include <memory>

// C++11�� �⺻ ������ ���� �ΰ���
// 1. �⺻ ���� ������ ��� : ������ ����� ���� ����
// 2. �⺻ �� ������ ��� : ������ ����� �Ҵ� ������ ���� �� ������ �׷��� �ʰ�,
//							�ڱ� �ϰ����� �� ������, �׷��� ���� ��쵵 �ִ�.

// ���� �������� ����ϴ� Ŭ������ ���� ���� �Ǵ� ���ٰ� ���ǵ� �������� �� �� �ִ� �Ű������� ���� ������ ������.
// ���ٿ� ���� ������ Ŭ������ ������ �� ���� ������ �Ű������� ������ ���� ���ӵǸ� Ŭ���� ���� ������ ����� �Ҵ´�.
using FilterContainer = std::vector<std::function<bool(int)>>;
int computeSomeValue1() { return 1; }
int computeSomeValue2() { return 2; }
int computeDivisor(int, int) { return 3; }

void addDivisorFilter()
{
	FilterContainer filters;
	filters.emplace_back([](int value) { return value % 5 == 0; });	// OK!!

	auto calc1 = computeSomeValue1();
	auto calc2 = computeSomeValue2();

	auto divisor = computeDivisor(calc1, calc2);

	filters.emplace_back([&](int value) { return value % divisor == 0; });	// ����!!
	// addDivisorFilter�� filters.emplace_back�� ��ȯ�� ���Ŀ� ��ȯ
	// filters�� �߰��Ǵ� ���� �Լ��� ��ǻ� �̹� ��� ���·� ����
	// ����� �������� ���� ���� �߻�
	// �׷��� �� ���� ǥ������ ��ȿ���� divisor�� ���� �����Ѵٴ� ���� ��Ȯ�� ��Ÿ���ٴ� ����
}

template<typename C>
void workWithContainer(const C& container)
{
	auto calc1 = computeSomeValue1();
	auto calc2 = computeSomeValue2();

	auto divisor = computeDivisor(calc1, calc2);

	using ContElemT = typename C::value_type;	// �����̳ʿ� ��� ��ҵ��� ����

	using std::begin;
	using std::end;
	
	if(std::all_of(begin(container), end(container),				// �� ������ ��� ��Ұ� �־��� ������ �����ϴ����� ���θ� �����ִ� �˰���
		[&](const ContElemT& value) { return value % divisor == 0})
		){ }
	else{}

	if (std::all_of(begin(container), end(container),				// C++14 ����
		[&](const auto& value) { return value % divisor == 0})
		) {
	}
	else {}

	// �� ���ٰ� �ٸ� ���ƿ����� ������ ������ ������ � ���α׷��Ӱ� ���ٸ� �����ؼ� �� ���ƿ� �ٿ����� �� �ִ�.
	// �׷��� �� ���ƿ��� divisor�� Ŭ�������� ���� �Ҹ��Ѵٸ�, ������ ����� �Ҵ� ���� �߻�.
	// ������ ������ ���� divisor�� ������ �м��� ���� �Ѵٴ� ���� �˷��ִ� ��Ʈ�� ���� ���� ������...
}

// addDivisorFilter�� ������ �ذ� ��� : filters.emplace_back([=](int value) {return value % divisor == 0;});
// �⺻ �� ������ ��� ���

// �׷��� �� ���� ������ �߻��� ���� �ִ�.
// �����͸� ������ �������ϸ� �� �����ʹ� ���ٿ� ���� ������ Ŭ���� ������ ����Ǵµ�, 
// ���� �ٱ��� � �ڵ尡 �� �����͸� delete�� �������� �ʴ´ٴ� ������ ������,
// �׷����� �߻��ϸ� ������ ���纻�� ��Ī ����� �Ұ� �ȴ�.

class Widget {
public:
	void addFilter() const;
	void addFilterFix() const;
	void addFilter14() const;
private:
	int divisor;
};
void Widget::addFilter() const {
	FilterContainer filters;
	filters.emplace_back(
		[=](int value) { return value % divisor == 0; }
	);
	// divisor�� ���������� �ƴ϶� Ŭ������ �� �ڷ� ����̹Ƿ� �������� �� ����.
	// �׷����� ���� �⺻ ������ ���(=)�� �����ϸ� �ڵ�� ������ ���� �ʴ´�.
	// divisor�� ��������� �������Ϸ��� ������ ���� ������ ���� �ʴ´�. [divisor]... 
	// this �����ͷ� ���� �� ���ٴ� ������ ����
	// Ŭ������ ��ȿ���� �ش� Widget ��ü�� ���� ���� ����
}

void doSomeWork()
{
	auto pw = std::make_unique<Widget>();
	pw->addFilter();
}
// ���⼭ Widget�� �ı��ȴ�. �� ������ filters�� ����� �Ҵ� �����͸� ���� ���°� �ȴ�.
// ���� �� ������ �ڷ����� ���� ���纻�� ���� �� ���纻�� �������ϸ� �ذ�
void Widget::addFilterFix() const
{
	FilterContainer filters;
	auto divisorCopy = divisor;

	filters.emplace_back(
		[divisorCopy](int value)
	{ return value % divisorCopy == 0; }
	);
}
// ��� �� ���� ����� ����Ѵٸ� �⺻�� ������ ��嵵 �� �۵�.
// ���ʿ� divisor�� �������Ϸ� �ߴµ� ����� this�� �������ǰ� �� �庻���� �⺻ ������ ���

void Widget::addFilter14() const
{
	FilterContainer filters;

	filters.emplace_back(
		[divisor = divisor](int value)
		{ return value % divisor == 0; }
	);
	// �Ϲ�ȭ�� ���� ������ (�Ϲ�ȭ�� ���� ���������� �⺻ ������ ����� ���� ����)
}

// ���� ���� �⺻ ������ ����� �Ǵٸ� ����
// �ش� Ŭ������ �ڱ� �ϰ����̰� Ŭ���� �ٱ����� �Ͼ�� �ڷ��� ��ȭ�κ��� �ݸ��Ǿ� �ִٴ� ���ظ� �θ�
// ���ٰ� ���������� �Ű����� �Ӹ� �ƴ϶� ���� ����� ���� �Ⱓ�� ���� ��ü���� ������ �� ����.
// ���� ������ �̸� ���� �������� ���ǵ� ��ü�� Ŭ����, �Լ�, ���� �ȿ��� static���� ����� ��ü�� �׷� ��ü�� �ش�
// �׷� ��ü�� ���� �ȿ��� ����� ���� ������, ������ �� ���� ����.
// �⺻ �� ������ ����� ǥ��� ��ġ �׷� ��ü�� ��� ������ �ȴٴ� ������ �ش�.

void addDivisorFilter2()
{
	FilterContainer filters;

	static auto calc1 = computeSomeValue1();
	static auto calc2 = computeSomeValue2();

	static auto divisor = computeDivisor(calc1, calc2);

	filters.emplace_back(
		[=](int value)
		{ return value % divisor == 0; }
	);
	// �� ���ٴ� �� � ������ ���� ������ ������� �����Ƿ�, �ƹ��͵� ���������� �ʴ´�.
	// ������ �� ������ �ڵ�� static ���� divisor�� ��Ī�Ѵ�.
	// addDivisorFilter2�� �� ȣ���� ������ divisor�� �����ϸ�, ���� �� �Լ��� ���ؼ� filters�� �߰��� ���ٴ�
	// ������ �ٸ� �ൿ(divisor�� ������ �����ϴ�)�� ���̰� �ȴ�.
	// ���������� �� ���ٴ� divisor�� ������ �������� �Ͱ� ������, �̴� �⺻ �� ������ ��尡 ���ϴ� �ٿ� ���������� ���.
	// ���ʿ� �⺻�� ������ ��带 ������� �ʴ´ٸ� ��ó�� ������ ������ ū �ڵ尡 ������� ���赵 �������.
}


int main() {}

// 1. �⺻ ���� �������� ������ ����� ���� ������ �ִ�.
// 2. �⺻ �� �������� ������(Ư�� this)�� ����� ���� �� ������, ���ٰ� �ڱ� �ϰ����̶�� ���ظ� �θ� �� �ִ�.