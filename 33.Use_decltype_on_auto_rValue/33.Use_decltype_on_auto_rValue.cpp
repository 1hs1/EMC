// std::forward�� ���ؼ� ������ auto&& �Ű��������� decltype�� ����϶�

#include <type_traits>

// C++14���� ���� ������ ����� �Ϲ��� ����
// �Ű����� ���� auto�� ����ϴ� ����

int main()
{
	auto f = [](auto x) { return normalize(x); };
	// �� ���ٰ� �����ϴ� Ŭ���� Ŭ������ �Լ� ȣ�� ������

	// ������
	auto fx = [](auto&& x) { return normalize(std::forward<decltype(x)>(x)); };

	// C++14 ���ٰ� �����μ� ����
	auto f14 = [](auto&&... xs)
	{
		return
			normalize(std::forward<decltype<xs>(xs)...);
	};
}

class temp{	// �����Ϸ��� ���� � Ŭ���� �̸�
public:
	template<typename T>
	auto operator() (T x) const { return normalize(x); }
};
// �� �������� ���ٴ� �Ű����� x�� �׳� normalize�� ����(�ް����θ�)�ϱ⸸ �Ѵ�.
// ���� normalize�� �ް� ������ �ٸ��� ó���Ѵٸ� ������ �ʿ�(main ����)

//  C++14�� std::forward ����
template<typename T>
T&& forward(std::remove_reference_t<T>& param)
{
	return static_cast<T&&>(param);
}
// T�� ������ ���� �������� std::forward�� �ν��Ͻ�ȭ�� ����� ������ �������� �ν��Ͻ�ȭ�� ����� ����.
// ������ �Ű����� x�� �־��� �μ��� �������� �� decltype(x)�� ������ ���������� ����
// �ް��̵� �������̵� decltype(x)�� std::forward�� �Ѱ��ָ� �츮�� ���ϴ� ����� ���´�.
// ���ϴ� ��� : std::forward ȣ��� ������ �μ��� �ް����� ��Ÿ���� ���ؼ��� ���� �������� �μ��� ���,
// ���������� ��Ÿ���� ���ؼ��� ������ ���� �μ��� ���(�׸�28)


// std::forward�� ���ؼ� ������ auto&& �Ű��������� decltype�� ����϶�.