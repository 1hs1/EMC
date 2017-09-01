//auto�� ��ġ ���� �������� ������ ������ ����� ������ �ʱ�ġ�� ����϶�

#pragma once

#include <vector>
//�׸�5 auto�� ����Ͽ� ������ �����ϸ� ���� ������ �ִ�.������ auto�� ���� ������ ���� ����ġ ���� ���

//Widget�� �޾� vector<bool>�� �����ִ� �Լ�
std::vector<bool> features(const Widget& w);

//5��° ��Ʈ�� Widget�� �켱������ ��Ÿ����.
Widget w;
bool highPriority = features(w)[5]; //w�� �켱���� ������?
processWidget(w, highPriority);

//����� ���� -> auto ���
auto highPriority = features(w)[5]; // ������ �ൿ
//auto�� ����ϴ� heightPriority�� bool�� �ƴϴ�.
//std::vector<bool>�� operator[]�� �����ִ� ���� �� �����̳��� �� ��ҿ� ���� ������ �ƴ϶�,
//std::vector<bool>::reference ������ ��ü�̴�.


bool highPriority = features(w)[5]; //����
//features�� std::vector<bool>�� �����ָ�, �� ��ü�� ���� operator[]�� ȣ��ȴ�. 
//operator[]�� std::vector<bool>::reference��ü�� �����ָ�, 
//�� ��ü�� �Ϲ������� bool�� ��ȯ�Ǿ heightPriority�ʱ�ȭ�� ���̸�, 
//��������� std::vector<bool>�� 5�� ��Ʈ�� ���� ������ �ȴ�.

auto highPriority = features(w)[5]; //������� ��
//�̹����� features�� std::vector<bool>��ü�� �����ָ�, 
//�� ��ü�� ���� operator[]�� ȣ��ǰ� �� operator[]�� std::vector<bool>::reference��ü�� �����ش�.
//���� auto�� ���� hightPriority�� ������ �����ȴ�.


// ����ġ ���� ����� ����Ǵ� ����
//features ȣ���� �ӽ� std::vector<bool> ��ü�� �����ָ�, 
//�� �ӽð�ü temp�� ���� ȣ��� operator[]�� std::vector<bool>::reference ��ü�� �����ָ�, 
//�� ��ü���� temp�� �����ϴ� ��Ʈ���� ���� �ڷᱸ���� �� ���带 ����Ű�� �����Ϳ� 
//�� ���忡�� ������ ��Ʈ�� �ش��ϴ� ��Ʈ �������� ��� �ִ�.
//���� highPriority ���� temp ���� �ش� ���带 ����Ű�� �����Ϳ� 5�� ��Ʈ�� �������� ��Եȴ�.
//�׷��� temp�� �ӽð�ü�� ������ ������ �ı��Ǹ�, ��������� highPriority�� �����ʹ� ��۸� �����Ͱ� �ȴ�.


// �븮�� Ŭ����
// std::shared_ptr -> �巯��
// std::vector<bool>::reference -> ������

// �븮�� ���� �߷�
// �������, �����Ϲ��� �����, ���� �˻� �����...


//���
//std::vector<bool>::referenceó�� '������ �ʴ�' �븮�� Ŭ������ auto�� �� ���� �ʴ´�.
//���� �� ������ �ذ��ϱ� ���ؼ��� auto�� �ٸ� ������ �����ϵ��� �����ϴ� ���̴�.���� ��� �ʱ�ġ ���뱸�� �׷��� �ع��� �ϳ��̴�.
//���� ��� �ʱ�ġ ���뱸������ ������ auto�� �����ϵ�, �ʱ�ȭ ǥ������ ������ auto�� �����ϱ� ���ϴ� �������� ĳ�����Ѵ�.
Matrix sum = m1 + m2 + m3 + m4;
Sum<Sum<Sum<Matrix, Matrix>, Matrix>, Matrix>;
auto sumMatrix = static_cast<Matrix>(m1 + m2 + m3 + m4);

auto hightPriority = static_cast<bool>(features(w)[5]);


//- "������ �ʴ�" �븮�� ���� ������ auto�� �ʱ�ȭ ǥ������ ������ "�߸�" ���� �� �� �ִ�.
//- ���� ��� �ʱ�ġ ���뱸�� auto�� ���ϴ� ������ �����ϵ��� �����Ѵ�.