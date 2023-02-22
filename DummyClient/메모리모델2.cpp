#include "pch.h"


atomic<bool> ready;
int32 value;

void Producer()
{

	value = 10;
	ready.store(true, memory_order_seq_cst);

}
void Consumer()
{
	while (ready.load(memory_order_seq_cst) == false)
	{
	}
	cout << value << endl;;
}


int main()
{

	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	//�޸� �� (��å)
	/*
	1. Sequentially Consistent (seq-cst)
	2. Acquire-Release (consume, acquire, release, acq_rel)
	3. Relaxed (relaxed)

	1( seq_cst) ���� ����=�����Ϸ� ����ȭ ���� ���� =������)
	 ���ü� / �ڵ����ġ �Ѵ� �ذ�

	2 acqure-relaease
		release ������ ��ɾ���� release���ķ� �ڵ� ���ġ ����.
		acquire �� ���� ������ �д� �����尡 �����
		release���� ��ɵ���  acquire-> ���� �������� (���ü�����)


	3 relaxed  �����Ӵ�- �����Ϸ� ����ȭ ���� ���� =������ x)  �ڵ����ġ ���ɼ� ŭ. ���ü� �ذ� x
	���� ��ü ���� ���� ���� ������ ����.



	//���� AMD ��� �ִ��� Ĩ ��ü�� ������ �ϰ��� ����
	seq-cst �ᵵ ���ٸ� ���ϰ� ����.
	ARM�� ���� ���� �ִ���.
	
	*/


}