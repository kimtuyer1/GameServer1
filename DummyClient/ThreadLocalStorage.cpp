#include "pch.h"




//c+11 
//_declspec(thread)int32 value;

thread_local int32 LThreadId = 0;


void ThreadMain(int32 threadId)
{
	LThreadId = threadId;

	while (true)
	{
		cout << LThreadId << endl; 
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;
	for (int32 i = 0; i < 10; i++)
	{
		int32 threadid = i + 1;
		threads.push_back(thread(ThreadMain,threadid));
	}

	for (thread& t : threads)
		t.join();


}