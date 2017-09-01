// ����� ���𺸴ٴ� auto�� ��ȣ�϶�

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

class Widget
{
public:
	bool operator < (Widget const& rhs)
	{
		return true;
	}
};

int main()
{
	int x1{};		// ���ƿ� ���� �ʱ�ȭ ���� ���� �� ����.
	//auto x2;		// ����! �ʱ�ġ�� �� �ʿ���
	auto x3 = 0;	// ��ȣ
	
	std::unordered_map<std::string, int> m;
	for (const std::pair<std::string, int>& p : m) // const auto& p : m
	{
		// m�� std::pair<const std:string, int> ��ü, p�� std::pari<std::string, int> ����, ��ȯ �ʿ�.
		// p�� ������ �ϴ� ������ �ӽ� ��ü�� �����ϰ�, m�� �� ��ü�� �����ϰ�, ���� p�� �� �ӽð�ü�� �������ν� ��ȯ ����
		// auto�� ����ϸ� p�� �ӽ� ��ü�� �ƴ� ���� m ���� ��Ҹ� ����Ű�� �����͸� ����.
	}
}

template<typename It>
void dwim(It b, It e)
{
	for (; b != e; ++b)
	{
		auto currValue = *b;	 //typename std::iterator_traits<It>::value_type currValue = *b;	//�������� ����
	}
}

// �����Ϸ��� �˴� ������ ����
// std::unique_ptr���� ����Ű�� Widget��ü���� ���ϴ� �Լ�
// C++11 ����
auto derefUPLess = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2)
{return *p1 < *p2; };
// C++14 ����
auto derefLess = [](const auto& p1, const auto& p2)
{return *p1 < *p2; };
// auto ��� std::function�� ����� ���
std::function<bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&)>
derefUPFLess = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2)
{return *p1 < *p2; };
// auto ��� std::function�� ���� ��������
// 1. auto�� ����� Ŭ������ ��� ������ �� Ŭ������ ���� ����, �� Ŭ������ �䱸�Ǵ� ��ŭ�� �޸𸮸� ���.
//	  std::function���� ����� Ŭ������ ���� ������ std::function ���ø��� �� �ν��Ͻ�
//    �� ũ��� ������ �־��� ���� ���� ����, Ŭ������ �����ϱ⿡ ������ ���� �ְ�,
//    �� ��� ���޸𸮸� �Ҵ��Ͽ� Ŭ������ ����. ��������� �޸𸮸� �� ���� �Һ�
// 2. std::function�� �ζ���ȭ�� �����ϰ� ���� �Լ� ȣ���� �����ϴ� ���� ���λ��� ������,
//    auto�� ����� ��ü�� ���� ȣ���ϴ� �ͺ��� ������.
// * std::bind ��� ���� ����� ����. �׸� 34

std::vector<int> v;
unsigned sz = v.size();
// v.size()�� ���� ��ȯ���� : std::vector<int>::size_type
// 32��Ʈ Windows������ unsigned�� std::vector<int>::size_type�� ���� ũ��
// 64��Ʈ Windows������ unsigned�� 32��Ʈ, std::vector<int>::size_type�� 64��Ʈ
// auto�������� �ذᰡ�� auto sz = v.size();

// ���
// auto ������ �ݵ�� �ʱ�ȭ�ؾ� �ϸ�, �̽ļ� �Ǵ� ȿ���� ������ ������ �� �ִ� ���� ����ġ�� �߻��ϴ� ��찡 ���� ����.
// ��ü�� ������ ������ ��������� ������ ������ Ÿ�ڷ��� �� ����.
