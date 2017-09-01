// ������ ������ �ľ��ϴ� ����� �˾Ƶζ�

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>


// ���� ���� ������ ��� ���
// �ڵ� �ۼ�, ���� ������ ������ ���� �������� ���� �����̴�.

// IDE������
// IDE(���� ���� ȯ��)�� �ڵ� ������ �߿��� ���α׷� ��ü���� ���콺 ������ ������ �ִ� ��찡 ����.
// �̷� ����� �����Ϸ��� �������� �ڵ尡 ������� ������ ������ �����̾�� �Ѵ�.
// �̷� ����� ������ ������ IDE�ȿ��� c++�����Ϸ��� ����Ǳ� �����̴�.


// �����Ϸ��� ���� �޽���
// ������ ������ �߻��ϵ��� ����� ��
template<typename T>
class TD; //TD�� ���� �صд�.


// ������� ���
// printf�� �̿��� ǥ���ϴ� ���
//������ �����Ϸ� ���� �޽����� ���ؼ� ��Ȯ�� �˱�� ��ƴ�.
class Widget {};
std::vector<Widget> createVec()
{
	std::vector<Widget> vw;
	Widget w;
	vw.push_back(w);
	return vw;
};
template<typename T>
void f(const T& param)
{
using std::cout;
cout << "T = " << typeid(T).name() << '\n'; //T�� ǥ�� 
cout << "param = " << typeid(param).name() << '\n'; //param�� ������ ǥ��
}
// std::type_info::name�� ������ param�� ������ Ʋ���� ���� �ߴ�. (ǥ���� �ؼ� : �� ���� �Ű������� ��� )
// �� ������ ��� ���� ������ �����̸� �������� ���õǸ�, ������ ������ ���� ������ const�̸� �ش� const�� ���� ���õȴ�.
// �׷��� ������ const Widget* const&�� param�� ������ const Widget*���� ��µ� ���̴�.
// const Widget*�� ������ ���Ŀ��� ���� �������� ���ŵǰ�, �׷��� ���� �������� const���� ���ŵ� ����̴�.
// ���������� IDE�����Ⱑ ǥ���ϴ� ���� ���� �������� �ʴ�.

//���̺귯���� �ŷ��Ѵٸ� Boost TypeIndex���̺귯���� �ִ�.
#include <boost/type_index.hpp>
template<typename T>
void fb(const T& param)
{
using std::cout;
using boost::typeindex::type_id_with_cvr;
using boost::typeindex::type_id;
using boost::typeindex::type_id_runtime;

//T�� ǥ��
cout << "T = " << type_id_with_cvr<T>().pretty_name() << '\n';
//param�� ������ ǥ��
cout << "param = " << type_id_with_cvr<decltype(param)>().pretty_name() << '\n';

//type_id�� ������ ǥ��
cout << "type_id param = " << type_id<decltype(param)>().pretty_name() << '\n';
//type_id_runtime�� ������ ǥ��
cout << "type_id_runtime param = " << type_id_runtime(param).pretty_name() << '\n';
}
// �Լ� ���ø� boost::typeindex::type_id_with_cvr�� �ڽſ��� ���޵� ���� �μ� const�� volatile, ���� ��������� �״�� �����Ѵ�.
// type_id_with_cvr�� �ϳ��� boost::typeindex::type_index��ü�������ϸ�, �� ��ü�� pretty_name����Լ��� std::string��ü�� �����ش�.

// Boost.TypeIndex�� ��Ȯ�� ����� ����.IDE, �����Ϸ� ����, Boost.TypeIndex�� �����Ϸ��� �����ϴ� ������ �ľ��ϱ� ���� ������ ���̴�.
// �ñ������� �߿��� ���� �׸� 1~3�� ���� ���� ��Ģ�� �����̴�.

//- �����Ϸ��� �����ϴ� ������ IDE �����⳪ �����Ϸ� ���� �޽���, Boost.TypeIndex ���̺귯���� �̿��ؼ� �ľ��� �� �ִ� ��찡 ����.
//- �Ϻ� ������ ����� ���������� �ʰ� ��Ȯ������ ���� �� �����Ƿ�, c++�� ���� ���� ��Ģ���� ����� �����ϴ� ���� ������ �ʿ��� ���̴�.*/


int main()
{
	// IDE������
	const int theAnswer = 7;
	auto x = theAnswer;
	auto y = &theAnswer;

	// �����Ϸ��� ���� �޽���
	//TD<decltype(x)> xType;
	//TD<decltype(y)> yType;
	//error : 'xType'��(��) ���ǵ��� ���� class 'TD<int>'��(��) ����մϴ�.
	//error : 'yType'��(��) ���ǵ��� ���� class 'TD<const int *>'��(��) ����մϴ�.

	//������� ���
	//typeid�� std::type_info��ü�� name�̶�¸�� �Լ� ���
	std::cout << typeid(x).name() << '\n'; //x�� ������ i( int )
	std::cout << typeid(y).name() << '\n'; //y�� ������ PKi(int const * __ptr64)
	const auto vw = createVec();
	if (!vw.empty())
	{
		// ���ø� f���� param�� ����� ������ const T&�̴�.�׷����� T�� param ���� ���� ���
		// T�� int���ٸ�, param�� ������ �׿ʹ� �ٸ� constint&�� �Ǿ�� �Ѵ�.
		f(&vw[0]);
		// Boost.TypeIndex�� ��Ȯ�� ���
		fb(&vw[0]);
	}

	return 0;
}