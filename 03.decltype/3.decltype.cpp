// decltype�� �־��� �̸��̳� ǥ������ ������ �˷��ش�.
#include <deque>

class Widget{};

const int i = 0;			// decltype(i)�� const int
bool f(const Widget& w) { return true; };	// decltype(w)�� const Widget&
							// decltype(f)�� bool(const Widget&)

struct Point {
	int x, y;		// decltype(Point::x)�� int
};					// decltype(Point::y)�� int

Widget w;			// decltype(w)�� Widget

template<typename T>
class vector {
public:
	T& operator[](std::size_t index) { return a; }
	T a;
};

vector<int> v;	// decltype(v)�� vector<int>

int main()
{
	if (f(w))		// decltype(f(w))�� bool
	{

	}

	if (v[0] == 0)	// decltype(v[0])�� int&
	{

	}
}

template<typename Container, typename Index>
auto authAndAccess1(Container& c, Index i)
-> decltype(c[i])	// ���� ��ȯ ����,
{					// �Լ��� ��ȯ ������ �� ��ġ�� �ƴ϶� �Ű����� ��� ������ ����
	return c[i];	// ��ȯ������ �Ű��������� �̿��ؼ� ����
}					// C++ 14������ �ʿ����

// ���� �߻�
std::deque<int> d;
//authAndAccess1(d, 5) = 10;	// d[5]�� �����ְ�, 10�� d[5]�� �����Ѵ�. ������ �ȵ�
// d[5]�� int&�� �����ֳ�, auto ��ȯ ���� ���� �������� ������ ���ŵǾ� �ᱹ ��ȯ������ int
// �������: ���� �������� auto -> decltype(auto)



const Widget& cw = w;
auto myWidget1 = cw;			// auto ���� ���� : myWidget1�� ������ Widget
decltype(auto) myWidget2 = cw;	// decltype ���� ���� : myWidget2�� ������ const Widget&

// authAndAccess�� ��������
template<typename Container, typename Index>
auto authAndAccess2(Container&& c, Index i)	// �������� �Ű������� �޵��� ����(�׸�24)
-> decltype(/*std::forward<Container>*/c[i])	// C++ 11 ����
{											// �����̳� ���İ� ���� ��ü�� ���� ��. ǥ�ض��̺귯���� ���� ���� ���.
	return /*std::forward<Container>*/c[i];		// ���� �Ű������� ���ؼ��� �� ���޹�� ���, ���������� std::forward ���� (�׸�25)
}														

std::deque<std::string> makeStringDeque()
{
	std::deque<std::string> d;
	return d;
}
auto s = authAndAccess2(makeStringDeque(), 5);	// ����, authAndAccess1�� �Ұ���

int x = 0;	// decltype(x)�� int
			// decltype((x))�� int&		������ ǥ����
decltype(auto) f1()
{
	int x = 0;
	return x;	// f1�� int ��ȯ
}

decltype(auto) f2()
{
	int x = 0;
	return (x);	// f2�� int& ��ȯ (����) ���������� ���� ����
}

// decltype�� �׻� ������ ǥ������ ������ �ƹ� �������� �����Ѵ�.
// ������ T�̰� �̸��� �ƴ� �ް� ǥ���Ŀ� ���ؼ��� �׻� T& ������ �����Ѵ�.
// C++14�� decltype(auto)�� ����
// autoó�� �ʱ�ġ�κ��� ������ ����������, �� ���Ŀ����������� decltype�� ��Ģ�� ����