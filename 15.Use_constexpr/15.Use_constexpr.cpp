// �����ϸ� �׻� constexpr�� ����϶�
// constexpr�� ��� ���� ���� ����� �Ӹ� �ƴ϶� ������ �������� �˷�����.

#include <array>

// 1. constexpr�� ��ü�� ����� ���
// ��ü�� ������ const, �� ���� ������ ������ �������� �˷�����.
// ������ ������ �˷��� ������ Ư���� ���� :	�б� ���� �޸𸮿� ��ġ�� �� �ִ�. (������ �ý��� ������)
//												���� ��� ǥ������ �䱸�Ǵ� ���ƿ� ���
//												(�迭 ũ��, ���� ���ø� �μ�, ������ ��)

int sz1;	// �� constexpr ����
//constexpr auto arraySize1 = sz1;	// ����! sz1�� ���� ������ ���߿� �˷����� ����
//std::array<int, sz1> data1;		// ����!
constexpr auto arraySize2 = 10;		// 10�� Ȯ���� ������ ���� ���
std::array<int, arraySize2> data2;	// arraySize2�� constexpr ��ü

// const�� constexpr������ ������ ������ �������� ����. 
// const ��ü�� �ݵ�� ������ �������� �˷����� ������ �ʱ�ȭ ���� ����
int sz2;
const auto arraySize = sz2;		// arraySize�� sz�� const ���纻
//std::array<int arraySize> data;	// ����! arraySize�� ���� ������ �������� �˷����� ���� 

// ��� constexpr ��ü�� const������ ��� const ��ü�� constexpr�� �ƴϴ�.

// 2. constexpr�� �Լ��� ����� ���
// ������ ���� ����� �μ��� �ؼ� ȣ�ص� ���, ������ ���� ����� ����.
// ��������� �Ǿ�� �˷����� ������ ȣ���ϸ� ������� ���� ����.
constexpr
int powO(int base, int exp) noexcept { return 0; }	// pow�� ���� ���ܸ� ������ �ʴ� constexpr �Լ�
constexpr auto numConds = 5;						// ���ǵ��� ����
std::array<int, powO(3, numConds)> results;			// result�� 3^numConds ���� ��ҵ��� ��´�.
// pow�տ� constexpr�� �ִٰ� �ؼ� pow�� �ݵ�� const ���� �����ִ� ���� �ƴϰ�,
// ���� base�� exp�� ������ ���� ����� ������ pow�� ����� ������ ���� ����� ����� �� �ִ�.
// base�� exp�� �ϳ��� ������ ���� ����� �ƴϸ� pow�� ����� ����������� ���� �� �ִ�.

int readFromDB(std::string const& name) { return 0; }
auto base = readFromDB("base");		// ����������� ������ ���Ѵ�.
auto exps = readFromDB("exponent");
auto baseToExp = powO(base, exps);	// ����������� pow�Լ��� ȣ��

// constexpr �Լ��� ������ ���� ����� ȣ������ �� �ݵ�� ������ ���� ����� ������ �� �־�� �ϹǷ�, ������ ������ ������ ����
// C++11 ���� : ���� ���� ������ ���ƾ� �ϳ�, return ��
constexpr int pow11(int base, int exp) noexcept
{
	return (exp == 0 ? 1 : base * pow11(base, exp - 1));	// ���׿�����"?:", ��� ���
}

// C++14 ���� : ���� ���� ���� ������ �������� // VS2015������ �ȵ�
//constexpr int pow14(int base, int exp) noexcept
//{
//	int result = 1;
//	for (int i = 0; i < exp; ++i) result *= base;
//
//	return result;
//}
//https://msdn.microsoft.com/ko-kr/library/hh567368.aspx

// constexpr �Լ��� �ݵ�� ���ͷ� ���ĵ��� �ް� ������� �Ѵ�.(������ ���߿� ���� ������ �� �ִ� ����)
// C++11���� void�� ������ ��� ���� ������ ���ͷ� ����
// �����ڿ� ������ ����Լ����� constexpr�� ����� ���� ���ĵ� ���ͷ� ������ �� �� �ִ�.

class Point {
public:
	constexpr Point(double xVal = 0, double yVal = 0) noexcept
		: x(xVal), y(yVal) {}

	constexpr double xValue() const noexcept { return x; }
	constexpr double yValue() const noexcept { return y; }

	void setX(double newX) noexcept { x = newX; }
	void setY(double newY) noexcept { y = newY; }
	//  C++11 ������ constexpr ����Լ��� �Ϲ������� const�� �����Ͽ�, ��� ��ü�� ������ �� ����.
	// ��ȯ������ void �ε�, C++11���� void�� ���ͷ� ������ �ƴϴ�.
	// C++14 ������ ���� �� ������ �������.

private:
	double x, y;
};

// Point�� ��ȸ �Լ����� ȣ���� ������ �� �ٸ� constexpr ��ü�� �ʱ�ȭ�ϴ� constexpr �Լ��� �ۼ��� �� �ִ�.
constexpr
Point midPoint(const Point& p1, const Point& p2) noexcept
{
	// constexpr ��� �Լ����� ȣ��
	return {(p1.xValue() + p2.yValue()) / 2, (p1.yValue() + p2.yValue()) / 2};
}

// C++14 ���������� ����
//constexpr Pointreflection(const Point& p) noexcept
//{
//	Point result;
//	result.setX(-p.xValue());
//	result.setY(-p.yValue());
//
//	return result;
//}

int main()
{
	constexpr Point p1(9.4, 27.7);	// constexpr �����ڰ� ������ �������� �����
	constexpr Point p2(28.8, 5.3);

	constexpr auto mid = midPoint(p1, p2);	// constexpr �Լ��� ����� �̿��ؼ� constexpr ��ü�� �ʱ�ȭ

	// C++14������ ����
	//constexpr auto reflectedMid = Pointreflection(mid);
}



// 1. constexpr ��ü�� const�̸�, ������ ���߿� �˷����� ����� �ʱ�ȭ �ȴ�.
// 2. constexpr �Լ��� �� ���� ������ ���߿� �˷����� �μ���� ȣ���ϴ� ��쿡�� ������ ���� ����� �����Ѵ�.
// 3. constexpr ��ü�� �Լ��� ��constexpr ��ü�� �Լ����� �������� ���ƿ��� ����� �� �ִ�.
// 4. constexpr ��ü�� �Լ��� �������̽��� �Ϻ��̴�.