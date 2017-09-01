// typedef���� ��Ī ������ ��ȣ�϶�

#include <memory>
#include <unordered_map>

class Widget{};

// �⺻���� ������ ����ϴ�
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSStypedef;
using UptrMapSSusing = std::unique_ptr <std::unordered_map<std::string, std::string>>;

// �Լ� ������... �ణ Ʋ��
typedef void(*FP)(int, const std::string&);
using FP = void(*)(int, const std::string&);

// ���ø����� ���� ���̳�. typedef�� ���ø�ȭ�Ҽ� ������, ��Ī ������ ���ø�ȭ �Ҽ� �ִ�.
template<typename T>
using MyAllocList1 = std::list<T>;	// ���� ���ø�
MyAllocList1 <Widget> lw1;

// ���� MyAllocList1�� Ŭ���� ����� �������� �� ��...
template<typename T>
class WidgetUsing {
private:
	MyAllocList1<T> list;
	// MyAllocList1<T>�� �ݵ�� ������ �̸�. �������� ����. typename�� ���� �ʿ䰡 ����.
};

template<typename T>
struct MyAllocList2 {
	typedef std::list<T> type;
};
MyAllocList2<Widget>::type lw2;

// ���� MyAllocList2�� Ŭ���� ����� �������� �� ��...
template<typename T>
class WidgetTypedef {
private:
	typename MyAllocList2<T>::type list;                                                                                                                                                     
	// ���ø� ���� �Ű�����(T)�� �������� ����, �������� ���� �տ� �ݵ�� typename�� �ٿ��� �Ѵ�.
};

// MyAllocList2<T>::type�� ������ �ƴ� ��찡 ����(�׷��� typename�� �ٿ��� �Ѵ�.)
class Wine{};
template<>
class MyAllocList2<Wine> {	// T�� Wine�� ��쿡 ���� MyAllocList2�� Ư��ȭ
private:
	enum class WineType{White, Red, Rose};
	WineType type;	// �� Ŭ�������� type�� �ڷ� ���
};

// ��ȯ�� �����ϰ��� �ϴ� ���� T�� �־����� ��, ��ȯ�� ������ ����� ���� ������ �׻� std::��ȯ<T>::type�̴�.
template<class T>
typename std::remove_const<T>::type a;		// const T�� T�� ��ȯ
template<class T>
std::remove_const_t<T> b;			// C++14 ����
template<class T>
typename std::remove_reference<T>::type c;	// T&�� T&&�� T�� ��ȯ
template<class T>
std::remove_reference_t<T> d;			// C++14 ����
template<class T>
typename std::add_lvalue_reference<T>::type e;	// T�� T&�� ��ȯ
template<class T>
std::add_lvalue_reference_t<T> f;			// C++14 ����

// ���� ���� C++11�� ���� �ִٸ�...
template<class T>
using remove_const_ty = typename std::remove_const<T>::type;	// �̷������� ���ļ� ����ص� ����.

int main()
{}

// 1. typedef �� ���ø�ȭ�� �������� ������, ��Ī ������ �����Ѵ�.
// 2. ��Ī ���ø������� "::type" ���̾ ���� �ʿ䰡 ����. ���ø� �ȿ��� typedef�� ��Ī�� ������ "typename" ���λ縦 �ٿ��� �ϴ� ��찡 ����.
// 3. C++14�� C++11�� ��� ���� Ư�� ��ȯ�� ���� ��Ī ���ø����� �����Ѵ�.