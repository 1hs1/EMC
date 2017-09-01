
// auto�� ���� ���� ��Ģ�� �����϶�

// auto�� �̿��Ͽ� ���� ����� ���ø��� T�� ������ ������ �ϸ�, 
// ������ ���� �����ڴ� ParamType�� ������ ������ �Ѵ�.

// auto -> ���ø�T
// ������ ���� ������ -> ParamType
auto x = 27;
const auto cx = x;
const auto& rx = x;

// x�� cx, rx�� ���ĵ��� ������ ��, �����Ϸ��� ��ġ ���𸶴� ���ø� �Լ� �ϳ��� �ش� �ʱ�ȭ ǥ�������� 
// �� ���ø� �Լ��� ȣ���ϴ� ������ �����ϴ� ��ó�� �ൿ�Ѵ�.
template< typename T >
void func_for_x(T param); //x�� ������ �����ϱ� ���� �������� ���ø�
func_for_x(27);
template< typename T >
void func_for_cx(const T param); //cx�� ������ �����ϱ� ���� �������� ���ø�
func_for_cx(x);
template< typename T >
void func_for_rx(const T& param); //rx�� ������ �����ϱ� ���� �������� ���ø�
func_for_rx(x);

// case 1 : ���� �����ڰ� �����ͳ� ���� ���������� ���� ������ �ƴ� ���
// case 2 : ���� �����ڰ� ���� ������ ���
// case 3 : ���� �����ڰ� �����͵� �ƴϰ� ������ �ƴ� ���
auto x = 27; //��� 3
const auto cx = x; //��� 3
const auto& rx = x; //��� 1

auto&& uref1 = x; //��� 2. x�� �ް�, uref1�� ������ int&
auto&& uref2 = cx; //��� 2. cx�� const int���� �ް�, uref2�� ������ const int&
auto&& uref3 = 27; //��� 2. 27�� int���� ���� ���̹Ƿ� uref3�� ������ int&&

// ������ ���� �������� ��� �迭�� �Լ� �̸��� �����ͷ� �ر��ϴ� ��, auto�� �����ϴ�.
const char name[] = "R. N. Briggs"; //name�� ������ const char[13];
auto arr1 = name; //arr1�� ������ const char*
auto& arr2 = name; //arr2�� ������ const char (&)[13]


// auto, ���ø� ���Ŀ��� �ٸ� �� : �߰�ȣ �ʱ�ġ�� ������ ��
// int������ �����ϴ� ���� ����.�� 4���� ����� �ִ�.
int x1 = 27;
int x2(27);
int x3 = { 27 }; //c++11 ���� �ʱ�ȭ
int x4{ 27 }; //c++11 ���� �ʱ�ȭ

// auto�� ����� ���
auto x1 = 27;     //int, 27
auto x2(27);     //int, 27
auto x3 = { 27 }; //std::initializer_list<int>, {27}
auto x4{ 27 };     //std::initializer_list<int>, {27}

// auto�� ����� ������ �ʱ�ġ�� �߰�ȣ ������ ������ �����̸�, ������ ������ std::initializer_list�̴�.
auto x5 = { 1, 2, 3.0 }; //error. std::initializer_list<T>�� T�� ������ �� ����

// auto�� ����� ������ �߰�ȣ �ʱ�ġ�� �ʱ�ȭ�ϴ� ���, ������ ������ std::initializer_list�� �� �ν��Ͻ��̴�.

auto x = { 11, 23, 9 }; //x�� ������ std::initializer_list<int>

template< typename T >
void f(T param); //x�� ���� �ش��ϴ� �Ű����� ������ ����  ���ø�
f({ 11, 23, 9 }); //error T�� ���� ������ ������ �� ����

// param�� ������ � �˷��������� T�� ���� std::initialzer_list<T>�� ���ø��� �� �߰�ȣ �ʱ�ġ�� �����ϸ�
// ���ø� ���� ���� ��Ģ�鿡 ���� T�� ������ ����� �����ȴ�.

template< typename T >
void f(std::initializer_list<T> initList);
f({ 11, 23, 9 }); //T�� int��, initList�� ������ std::initializer_list<int>�� �����ȴ�.

// auto ���� ������ ���ø� ���� ������ �������� ���̴�, auto�� �߰�ȣ �ʱ�ġ�� std::initializer_list�� ��Ÿ���ٰ� ���������� 
// ���ø� ���� ������ �׷��� �ʴٴ� �ͻ��̴�.


// c++14
// �Լ��� ��ȯ ������ auto�� �����ؼ� �����Ϸ��� �����ϰ� ���� �� ������, 
// ������ �Ű����� ���� auto�� ����ϴ� �͵� �����ϴ�.�̴� ���ø� ���� ������ ��Ģ���� ����Ǹ�, 
// �߰�ȣ �ʱ�ġ�� �����ִ� �Լ��� ��ȯ ������ auto�� �����ϸ� �������� �����Ѵ�.
auto createInitList()
{
	return{ 1, 2, 3 }; //error { 1, 2, 3 }�� ������ ������ �� ����
}
// ������ �Ű����� ���� ���� �����ϴ�.
auto resetV = [&v](const auto& newValue) { v = newValue; }
resetV({ 1, 2, 3 }); //error { 1, 2, 3 }�� ������ ������ �� ����


// - auto���� ������ ��ü�� ���ø� ���� ������ ������, 
//   auto ���� ������ �߰�ȣ �ʱ�ġ�� std::initializer_list�� ��Ÿ���ٰ� �����ϴ� �ݸ� ���ø� ���� ������ �׷��� �ʴٴ� ���̰� �ִ�.
// - �Լ��� ��ȯ �����̳� ���� �Ű������� ���� auto�� ���ؼ��� auto ���� ������ �ƴ϶� ���ø� ���� ������ ����ȴ�.