#include "pch.h"


atomic<int64>num;
atomic<bool> flag;

void Thread_1()
{
	///num=1;
	num.store(1);
	num.store(1, memory_order::memory_order_seq_cst);

}
void Thread_2()
{
	///num=1;
	num.store(2);
	num.store(2, memory_order::memory_order_seq_cst);

}

void Thread_Observer()
{
	while (true)
	{
		//int64 value = num.load();
		int64 value = num.load(memory_order::memory_order_seq_cst);
	}
}

int main()
{


	//flag.is_lock_free(); //lock�� ���°� true=1
	flag.store(true,memory_order_seq_cst);

	bool val = flag.load(memory_order_seq_cst);
	
	//���� flag���� prev�� �ְ�, flag���� ����
	{
		/*
		bool prev = flag;
		flag=true;
		flag���� prev�� �����ϴ� ������ ���� ����ɰ�� �������� ���� x , �������� �����ϸ鼭  ���� �����ϴ� ������  ���������� �����ؾ���!

		*/
		bool prev=flag.exchange(true);
		
	}
	// CAS (Compare-And-Swap) ���Ǻ� ����
	{
		bool expected = false;
		bool desired = true;
		flag.compare_exchange_strong(expected,desired);

		if (flag == expected)
		{
			flag = desired;
			return true;
		}
		else
		{
			expected = flag;
			return false;
		}



		bool expected = false;
		bool desired = true;
		flag.compare_exchange_weak(expected, desired); 
		//�ٸ� �������� interruption�� �޾Ƽ� ���� if������ ������������ �������� ���ϰ� �߰��� �����Ҽ� ����.
		// srtrong�� Ȥ�ö� ���н� �ٽ� ������ ���� �����ϰԲ� �ݺ���.
		if (flag == expected)
		{
			if (���� ��Ȳ)
				return false;
			flag = desired;
			return true;
		}
		else
		{
			expected = flag;
			return false;
		}


		//���� while���ȿ� �־ �׷���Ȳ ����.
		while (true)
		{
			bool expected = false;
			bool desired = true;
			flag.compare_exchange_weak(expected, desired);
		}



	}

}