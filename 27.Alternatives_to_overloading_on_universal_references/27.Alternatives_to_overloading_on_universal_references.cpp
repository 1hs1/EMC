// ���� ������ ���� �ߺ����� ��� ����� �� �ִ� ��� ���� �˾Ƶζ�

#include <string>
#include <set>
#include <type_traits>

// 1. �ߺ����縦 �����Ѵ�
// �׸�26�� logAndAdd ������ �� �ߺ����縦 logAndAddNAme�� logAndAddNameIdx�� ������.
// Person�� ������ �������� ������ ����.

// 2. const T& �Ű������� ����Ѵ�.
// ���� ���� �Ű����� ��� const�� ���� �ް� ���� �Ű������� ����ϴ� ���̴�.
// ȿ������ �����ϴ��� ����ġ ���� ������ ���ϴ� ���� �� �ŷ����� �����

// 3. �� ���� ����� �Ű������� ����Ѵ�.
std::string nameFromIdx(int idx) {
	return "";
}

class Person {
public:
	explicit Person(std::string n)
		: name(std::move(n)) {}	// �׸�41 ����
	explicit Person(int idx) : name(nameFromIdx(idx)) {}
private:
	std::string name;
};
// Person�� ������ ������ ����, ����� ���� Ȯ���� ��ü�� ������ ����

// 4. ����ǥ ����� ����Ѵ�.
// logAndAdd�� ȣ���� �ٸ� �� �Լ��� �����ϰ� �Ѵ�.
// �ϳ��� �������� ���� �Լ��̰�, �ٸ� �ϳ��� �� ���� ��� ���� ���� �Լ��̴�.
// logAndAdd�� ��� �μ� ������ �޴´�.
// ���� �۾��� �����ϴ� �� �Լ��� �̸��� logAndAddImpl, �̵鿡 ���ؼ��� �ߺ����� ���
// ���ʿ� ���޵� �μ��� ���� �������� �ƴ����� ���ϴ� �� �ٸ� �Ű������� �޾� ���ϴ� ������� ����.
// std::true_type, std::false_type ����ǥ(����������� ������ ����), ���ø� ��Ÿ���α׷���
std::multiset<std::string> names;
template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
	names.emplace(std::forward<T>(name));
}

template<typename T>
void logAndAddImpl(int idx, std::true_type)
{
	logAndAdd(nameFromIdx(idx));
}

template<typename T>
void logAndAdd(T&& name)
{
	logAndAddImpl(std::forward<T>(name), std::is_integral<T>());	// �����߻� : �μ��� �ް��̸�, T�� �ް������������� ����(��:int&)
																	// �μ��� ������ �������� ��Ÿ��� std::is_integral�� �����̵ȴ�.
	logAndAddImpl(													
		std::forward<T>(name),
		std::is_integral<typename std::remove_reference<T>::type>()	// ���� �Ϸ�.
	);
}

// 5. ���� ������ �޴� ���ø��� �����Ѵ�.
// ���������� �޴� �ߺ����� ������ ������ ��� �Լ��� ������ ������ Ž�彺����(?) �ʴٸ� ����ǥ ��� ���谡 �������� �ʴ�.
// ���� ���� �Ű������� �����ϴ� �Լ� ���ø��� �ߺ����� �ؼ��� �ĺ��� �Ǵ� ���ǵ��� ������ ������ �� �ִ� std::enable_if�� �ʿ��ϴ�.
class PersonE {
public:
	template<typename T,
		typename = typename std::enable_if<
		!std::is_same<Person, 
					typename std::decay<T>::type	// T���� ��� ���� ������� const, volatile�� �����ϴ� ������ �ʿ�. std::decay
					>::value
			>::type
		>
		explicit PersonE(T&& n);
};
// Person�� �ٸ� � Person���� �����ϴ� ���, �װ��� �ް��̵� �������̵�, const�̵� ��const�̵�, volatile�̵� �� volatile�̵�,
// ���������� �޴� �����ڴ� ���� ȣ����� �ʴ´�.

// �μ� ������ ���� Person�� �ƴϸ� ���ø�ȭ�� �����ڸ� Ȱ��ȭ�ϴ� ���� �ƴ϶�, Person�� �ƴϰ�
// Person���� �Ļ��� ���ĵ� �ƴ� ������ Ȱ��ȭ�ؾ� �Ѵ�. is_same -> is_base_of�� ����

// C++14 ����
class Person14 {
public:
	template<
		typename T,
		typename = typename std::enable_if_t<
		!std::is_base_of<Person,
		std::decay_t<T>
		>::value
		>
	>
		explicit Person14(T&& n);
};

// ���� �μ����� ó���ϴ� Person ������ �ߺ����縦 �߰��ϰ�, 
// �׷� �μ��鿡 ���ؼ��� ���ø�ȭ�� �����ڰ� ��Ȱ��ȭ�ǵ��� �ϴ� ������ �߰�

class PersonF {
public:
	template<
		typename T,
		typename = std::enable_if_t<
			!std::is_base_of<Person, std::decay_t<T>>::value
			&&
			!std::is_integral<std::remove_reference_t<T>>::value
			>
		>
		explicit PersonF(T&& n)
		: name(std::forward<T>(n))
	{
		// �Ʒ� ����
		static_assert(
			std::is_constructible<std::string, T>::value,
			"Parameter n can't be used to construct a std::string"
			);
	}

	explicit PersonF(int idx)
		: name(nameFromIdx(idx)) {}

private:
	std::string name;
};

// �����
// �ߺ����縦 �����ϴ� �Ͱ� const T& �����ϴ� ��, ���� �����ϴ� ��
// ȣ��Ǵ� �Լ����� �� �Ű������� ���� ������ ����
// ����ǥ ��а� ���ø� Ȱ��ȭ ������ �Ϻ� ������ ���. �Ű��������� ������ �������� �ʴ´�.
// �Ϻ������� �� ȿ�����̶�� ���� �ϳ��� ��Ģ�̴�.
// ���� ����� �Ű����� ������ �����ϱ� ���� �ӽ� ��ü�� �����ϴ� ��ȿ������ ���� �����̴�.
// �Ϻ����� ���� : �Ϻ� ������ �Ұ����� �μ����� �ִ�.
// ��ü���� ������ �޴� �Լ����� ������ �� �־ �Ϻ� ������ �Ұ����� �� �ִ�.(�׸�30)
// �Ǵٸ� ���� : ��ȿ���� ���� �μ��� �������� �� ������ ���� �޽����� �����ϴ�.

// Person�� ��� ���� �Լ��� ���� ���� �Ű������� std::string�� ���� �ʱ�ġ�� ���� ���� �˰� �����Ƿ�,
// �׷� �ʱ�ġ�� ����ϴ� ���� ���������� �̸� ststic_assert�� �̿��ؼ� ����
// std::is_constructible�̶�� ����Ư���� �� ������ ��ü�� �ٸ� �� ����(�Ǵ� ���ĵ��� ����)�� ��ü(�Ǵ� ��ü���� ����)
// �κ��� ������ �� �ִ����� ������ �������� ����(���� ���� ����)

int main(){}

// 1. ���������� �ߺ������� ���տ� ���� ������δ� �����Ǵ� �Լ� �̸� ���, �Ű� ������ const�� ���� �ް� ������ ����,
// �Ű������� ������ ����, ����ǥ ��� ������ �ִ�.
// 2. std::enable_if�� �̿��ؼ� ���ø��� �ν��Ͻ�ȭ�� ���������ν� ���� ������ �ߺ����縦 �Բ� ����� �� �ִ�.
// std::enable_if�� �����Ϸ��� ���� ���� �ߺ����縦 ����ϴ� ������ ���α׷��Ӱ� ���� �����ϴ� �뵵�� ���δ�.
// 3. ���� ���� �Ű������� ȿ���� �鿡�� ������ ��찡 ������, ��ü�� ��뼺 �鿡���� ������ �ȴ�.