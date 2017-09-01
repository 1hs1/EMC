#pragma once
// ���ü����� std::atomic�� ����ϰ�, volatile�� Ư���� �޸𸮿� ����϶�.

// std::atomic
std::atomic<int> ai(0);
ai = 10;
std::cout << ai;
++ai;
--ai;
// ��ü ������ ���������� ó���ȴٴ� ������ ����
// �д� ������ �������̴�


//volatile
volatile int vi(0);
vi = 10;
std::cout << vi;
++vi;
--vi;
// ���� ������ ���ƿ��� �ƹ��͵� �������� �ʴ´�.


// ���� ������ ���α׷� ī���� ��
std::atomic<int> ac(0);
volatile int vc(0);

/*----- Thread 1 ----- */ /*------- Thread 2 ------- */
++ac;											++ac;
++vc;											++vc;

// ac : �ݵ�� 2
// vc : 2�� ���� �ƴ� ���� �ִ�.


// atomic ������ �ϰ���
// �ڵ� ���ġ ����
std::atomic<bool> valAvailable(false);
auto imptValue = computeImportantValue();
valAvailable = true;

ex)
a = b;
x = y;
// �����Ϸ��� ���� �ٲܼ� �ִ�.
x = y;
a = b;

// volatile
volatile bool valAvailable(false);
auto imptValue = computeImportantValue();
valAvailable = true;

// 1. ������ ���ڼ��� �������� �ʴ´�
// 2. �ڵ� ���ġ�� ���� ������ ������� �ʴ�.


// ���Ƶ��� ����, ���� ����
int x;
auto y = x;
y = x;

x = 10;
x = 20;

// �����Ϸ� ��ȯ ( ����ȭ )
auto y = x;
x = 20;

// volatile
// �ش� �޸𸮿� ���� ����鿡�� �� � ����ȭ�� �������� ����
volatile int x;
auto y = x;
y = x; // ����ȭ�� ������ �� ����
x = 10;
x = 20; // ����ȭ�� ������ �� ����
// �޸� ���� �����(memory-mapped I/O) ��ȿ


// atomic
std::atomic<int> x;
auto y = x;
y = x;
x = 10;
x = 20;

// ����ȭ ok, ������ error
auto y = x;
y = x;

// ���� ����/ ���� ���� ���� ( x�а� y�� ����ϴ� �۾��� �ϳ��� ������ �������� ���� �Ұ� )
// �׸� 17 Ư�� ��� �Լ� ���� ��Ģ : �̵� ����, �̵� ���� �������� �ʴ´�.


std::atomic<int> y(x.load());
y.store(x.load());

// �����Ϸ� : �������׿� ������ ����ȭ �Ҽ���
register = x.load();
std::atomic<int> y(register);
y.store(register);

// volatile, std::atomic �Բ� ��� ����
volatile std::atomic<int> vai;

// - std::atomic�� ���ؽ� ��ȣ ���� ���� �����尡 �����ϴ� �ڷḦ ���� ������, ������ ����Ʈ������ �ۼ��� ���� �����̴�.
// - volatile�� �б�� ����� ����ȭ�� �������� ���ƾ� �ϴ� �޸𸮸� ���� ������, Ư���� �޸𸮸� �ٷ� �� �ʿ��� �����̴�.
