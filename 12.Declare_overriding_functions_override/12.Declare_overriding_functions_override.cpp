
//������ �Լ����� override�� �����϶�

#include <memory>
#include <vector>

//���� �Լ� �����Ǵ� �Ļ� Ŭ���� �Լ��� ��� Ŭ������ �������̽��� ���ؼ� ȣ���� �� �ְ� ����� ��Ŀ�����̴�.

/*
�����ǰ� �Ͼ���� ������ ���� ������ �����ؾ� �Ѵ�.
- ��� Ŭ���� �Լ��� �ݵ�� ���� �Լ��̾�� �Ѵ�.
- ��� �Լ��� �Ļ� �Լ��� �̸��� �ݵ�� �����ؾ� �Ѵ�.(�Ҹ��� ����)
- ��� �Լ��� �Ļ� �Լ��� �Ű����� ���ĵ��� �ݵ�� �����ؾ� �Ѵ�.
- ��� �Լ��� �Ļ� �Լ��� const���� �ݵ�� �����ؾ� �Ѵ�.
- ��� �Լ��� �Ļ� �Լ��� ��ȯ ���İ� ���� ���� �ݵ�� ȣȯ�Ǿ�� �Ѵ�.
//c++11�� �߰��� ����
- ��� �Լ����� ���� ��������� �ݵ�� �����ؾ� �Ѵ�.���� ������ ����� ����ϸ� ��� �Լ��� �ް����� �Ǵ� ���������� ����� �� �ְ� ������ �� �ִ�.
*/

class Base {
public:
	virtual void doWork() {}; //��� Ŭ���� ���� �Լ�
};
class Derived : public Base {
public:
	virtual void doWork() {}; //base::doWork�� ������
};


class Widget {
public:
	void doWork() & {}; //*this�� �ް��� �� ����
	void doWork() && {}; //*this�� �������� �� ����
};
Widget w;
//Widget makeWidget(); //���丮 �Լ�, ������ ������


//��� Ŭ������ ���� �����簡 ����Ǿ� ���� ���, �Ļ� Ŭ�������� �ش� �Լ��� ���� �Ǿ� �־�� �Ѵ�.
class Base_fail {
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	void mf4() const;
};
class Derived_fail : public Base_fail {
public:
	virtual void mf1(); // const
	virtual void mf2(unsigned int x); // unsigned int
	virtual void mf3() && ; // &, &&
	void mf4() const; // virtual
};
/*
class Base_fail_2 {
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	void mf4() const;
};
class Derived_fai_2 : public Base_fail_2 {
public:
	virtual void mf1() override; // const
	virtual void mf2(unsigned int x) override; // unsigned int
	virtual void mf3() && override; // &, &&
	void mf4() const override; // virtual
};
*/

//c++11, override ����
class Base2 {
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	virtual void mf4() const;
};

class Derived2 : public Base2 {
public:
	virtual void mf1() const override;
	virtual void mf2(int x) override;
	virtual void mf3() & override;
	void mf4() const override; //virtual�� �����ص� �ȴ�.
};

//�Ļ� Ŭ������ ��� ������ �Լ��� override�� �����ϴ� ����, ��� Ŭ������ �� �����Լ��� ���� ���� �� ������ �ȴ�.


//���� �������� ���� data�� ������ Widget�� ȣ��� ��� ������ ����, �ް� Widget ȣ��� ��� �ް�
//data�� �ް� Widget�� ������ Widget�� ���� ������ �ߺ� ����
class Widget2 {
public:
	using DataType = std::vector<double>;

	DataType& data() & { return values; } //�ް� Widget�� ���ؼ� �ް� ��ȯ
	DataType&& data() && { return std::move(values); } //������ Widget�� ���ؼ� ������ ��ȯ
private:
	DataType values;
};
Widget2 w2;
//Widget2 makeWidget(); //���丮 �Լ�, ������ ������

int main()
{
	//�Ļ� Ŭ���� ��ü�� ����Ű�� ��� Ŭ���� ������
	std::unique_ptr<Base> upb = std::make_unique<Derived>();
	upb->doWork(); //��� Ŭ���� �����ͷ� doWork()�� ȣ��, �Ļ� Ŭ������ �Լ��� ȣ��ȴ�.

	w.doWork(); //�ް���, Widget::doWork&�� ȣ��
	//makeWidget().doWork(); //��������, Widget::doWork&&�� ȣ��

	auto vals1 = w2.data(); //�ް� �ߺ����� ȣ��, ���� ������
  //auto vals2 = makeWidget2().data(); //������ �ߺ����� ȣ��, �̵� ������
}

//��� �Լ��� ���� �����簡 �پ� ������ �� �Լ��� ��� �ߺ����翡�� ���� �����縦 �����ؾ� �Ѵ�.
//�̴� ���� ���� ���� �ߺ����簡 �ް� ��ü�� ������ ��ü ��� ȣ��� �� �ֱ� �����̴�.

//- ������ �Լ��� override�� �����϶�
//- ��� �Լ� ���� �����縦 �̿��ϸ� ��� �Լ��� ȣ��Ǵ� ��ü(*this)�� �ް� ������ ������ ������ �ٸ� ������� ó���� �� �ִ�.