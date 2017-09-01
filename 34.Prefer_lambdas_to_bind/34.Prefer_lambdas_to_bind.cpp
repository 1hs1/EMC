//std::bind���� ���ٸ� ��ȣ�϶�

#include<chrono>
#include <functional>

using Time = std::chrono::steady_clock::time_point;
enum class Sound { Beep, Siren, Whistle };
using Duration = std::chrono::steady_clock::duration;
void setAlarm(Time t, Sound s, Duration d) {};

enum class Volume { Normal, Loud, LoudPlusPlus };
void setAlarm(Time t, Sound s, Duration d, Volume v) {};

class Widget {};
enum class CompLevel { Low, Normal, High };
Widget compress(const Widget& w, CompLevel lev) {
	return w;
};

class PolyWidget
{
public:
	template<typename T>
	void operator()(const T& param) {};
};

int main()
{
	
		//C++11���� ���� ��κ��� ��� std::bind���ٴ� ���� ǥ������ ���� ���� �� ���� �����̴�.
		//������ ����� ������ȭ�� C++14 ���� ����������.�� �������� ���� ������ ������, �켱 ���� ù��°�� �������̴�.
		//std::bind�� �̿��� �ۼ��� �ڵ庸�� ���� ǥ�������� �ۼ��� �ڵ��� �������� �� �پ��.
		
		//Ÿ�� t�� �Ǹ� d �ð����� s �Ҹ��� ��
		//SetAlarm()
		//�̷� �Լ��� ���� ��, ���α׷� ������ Ư�� ��Ȳ�� �Ǹ� �ش� �������κ��� 1�ð� �ڿ� 30�ʵ��� �Ҹ��� ������ �˶��� �����ϰ� �ʹٰ� ����.
		//� �Ҹ��� �� ���� ���ڷ� �޴� ���� �Լ��� �̿��ϸ� ���� �ذ��� �� �ִ�.

		//C++11
		auto setSoundL =
		[](Sound s)
		{
			using namespace std::chrono;

			setAlarm(steady_clock::now() + hours(1),
				s,
				seconds(30));
		};

		//C++14
		auto setSoundL_ =
			[](Sound s)
		{
			using namespace std::chrono;
			using namespace std::literals;

			setAlarm(steady_clock::now() + 1h,
				s,
				30s);
		};
		//�� �ڵ带 std::bind�� �̿��ؼ� ¥���� �ϸ� �ϴ� �Ʒ��� ���� ���·� �����ϰ� �� ���̴�(�ùٸ� �ڵ尡 �ƴϴ�.�ϴ� �̷��� �����غ���).

		using namespace std::chrono;
		using namespace std::literals;

		using namespace std::placeholders;

		/*auto setSoundB =
			std::bind(setAlarm,
				steady_clock::now() + 1h, // �߸��� �ڵ�!
				_1,
				30s);*/
		//���⼭ _1�� placeholder��, ���߿� setSoundB�� operator()�� ȣ���� �� ���� ���ڿ� ������� �̸��� ���� ���̶�� �����ϸ� �ȴ�.
		//�� placeholder�� std::bind�� �������� ����߸��� �������� �ϳ�����(�� setSoundB�� ���𹮸� ���� �̰� � Ÿ���� ���ڸ� �޴��� �˱Ⱑ �����.
		//setAlarm�� ���Ǻα��� ���߸� �� �� ����), �װͺ��� ���� �ϴ� ���� �ڵ�� �߸��� �ڵ��.
		//steady_clock::now() + 1h�� setSoundB �Լ� ��ü�� ����Ǵ� ������ ����Ǵ°� �ƴ϶�, 
		//�� bind object�� ������ �� ���ǰ� �� ���� ���������� �����ϰ� �ְ� �ȴ�.
		//�� �츮�� ���ϴ� �ǵ��ʹ� ����� �ٸ� ������ �ϴ� �Լ� ��ü�� ��������� ���̴�.
		//�� ������ �ذ��Ϸ��� steady_clock::now() + 1h ������ ������ setAlarm �Լ��� ȣ��Ǵ� �������� �̷���ϹǷ�, bind�� ��ø�ؼ� ��߸� �Ѵ�.

		/*auto setSoundB_ =
		std::bind(setAlarm,
			std::bind(std::plus<steady_clock::time_point>(),
				steady_clock::now(), 1h),
			_1,
			30s);*/
		//�� ������ ���� ���ٰ� �ξ� ���ٴ� ������ �� ���̴�.

		//std::bind���� �Ǵٸ� �������� �ִ�.�ٷ� �����ε��� ���õ� �����̴�.���� setAlarm �Լ��� �����ε��� �Լ���� ����.

		
		//���� ���� �˶��� ũ����� �޴� ���ο� setAlarm �Լ��� �����ε��Ǿ��ٰ� ����.
		//���ٴ� ������ �ٸ����� �� ����������, std::bind ������ ������ ����Ų��.
		//�ռ� �׸񿡼��� �ٷ����� �����ε��� �Լ��� �̸��� ����� � setAlarm�� �����ؾ����� �� ���� ���� �����̴�.
		//�̸� �ذ��ϱ� ���ؼ��� setAlarm �Լ��� �����ϰ� ĳ�����ϴ� �� �ۿ� ����.

		using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);

		auto setSoundB =
			std::bind(static_cast<SetAlarm3ParamType>(setAlarm),
				std::bind(std::plus<>(), //c++ 14���� Ÿ������ ���� ����
					steady_clock::now(),
					1h),
				_1,
				30s);
		//�̷� Ư¡�� std::bind�� ���� ǥ���� ���̿� ���ο� ���̸� ������.

		//setAlarm �Լ��� body�� �� ���ο� inline�� �� ����
		setSoundL(Sound::Siren);
		//���ٸ� ����� ��� setSoundL�� operator() ���ο��� �ܼ��� setAlarm �Լ��� ȣ�� �ϳ� ���̴�.���� �� �Լ��� ȣ���� ���ο� inline�� �� �ִ�.

		//�� ��� �Ƹ� inline���� ���� ��
		setSoundB(Sound::Siren);
		//�ݸ�, setSoundB�� ��� setAlarm�� ���� �����͸� ���ڷ� �Ѱ����Ƿ� ���ο����� �ش� �Լ��� �ּҸ� Ÿ�� ���� ȣ���ؾ߸� �ϹǷ� 
		//�����Ϸ��� inline���� ����� �����.�� ������ ���ٰ� std::bind���� �� �� ���� �ڵ带 ������ ���ɼ��� ��������.

		//�� �ٸ� ������ ���캸��.�Ʒ� �ڵ�� � ���� val�� ���� ���� lowVal�� highVal ���̿� �ִ��� Ȯ���ϴ� �Լ��� ���ٷ� �ۼ��� ���� �ڵ��.

		int lowVal, highVal;
		//C++11
		auto betweenL =
		[lowVal, highVal]
		(int val)
		{ return lowVal <= val && val <= highVal; };

		//C++14
		auto betweenL_ =
			[lowVal, highVal]
		(const auto& val)
		{ return lowVal <= val && val <= highVal; };
		//�Ʒ��� ���� ������ �ڵ带 std::bind�� § �ڵ��.

		using namespace std::placeholders;

		//C++11
		auto betweenB =
			std::bind(std::logical_and<bool>(),
				std::bind(std::less_equal<int>(), lowVal, _1),
				std::bind(std::less_equal<int>(), _1, highVal));

		//C++14
		auto betweenB_ =
			std::bind(std::logical_and<>(),
				std::bind(std::less_equal<>(), lowVal, _1),
				std::bind(std::less_equal<>(), _1, highVal));
		//C++11�̰� 14�� ���� ǥ������ ��찡 �ڵ��� ���̵� ª�� ������������ �� �پ����� ���� ������ ���� �� ���� ���̴�.

		//std::bind�� ���ٿ� ���� �ڵ��� ������ ������ ������ �����ϱ� �����.�Ʒ� �ڵ带 ����.


		Widget w;

		using namespace std::placeholders;

		auto compressRateB = std::bind(compress, w, _1);
		//���⼭ w�� std::bind�� �ѱ� �� w�� compressRateB ��ü �ȿ� ����Ǿ��ٰ� ���߿� compress �Լ��� ȣ���� �� ���� ���̴�.
		//�� ��, w�� ���۷����� ����ɱ�, ������ ����ɱ� ? �� ���̴� ũ��.
		//���۷����� ����� ��� �ܺο��� �Ͼ�� �������� ��ȭ�� ������ ������ ������ ����� ��� ������ ���� �ʱ� �����̴�.
		//�ϴ� ������ ������ ����ȴ��̴�.������ �̰� ������ ����ȴٴ� �� �˷��� �׳� std::bind�� ���� �׷��� �����Ѵٴ� ���� �ܿ�� �־�߸� �Ѵ�.
		//�ݸ� ������ ����, ������ ĸ���ϴ��� ���۷����� ĸ���ϴ��� ��Ȯ�ϰ� ��õǾ� �ִ�.

		auto compressRateL =
		[w](CompLevel lev)
		{ return compress(w, lev); };
		//�� �� ĸ�Ŀ��� w��� ���� ������ �̰� ������ ĸ�ĵ� ���̴�(&w ��� ���۷���).�̷� ��ü��� �Լ��� �Ű������� �־�� �����ϴ�.

		//���ڴ� ������ ���޵ȴ�
		compressRateL(CompLevel::High);
		//�ݸ�, std::bind�� ���, �Լ� ȣ��� �Ѿ�� ���ڰ� ������ �Ѿ���� ���۷����� �Ѿ���� �˱� �����.
		//���� std::bind�� ���� ��� �����ϴ��� �ܿ�� �־�߸� �Ѵ�(bind object�� �Ѿ�� ��� ���ڵ��� ���۷����� �Ѿ��.perfect forwarding�� �ϱ� ����).

		//��? ���۷���? �ڵ常 ���� �� �� �ִ� ����� ����.
		compressRateB(CompLevel::High);
		//�ᱹ ���ٿ� ���ϸ� std::bind�� ����ϴ� �� ������ ���̳�, ǥ�� �ɷ� ���̳�, ȿ���� ���̳� ���ٺ��� ���� ���� �ϳ��� ���� ���̴�.
		//Ư�� C++ 14���ʹ� std::bind�� ���� ��ſ� �� ���� �ո����� ��Ȳ�� �� �ϳ��� �������� �ʴ´�.
		//C++11�� ��� ���� ��ſ� std::bind�� ����� ���� ��Ȳ�� 2���� ���� �ִ�.

		//Move Capture C++11�� ���ٴ� move capture�� �������� �ʴ´�.�̸� �䳻���� ���� ���ٿ� std::bind�� �������� ��Ȳ�� �ִ�.������ C++14���ʹ� ���� �߰��� ����� init capture�� ����ϸ� �Ǳ� ������ std::bind�� �� �ʿ䰡 ����.�ڼ��� ������ item 32 ����.

		//Polymorphic function object std::bind�� ��� �Լ� ȣ���� perfect forwarding�� ���� �����ϱ� ������, � ������ ���ڵ� �޾Ƶ��� �� �ִ�.�̰� ���ø�ȭ�� �Լ� ȣ�� �����ڿ� ��ü�� bind�� �� �����ϴ�.

		PolyWidget pw;
		auto boundPW = std::bind(pw, _1);
		//�̷��� boundPW�� �����, ���� �ٸ� Ÿ���� ���ڸ� �ѱ� �� �ִ�.

		boundPW(1930); //PolyWidget::operator()�� int �ѱ�

		boundPW(nullptr); //PolyWidget::operator()�� nullptr �ѱ�

		boundPW("Rosebud"); //PolyWidget::operator()�� string literal �ѱ�
		//C++11�� ���ٿ����� �̷� ����� ������ ����� ����.�ݸ� C++14������ auto �Ű������� ���� ���ٸ� ����ϱ� ������ std::bind�� �� �ʿ䰡 ����.

		auto boundPW_ = [pw](const auto& param)
		{ pw(param); };

//- std::bind�� ����ϴ� �ͺ��� ���ٰ� �� �б� ���� ǥ������ ����. �׸��� �� ȿ������ �� �ִ�.
//- C++14�� �ƴ� C++11������ �̵� �������� �����ϰų� ��ü�� ���ø�ȭ�� �Լ� ȣ�� �����ڿ�
//	������ �Ҷ� std::bind�� ������ �� �ִ�.

		return 0;
}