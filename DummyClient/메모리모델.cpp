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


	//flag.is_lock_free(); //lock이 없는게 true=1
	flag.store(true,memory_order_seq_cst);

	bool val = flag.load(memory_order_seq_cst);
	
	//이전 flag값을 prev에 넣고, flag값을 수정
	{
		/*
		bool prev = flag;
		flag=true;
		flag값을 prev에 저장하는 시점에 값이 변경될경우 이전값을 보장 x , 이전값을 보존하면서  값을 수정하는 과정을  원자적으로 진행해야함!

		*/
		bool prev=flag.exchange(true);
		
	}
	// CAS (Compare-And-Swap) 조건부 수정
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
		//다른 쓰레드의 interruption을 받아서 위의 if조건을 만족했음에도 진행하지 못하고 중간에 실패할수 있음.
		// srtrong은 혹시라도 실패시 다시 루프를 돌아 성공하게끔 반복함.
		if (flag == expected)
		{
			if (묘한 상황)
				return false;
			flag = desired;
			return true;
		}
		else
		{
			expected = flag;
			return false;
		}


		//따라서 while문안에 넣어서 그런상황 방지.
		while (true)
		{
			bool expected = false;
			bool desired = true;
			flag.compare_exchange_weak(expected, desired);
		}



	}

}