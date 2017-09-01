//���� ���� enum���� ���� �ִ� enum�� ��ȣ�϶�.

#include <vector>
#include <tuple>


std::vector<std::size_t> primeFactors(std::size_t x)
{
	std::vector<std::size_t> v;
	return v;
};

// template
template<typename E1>
constexpr typename std::underlying_type<E1>::type
toUType_1(E1 enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E1>::type>(enumerator);
}
// underlying_type_t
template<typename E2> // C++14
constexpr std::underlying_type_t<E2>
toUType_2(E2 enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E2>>(enumerator);
}
// auto
template<typename E3> // C++14
constexpr auto toUType_3(E3 enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E3>>(enumerator);
}

int main()
{
	//������ ���� enum
	enum Color { green, blue };
	//auto green = false; //error �̹� white�� �����ȿ� �����

	//������ �ִ� enum(enum class��� �θ��⵵ �Ѵ�)�� ��� �̸� ������ �߻����� �ʴ´�.
	enum class Scoped_Color { black, white, red };
	auto white = false; //ok ������ �ٸ� white�� ����
	//Scoped_Color c = white; //error ������ �ٸ� white�� ����
	Scoped_Color c = Scoped_Color::white; //ok
	auto c1 = Scoped_Color::white; //ok

  //������ ���� enum�� ��� �Ϲ������� ���� �������� ��ȯ (��ġ �ʴ� ���)
	Color c2 = Color::green;
	if (c2 < 14.5) {
	auto factors = primeFactors(c2);
	}
	//������ �ִ� enum�� ��� �Ϲ������� �ٸ� �������� ��ȯ���� �ʴ´�.
	/*
	std::vector<std::size_t> primeFactors(std::size_t x);
	Scoped_Color c = Scoped_Color::red;
	if (c < 14.5) { //error c�� double �� �Ұ�
	auto factors = primeFactors(c); //error std::size_t�� ����ϴ� �Լ��� color ���� �Ұ�
	}
	*/
	//color�� �ٸ� �������� ��ȯ�ϰ� ���� ���� ĳ������ ����Ѵ�
	Scoped_Color c3 = Scoped_Color::red;
	if (static_cast<double>(c3) < 14.5) {
		auto factors = primeFactors(static_cast<size_t>(c3));
	}

	//�� �ٸ� ������ ������ �ִ� enum�� ��� ���� ������ �����ϴٴ� ���̴�.
	//������ �ִ� enum�� ��� �⺻������ �������� �����Ѵ�.
	//enum�� ���漱���� �� ������ ��� ������ �����.
	//���� ������ ���� ���谡 �þ�� ������ ���ο� �����ڰ� �߰��� ��� �ý��� ��ü�� �ٽ� ������ �ؾ� �Ѵ�.

	enum Status { good = 0, failed = 1, incomplete = 100, aduited = 500, indeterminate = 0xFFFFFFFF };

	void continueProcessing(Status s);

	//���漱��
	enum class Status1;
	//���漱���� ��� Status�� ���ǰ� �ٲ� �� ������ �� �ʿ䰡 ����.Status�� ������ �Ǿ, 
	//continueProcessing�� ������ ���� �ʴ´ٸ� �ٽ� ������ �� �ʿ䰡 ����.

	//������ �ִ� enum �⺻ ���� int
	//���� ���� enum���� �⺻ ���� ������ ����

	//���� ������ �����ϴ� ���
	//�ٸ� ������ ��������� ����
	enum class Status2 : std::uint32_t;

	//������ ���� enum�� �������� ����
	enum Color2 : std::uint8_t;

	//enum ���� �ÿ��� ����
	enum class Status3 : std::uint32_t { good = 0, failed = 1 };


	// tuple
	using UserInfo =
		std::tuple < std::string, // name
		std::string, // email
		std::size_t>; // reputation

	UserInfo uInfo;
	auto val_1 = std::get<1>(uInfo); // ?? 1

	enum UserInfoFields { uiName, uiEmail, uiReputation };
	auto val_2 = std::get<uiEmail>(uInfo);

	enum class Scoped_UserInfoFields { uiName, uiEmail, uiReputation };
	auto val_3 = std::get<static_cast<size_t>(Scoped_UserInfoFields::uiEmail)>(uInfo);

	auto val = std::get<toUType_3(Scoped_UserInfoFields::uiEmail)>(uInfo);

	return 0;
}


//- c++98 ��Ÿ���� enum�� ������ ���� ���� enum�̶�� �θ���.
//- ���� �ִ� enum�� �����ڵ��� �� �ȿ����� ���δ�.�� �����ڵ��� ���� ĳ������ ���ؼ��� �ٸ� �������� ��ȯ�ȴ�.
//- ���� �ִ� enum�� �������� enum��� ���� ���� ������ �����Ѵ�.���� �ִ� enum�� �⺻ ���� ������ int ���� ���� enum���� �⺻ ���� ������ ����.
//- ���� �ִ� enum�� �׻� ���� ������ �����ϴ�.���� ���� enum�� �ش� ���� ���� ������ �����ϴ� ��쿡�� ���� ������ �����ϴ�.