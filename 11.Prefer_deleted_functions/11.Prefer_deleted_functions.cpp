// ���ǵ��� ���� ������Լ����� ������ �Լ��� ��ȣ�϶�
// C++�� �ʿ信 ���� �ڵ����� �ۼ��ϴ� ����Լ����� �ִ�.(�׸�17)

// C++98������ ����� �����Ϸ��� ��� �Լ��� ���� ������ �Ǵ� ���� ���� �����ڿ���.
// ��� : private�� �����ϰ� �������� �ʴ´�.
template <class charT, class traits = char_traits<chatT>>
class basic_ios98 : public ios_base {
public:
private:
	basic_ios98(const basic_ios98&);			// not defined
	basic_ios98& operator=(const basic_ios98&);	// nod defined
};

//  C++11������ ����� �����Ϸ��� ����Լ� ���� ���� "=delete"�� ���̴� ���̴�. (������ �Լ�)
template <class charT, class traits = char_traits<chatT>>
class basic_ios11 : public ios_base {
public:
	basic_ios11(const basic_ios11&) = delete;
	basic_ios11& operator=(const basic_ios11&) = delete;
};
// ��� �Լ��� friend�Լ����� basic_ios11��ü�� �����Ϸ� �ϸ� �������� �����Ѵ�.
// �̴� �������� ��ʰ� ��ũ ������ ������ �߰ߵǴ� C++98��Ŀ� ���� ������ �����̴�.

// ������ �Լ��� private�� �ƴ϶� public�� ����
// C++�� ���� �� �Լ��� ���ټ��� ������ �Ŀ��� ���� ���θ� ����. �� ���� �����޽����� ���� ���ؼ�...

// ������ �Լ��� �Ǵٸ� ���� : �� � �Լ����� ������ �� �ִ�.
// �ݸ� private�� ��� �Լ����� ������ �� �ִ�.
bool isLucky(int number);		// ���� �����ϰ����ϴ� �Լ�(���ڸ� �Է� ����)
bool isLucky(char) = delete;	// char ����	// if(isLucky('a'))...	����
bool isLucky(bool) = delete;	// bool ����	// if(isLucky(true)...	����
bool isLucky(double) = delete;	// double�� float ����	// if(isLucky(3.5f))... ����

// ������ �Լ��� ��ġ �ʴ� ���ø� �ν��Ͻ�ȭ�� ������ �� �ִ�
template<typename T>
void processPointer(T* ptr);
// void* �������� ����, ���Ұ� �ƿ� �Ұ����ϴ�.
// char* C ��Ÿ�� ���ڿ��� ��Ÿ����.
// �̷��� �����͵��� Ư���� ó���� �ʿ��� ��찡 ������, ȣ���� �ź��ϱ� ���� ������� delete�� ����Ѵ�.
template<>
void processPointer<void>(void*) = delete;

template<>
void processPointer<char>(char*) = delete;

// Ŭ���� ���� �Լ� ���ø��� �Ϻ� �ν��Ͻ�ȭ�� �����Ϸ��� �������� private������ ���� �ȵ�
class Widget {
public:
	template<typename T>
	void processPointer(T* ptr) {}

private:
	template<>							// ����
	void processPointer<void>(void*);	
};

// private���� �����ϵ� ���Ǹ� �����ϴ� C++98 ����� Ŭ���� �ٱ������� �۵����� ������, Ŭ���� �ȿ����� �׻� �۵������� �ʴ´�.
// �۵��Ѵٰ� �ص� ��ũ������ ������ �۵��ϴ� ��쵵 �ִ�.

int main(){}

// ���ǵ��� ���� ����� �Լ����� ������ �Լ��� ��ȣ�϶�.
// ���� �Լ��� ���ø� �ν��Ͻ��� ����� �� � �Լ��� ������ �� �ִ�.