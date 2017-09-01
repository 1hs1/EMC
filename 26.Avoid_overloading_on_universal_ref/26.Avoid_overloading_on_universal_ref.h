#pragma once

//���� ������ ���� �ߺ� ���縦 ���϶�.


//ex)
std::multiset< std::string > names; //���� �ڷ� ����
void logAndAdd(const std::string& name)
{
	auto now = std::chrono::system_clock::now(); //����ð��� ����
	log(now, "logAndAdd"); //�α� ����
	name.emplace(name); //�̸��� ���� �ڷᱸ���� ����
}

//�� �Լ����� ��ȿ������ ���� ���� �ִ�.���� ��� �Ʒ��� �Լ� ȣ���� ���캸��.

std::string petName("Darla");
//case1
logAndAdd(petName); //�ް�, std::string�� �Ѱ���
//case2
logAndAdd(std::string("Persephone")); //������, std::string�� �Ѱ���
//case3
logAndAdd("Patty Dog"); //���ڿ� ���ͷ��� �Ѱ���

//case1 ȣ��, name�� PetName�� ���̸�, name�� �ް��̹Ƿ� emplace�� name�� ����
//case2 ȣ��, name�� �������� ���δ�.name��ü�� �ް����� names�� ����ȴ�.��Ģ�����δ� name�� ���� names�� �̵��ϴ� ���� ����
//case3 ȣ��, name�� �������� ���̸�, �Ϲ������� ������ �ӽ� std::string��ü�� ���δ�.
//std::string���� 1ȸ ����� ġ������, ��Ģ������ ����� Ŀ�� �̵� ��뵵 ġ�� �ʿ� ����.


//����, logAndAdd�� ���������� �ް��ϰ� �� ������ std::forward�� �̿��� emplace�� �����ϸ� case2,case3 ȣ���� ��ȿ������ ������ �� �ִ�.
template<typename T>
void logAndAdd(T&& name)
{
	auto now = std::chrono::system_clock::now();
	log(now, "logAndAdd");
	names.emplace(std::forward<T>(name));
}

std::string petName("Darla");
logAndAdd(petName);
logAndAdd(std::string("Persephone")); //�������� �̵�
logAndAdd("Patty Dog"); //�ӽ� string ��ü�� �����ϴ� ���, multiset�ȿ� string�� ����


//logAndAdd�� ������ �̿��� ���� ��ȸ�ϴ� �Լ��� �ߺ� �����Ѵ�.
std::string nameFromIdx(int idx); //idx�� �ش��ϴ� �̸��� ������
void logAndAdd(int idx)
{
	auto now = std::chrono::system_clock::now();
	log(now, "logAndAdd");
	names.emplace(nameFromIdx(idx));
}

//��������� ������� ���� ���ؼ��� ������ �۵��Ѵ� ���� ���ΰ��� ���� short�� �Ѱ��ָ� ��� �ɱ� ?
short nameIdx;
logAndAdd(nameIdx); //����

//case1.T�� shor& ����, case 2.short�� int�� �°��Ͽ� ȣ��ȴ�. �°����� ���պ��� ��Ȯ�� �ߺ����� ȣ��
//�Ű����� name�� std::froward�� ���ؼ� names�� ��� �Լ� emplace�� ���޵ȴ�.
//�̴� std::string�� ���޵Ǹ�, std::string �����ڿ� short�� �����Ƿ� ������ ȣ���� �����Ѵ�.
//���� ������ �޴� ���ø� �Լ��� c++���� ���� ��� ���� �Լ��̴�.
//���ø��� �ν��Ͻ��� ���� ��� ������ �μ��� ��Ȯ�� �����Ѵ�.�̷��� ������ ���ϴ� ����� �Ϻ� ���� �����ڸ� �ۼ��ϴ� ���̴�.

//������ �޴� �����Լ� ���, �������� �ϴ� �����ڸ� ���� Person Ŭ����
class Person {
public:
	template<typename T>
	explicit Person(T&& n)
		: name(std::forward<T>(n)) {} //�Ϻ� ���� ������, �ڷ����� �ʱ�ȭ�Ѵ�.

	explicit Person(int idx)
		: name(nameFromIdx(idx)) {} //int�� �޴� ������

	//�׸�17.Ư�����ǿ��� c++�� ��������ڿ� �̵������ڸ� �ڵ����� �ۼ��ϸ�,
	//������ ���ø�ȭ�� �����ڰ� ���� �����ڳ� �̵� �����ڿ� �ش��ϴ�
	//�������� �ν��Ͻ�ȭ�Ǵ� ��쿡�� �ڵ��ۼ��ȴ�.
	Person(const Person& rhs); //���� ������ ( �����Ϸ� �ۼ� )
	Person(Person&& rhs); //�̵� ������ ( �����Ϸ� �ۼ� )

private:
	std::string name;

};

Person p("Nancy");
auto cloneOfP(p); //p�κ��� �� Person�� ����, ������ ����

//Person��ü�κ��� �� Person��ü�� ����, Person�� std::string �ڷ����� Person ��ü(p)�� �����Ϸ� �ϴµ�, 
//std::string���� Person�� �޴� �����ڰ� �����Ƿ� �����Ϸ��� ������ �߻���Ų��.
//�����Ϸ��� �߷��� cloneOfP�� const�� �ƴ� �ް�(p)���� �ʱ�ȭ�Ǹ�, 
//���� ���ø�ȭ�� �����ڸ� Person ������ ��const �ް��� �޴� ���·� �ν��Ͻ�ȭ �� �� �ִ�.

//�ν��Ͻ�ȭ�� ���� Person
class Person {
public:
	explicit Person(Person& n) //�Ϻ� ���� ���ø����� �ν��Ͻ�ȭ��
		: name(std::forward<Person&>(n)) {}
};
//p�� ���� �����ڿ��� ���޵ɼ���, �ν��Ͻ�ȭ�� ���ø��� ���޵ɼ��� �ִ�.
//������ ���� �����ڸ� ȣ���Ϸ��� p�� const�� �߰��ؾ� �Ѵ�.
//�ν��Ͻ�ȭ�� ��� �ƹ��͵� �߰����� �ʾƵ� �ȴ�.�� �ߺ����簡 �����ϴ�.
auto cloneOfP(p);


const Person cp("Nancy"); //��ü�� const
auto cloneOfP(cp); //���� �����ڸ� ȣ��

class Person {
public:
	explicit Person(const Person& n); //���ø����� �ν��Ͻ�ȭ��
	Person(const Person& rhs); //���� ������ (�����Ϸ��� ����)
};

//������ ��ü�� const�̹Ƿ� ���� �����ڰ� �޴� �Ű������� ��Ȯ�� �����Ѵ�.
//�׷��� �ߺ����� �ؼ� ����� �޶������� �ʴ´�.
//c++�� �ߺ����� �ؼ� ��Ģ �߿��� � �Լ� ȣ���� ���ø� �ν��Ͻ��� �����ø� �Լ��� �Ȱ��� �����Ѵٸ� ���� �Լ��� �켱���Ѵٴ� ��Ģ�� �ִ�.
//�׷��� ���� �����ڰ� �ڽŰ� ���� ������ ���� ���ø� �ν��Ͻ��� ����ġ�� ȣ�������� ���õȴ�.

//����� �����ϴ� Ŭ���������� �Ϻ� ���� �����ڿ� �����Ϸ��� �ۼ��� ����, �̵� ����� ������ ��ȣ�ۿ��� ���İ� ���� ũ��.

class SpecialPerson : public Person {
public:
	SpecialPerson(const SpecialPerson& rhs) //���� ������
		: Person(rhs) //��� Ŭ������ �Ϻ�, ���� �����ڸ� ȣ��
	{
	}
	SpecialPerson(SpecialPerson&& rhs) //�̵� ������
		: Person(std::move(rhs)) //��� Ŭ������ �Ϻ� ���� �����ڸ� ȣ��
	{
	}
};

//����� ���� ���� �Ű������� ���� �ߺ� ���簡 �������� ���ؾ� �Ѵ�.


//- ���� ������ ���� �ߺ������ ���� �׻� ���� ���� �ߺ� ���� ������ ���󺸴� ���� ȣ��Ǵ� ��Ȳ���� �̾�����.
//- �Ϻ� ���� �����ڵ��� Ư���� ������ ����.�׷� �����ڴ� ��ü�� ��const �ް��� ���� ���� �����ں��� �� ���� �����̸�, 
//  ��� Ŭ���� ���� �� �̵� �����ڵ鿡 ���� �Ļ� Ŭ������ ȣ����� ����ç �� �ֱ� �����̴�.