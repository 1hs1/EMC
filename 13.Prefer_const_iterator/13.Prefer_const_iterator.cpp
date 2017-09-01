// const_iterator�� ��ȣ�϶�
// �ݺ��ڰ� ����Ű�� ���� ������ �ʿ䰡 ���� ������ �׻� const_iterator�� ����ϴ� ���� �ٶ���

#include <vector>

int main() 
{
	// C++98 ����
	std::vector<int> values;
	std::vector<int>::iterator it = std::find(values.begin(), values.end(), 1983);
	values.insert(it, 1998);

	// iterator�� ����Ű�� ���� ���� �������� �ʱ� ������ �ּ��� ������ �ƴ�
	// C++98 ���� const_iterator ���
	typedef std::vector<int>::iterator IterT;
	typedef std::vector<int>::const_iterator ConstIterT;
	std::vector<int> valuesCo;
	ConstIterT ci = std::find(static_cast<ConstIterT>(valuesCo.begin()),
		static_cast<ConstIterT>(valuesCo.end()),
		1983);
	//valuesCo.insert(static_cast<IterT>(ci), 1998);	Error!!!
	//const_iterator���� iterator���� �̽ļ� �ִ� ��ȯ�� �������� �ʴ´�.

	// C++11 ����
	std::vector<int> values11;
	auto iter = std::find(values11.cbegin(), values11.cend(), 1983);
	values11.insert(iter, 1998);
}

// �Ϲݼ��� �ش�ȭ�� �ڵ�� Ư�� ��� �Լ��� ���縦 �䱸�ϴ� ���,
// �� ��� �Լ��� �����ϴ� ���� �Լ��� ����Ѵ�.
// ���� �˻� �� ���� �ڵ带 �ϳ��� ���ø����� �Ϲ�ȭ (C++14 ����)
template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
	using std::cbegin;
	using std::cend;

	auto it = std::find(cbegin(container),
		cend(container),
		targetVal);

	container.insert(it.insertVal);
}
// C++11������ begin, end�� ǥ��ȭ�� �߰�������, cbegin, cend, rbegin, rend, crbegin, crend�� ���Ͽ���.
// C++11���� ���� cbegin �������
template<class C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
	return std::begin(container);
}

// iterator���� const_iterator�� ��ȣ�϶�.
// �ִ��� �Ϲ����� �ڵ忡���� begin, end, rbegin ���� 
// ���� �������� �ش� ��� �Լ��麸�� ��ȣ�϶�.
