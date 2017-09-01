// std::move�� std::forward�� �����϶�.

// std::move�� ��� ���� �̵������� �ʴ´�.
// std::forward�� ��� ���� ���������� �ʴ´�.
// ����������� �� �� �ƹ��͵� ���� �ʴ´�.
// std::move�� std::forward�� �׳� ĳ������ �����ϴ� �Լ��̴�.

// std::move�� �־��� �μ��� ������ ���������� ĳ����
// std::forward�� Ư�������� ������ ������ ������ ĳ����(���Ǻ� ĳ����)
#include <type_traits>
#include <string>

template<typename T>
typename std::remove_reference<T>::type&& move(T&& param)
{
	using ReturnType = typename remove_reference<T>::type&&;

	return static_cast<ReturnType>(param);
}
// ��ü�� ���� ����(��Ȯ�� ���ϸ� ���� ����: �׸�24)�� �޾Ƽ� ���� ��ü�� ���� � ������ �����ش�.
// �Ϲ������� ������������ �����ֳ�, ���� T�� ���� �ް� �����̸� T&&�� �ް� ������ �ȴ�.(�׸� 28)
// �̸� �����ϱ� ����, �� ������ T�� ���� Ư�� std::remove_reference�� ����
// ��ȯ ������ &&�� �׻� ������ �ƴ� ���Ŀ� ����ȴ�. ��������� std::move�� �ݵ�� ������ ������ �����ش�.
// std::move�� �ڽ��� �μ��� ���������� ĳ�����Ѵ�.

// C++14 ����
template<typename T>
decltype(auto) move(T&& param)
{
	using ReturnType = remove_reference_t<T>&&;
	return static_cast<ReturnType>(param);
}

// std::move�� ĳ������ ����������, �̵��� �������� �ʴ´�.
// std::move��� �̸��� �̵��� �� �ִ� ��ü�� �� �� ���� �����ϱ� ���� �Լ�

// �������� �̵��� �ĺ��� �ƴ� ���
class Annotation {
public:
	explicit Annotation(const std::string text)
		: value(std::move(text)) {}	// ������ �Ű����� �� ���޷� ����(�׸�41) -> �������� ġ���� �ʱ����� std::move ����

private:
	std::string value;
};
// Annotation �������� ��� �ʱ�ȭ ��Ͽ��� std::move(text)�� ����� const std::string������ �������̴�.
// �� �������� std::string�� �̵������ڿ� ���� �� �� ����.
// �ֳ��ϸ� �� �̵������ڴ� const�� �ƴ� std::string�� ���� ������������ �ޱ� �����̴�. �׷��� �� �������� ���� �����ڿ� ���� �� ���� �ִ�.
// const�� ���� �ް� ������ const �������� ���� ���� ���Ǳ� �����̴�.

// 1. �̵��� ������ ��ü�� const�� �������� ���ƾ� �Ѵ�. const ��ü�� ���� �̵� ��û�� �Ҹ� ���� ���� �������� ��ȯ
// 2. std::move�� �ƹ��͵� ������ �̵����� ���� �Ӹ� �ƴ϶�, ĳ���õǴ� ��ü�� �̵� �ڰ��� ���߰�  �ȴٴ� ���嵵 ���� ����.
// Ȯ���� ���� ��ü�� std::move�� ������ ����� �ϳ��� �������̶�� �ͻ�.
#include <chrono>
void makeLogEntry(const std::string& msg, const std::chrono::system_clock::time_point& time)	{}

class Widget{};
void process(const Widget& lvalArg) {}
void process(Widget&& rvalArg) {}
template<typename T>
void logAndProcess(T&& param)
{
	auto now = std::chrono::system_clock::now();
	makeLogEntry("Calling 'process'", now);
	process(std::forward<T>(param));
	// std::forwrad�� �־��� �μ��� ���������� �ʱ�ȭ �� ���� ������ �װ��� ���������� ĳ�����Ѵٴ� ������ ���Ǻ� ĳ����
	// ������� �ʱ�ȭ �Ǿ��ִ����� ������ logAndProcess�� ���ø� �Ű����� T�� ��ȣȭ(encoding)�Ǿ� �ִ�.
	// �� �Ű������� std::forward�� ���޵Ǹ�, std::forward�� �ű⼭ �ش������� �����Ѵ�.(�׸�28)
}

int main()
{
	Widget w;
	logAndProcess(w);
	logAndProcess(std::move(w));
}

// std::move�� �ŷ��� ����ϱ� ���ϰ�, ������ ������ �پ���, �ڵ��� ��Ȯ���� ��������.
// � Ŭ������ �̵� �����ڰ� ȣ��� Ƚ���� �����ϰ� �ʹٸ�, �׳� �̵� ���� ���߿� Ŭ���� ����(static) ī���� ������ ����
// Ŭ������ ������ �ڷ� ����� std::string �ϳ����̶� �� ��, �̵������� ���� ���
class WidgetMove {
public:
	WidgetMove(WidgetMove&& rhs) : s(std::move(rhs.s)) { ++moveCtorCalls; }

private:
	static std::size_t moveCtorCalls;
	std::string s;
};

class WidgetForward {
public:
	WidgetForward(WidgetForward&& rhs) : s(std::forward<std::string>(rhs.s)) { ++moveCtorCalls; }

private:
	static std::size_t moveCtorCalls;
	std::string s;
};

// ù ������ std::move������ �Լ� �μ�(rhs.s)�� �����ϸ� �Ǿ�����, ��° ������ std::forward������
// �Լ� �μ�(rhs.s)�� ���ø� ���� �μ�(std::string) �� �� �����ؾ� ������ �ָ�
// �׸��� std::forward�� �����ϴ� ������ �ݵ�� ������ �ƴϾ�� �Ѵٴ� ���� �ָ�(�׸�28)
// std::move���� std::forward���� Ÿ�ڷ��� ����, �����ϴ� ���� �������̶�� ������ ��ȣȭ�ϴ� �����μ��� �����ϴ� ���ŷο� ����.
// ���� �߸��� ������ �����ϴ� �Ǽ��� ������ ������ ����. (std::string&�� �����ϸ� �ڷ� ���s�� �̵������� �ƴ϶� ��������ȴ�.)

// std::move�� ����Ѵٴ� ���� �־��� �μ��� ������ ���������� ĳ�����ϰڴٴ� ��. �ϳ��� �̵��� �غ�
// std::forward�� �������� ���� ������ ���������� ĳ�����ϰڴٴ� ��. �� ��ü�� ������ �ް� �Ǵ� ������ ������ ������ ä�� �ٸ� �Լ��� �׳� �Ѱ��ִ� ����

// 1. std::move�� ������������ ������ ĳ������ �����Ѵ�. std::move ��ü�� �ƹ��͵� �̵����� �ʴ´�.
// 2. std::forward�� �־��� �μ��� �������� ���� ��쿡�� �װ��� ���������� ĳ�����Ѵ�.
// 3. std::move�� std::forward �� ��, ������������� �ƹ� �ϵ� ���� �ʴ´�.