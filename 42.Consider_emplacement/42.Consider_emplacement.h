#pragma once
// ���� ��� ���� ������ ����϶�

// std::string��ü�� ��� ���� �Լ�(insert, push_front, push_back, std::forward_list ��� insert_after)��
// �Ѱ��ִ� ��� ������ std::string�̾�� ������ �ƴ� ��쵵 �ִ�.
std::vector<std::string> vs;
vs.push_back("xyzzy"); // ���ڿ� ���ͷ�

// push_back �ߺ�����
template <class T,
	class Allocator = allocator<T>>
	class vector {
	public:
		void push_back(const T& x);
		void push_back(T&& x);
};


// �����Ϸ� (�ӽ� ��ü ���� )
vs.push_back("xyzzy");  -> vs.push_back(std::string("xyzzy"));

// 1. "xyzzy" ���� �ӽ� std::string ��ü ���� : temp�� Ī�ϰ� �ӽ� ��ü�̹Ƿ� ������
// 2. temp�� push_back�� ������ �ߺ������ ����. temp�� ������ ���� �ŰԺ��� x�� ���δ�.
//	  std::vector�� ���� �޸� �ȿ��� x�� ���纻�� �����ȴ�. (�����ϴ� �� ���� �����ڴ� �̵� ������)
//		�̶� �ι�° std::string ���� std::vector�ȿ� ���� �� ��ü ����
// 3. push_back�� ��ȯ�� ��� temp �ı� std::string �Ҹ��ڰ� ����

// �־��� �μ��� �̿��� std::vector�ȿ��� ���� std::string�� ����
vs.emplace_back("xyzzy");
// push_back : emplace_back
// push_front : emplace_front
// insert : emplace ( std::forward_list, std::array ���� )
// ���� �����̳� insert : emplace_hint
// forward_list intsert : emplace_after
// ���� ���� �Լ��� ���� �Լ����� ���� :���� �Լ��� ���� ��ü�� ������ ���� ���� �Լ����� ������ ��ü�� �����ڸ� ���� �μ����� �޴´�.
// ������ ���� ������ �ӽ� ��ü�� ������ �ı��� ���Ѵ�

// �����̳ʿ� ��� ����� ���İ� ������ ������ �μ� ( ���� ���� �Ǵ� �̵� ���� )
std::string queenOfDisco("Donna Summer");
vs.push_back(queenOfDisco); // ���� ����
vs.emplace_back(queenOfDisco); // ���� ����

// ���� �׻� ���� ������ ������ ������ �ɰ��ϴ� ���
// ������ ��� ����
// 1. �߰��� ���� �����̳ʿ� �����Ǵ� ���� �ƴ϶� �����̳� �ȿ��� �����ȴ�.
// 2. �߰��� �μ� ����(��)�� �����̳ʰ� ���� ���İ� �ٸ���.
// 3. �����̳ʰ� ���� ������ �ߺ� ������ �� ���� �ź��� ����� ���� ����.

// ���� ���� �Լ��� ��� ������ ��� ����
// 1. �ڿ� ����
std::list<std::shared_ptr<Widget>> ptrs;
void killWidget(Widget* pWidget);
ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
ptrs.push_back({ new Widget, killWidget });
// 1-1. �� ȣ�� ���, 'new Widget'���� ������� �� �����͸� ��� �ӽ� std::shared_ptr<Widget> ��ü ����( temp )
// 1-2. push_back�� �� temp�� ������ �޴´�. temp�� ���纻�� ���� ��� ��带 �Ҵ��ϴ� ���߿�
// �޸� ���� ���ܰ� �߻��Ѵ�.
// 1-3. ���ܰ� push_back �ٱ����� ���ĵǸ鼭 temp�� �ı��ȴ�. temp�� �ڽ��� �����ϴ�
// Widget�� �����ϴ� ������ std::shared_ptr ��ü�����Ƿ�, temp�� �ı��Ǹ鼭
// �ش� Widget�� �ڵ����� ����( killWidget ȣ�⿡ ���� )
// �޸� ���� ����

ptrs.emplace_back(new Widget, killWidget);
// 1-1. 'new Widget'���� ������� �� �����Ͱ� emplace_back���� �Ϻ� ����.
// emplace_back�� �� ���� ���� ��� ��带 �Ҵ��Ѵ�. �׷��� �Ҵ� �����ϰ� �޸� ���� ���� �߻��Ѵ�.
// 1-2. 'emplace_back' ������ ���ĵǸ鼭, ���� �ִ� Widget ��ü�� �����ϴ� ������ ������ �� �����Ͱ� �������.
// �ᱹ �� Widget�� �׸��� �� ��ü�� ������ ��� �ڿ��� ���� 
// �޸� ����

// �ڿ� ���� ��ü�� ���ʿ� 'new Widget'�� �����Ϸ� �ߴ� �Լ��� ���������� �����ؾ��Ѵ�. (�׸� 21)
std::shared_ptr<Widget> spw(new Widget, killWidget);
ptrs.push_back(std::move(spw));

std::shared_ptr<Widget> spw(new Widget, killWidget);
ptrs.emplace_back(std::move(spw));

// 2. explicit �����ڵ���� ��ȣ �ۿ� ����̴�.
std::vector<std::regex> regexes;

regexes.emplace_back(nullptr); // ������ ok ( ������ )
// std::regex ��ü�� ��ȯ�� ������ �ƴ϶� std::regex ��ü�� �����ڿ� ������ �μ�.
// std::regex r(nullptr)

std::regex r = nullptr; // ������ error

regexes.push_back(nullptr); // ������ error

std::regex upperCaseWord("[A-Z]+");
// const char* �޴� std:::regex �����ڴ� explicit�� ����

// ���� �ʱ�ȭ
std::regex r1 = nullptr; // ������ error
// ���� �ʱ�ȭ
std::regex r2(nullptr); // ������ ok ( ������ )

// explicit ������ : ���� �ʱ�ȭ ��� �Ұ�, ���� �ʱ�ȭ ���

// ���� �Լ��� ����� ������ ����� �� �μ��� �Ѱ��־�� ��

// - �̷�������, ���� ���� �Լ����� ���� �ش� ���� �������� �� ȿ�����̾�� �ϸ�,
// �� ȿ������ ���� ����� ����� �Ѵ�.
// - ����������, ���� (1) �߰��ϴ� ���� �����̳ʷ� �����Ǵ� ���� �ƴ϶� �����ǰ�,
// (2) �μ� ����(��)�� �����̳ʰ� ��� ���İ� �ٸ���, (3) �� ���� �ߺ��� ���̾
// �����̳ʰ� �ź����� �ʴ´ٸ�, ���� ���� �Լ��� ���� �Լ����� ���� ���ɼ��� ���� ũ��.
// - ���� ���� �Լ��� ���� �Լ���� �źδ����� ���� ��ȯ���� ������ �� �ִ�.
