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

	//메모리 모델 (정책)
	/*
	1. Sequentially Consistent (seq-cst)
	2. Acquire-Release (consume, acquire, release, acq_rel)
	3. Relaxed (relaxed)

	1( seq_cst) 가장 엄격=컴파일러 최적화 여지 적음 =직관적)
	 가시성 / 코드재배치 둘다 해결

	2 acqure-relaease
		release 이전의 명령어들은 release이후로 코드 재배치 방지.
		acquire 로 같은 변수를 읽는 스레드가 존재시
		release이전 명령들이  acquire-> 이후 관찰가능 (가시성보장)


	3 relaxed  자유롭다- 컴파일러 최적화 여지 많음 =직관적 x)  코드재배치 가능성 큼. 가시성 해결 x
	동일 객체 대한 동일 관전 순서만 보장.



	//인텔 AMD 경우 애당초 칩 자체가 순차적 일관성 보장
	seq-cst 써도 별다른 부하가 없음.
	ARM의 경우는 차이 있다함.
	
	*/


}