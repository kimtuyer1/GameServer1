// TestP.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>

//IOCP에서  IOCP 완료 처리 스레드 / 작업 스레드 나누어 처리

// IOCP 스레드에서 사용되는 함수
DWORD WINAPI IOCPThread(LPVOID lpParam) {
    HANDLE hIOCP = (HANDLE)lpParam;
    DWORD dwBytesTransferred;
    ULONG_PTR ulKey;
    LPOVERLAPPED lpOverlapped;

    while (true) {
        // IOCP 커널 객체에서 완료된 I/O 작업 대기
        BOOL bResult = GetQueuedCompletionStatus(hIOCP, &dwBytesTransferred, &ulKey, &lpOverlapped, INFINITE);
        if (!bResult) {
            // 에러 처리
            continue;
        }

        // 작업 스레드에서 처리할 데이터 큐에 넣기
        MyDataQueue.push(reinterpret_cast<MyData*>(lpOverlapped->hEvent));
    }

    return 0;
}

// 작업 스레드에서 사용되는 함수
DWORD WINAPI WorkerThread(LPVOID lpParam) {
    while (true) {
        // 데이터 큐에서 작업 꺼내오기
        MyData* pData = MyDataQueue.pop();
        if (!pData) {
            // 큐에서 데이터가 없는 경우 대기
            continue;
        }

        // 작업 처리
        // ...
        // 작업 완료 후, MyData 객체 해제
        delete pData;
    }

    return 0;
}

int main() {
    // IOCP 객체 생성
    HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    // IOCP 스레드 생성
    HANDLE hIOCPThread = CreateThread(NULL, 0, IOCPThread, hIOCP, 0, NULL);

    // 작업 스레드 생성
    for (int i = 0; i < NUM_WORKER_THREADS; ++i) {
        HANDLE hWorkerThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);
    }

    // I/O 작업 등록
    MyData* pData = new MyData();
    // ...
    // I/O 작업 등록 후, IOCP 객체에 등록
    CreateIoCompletionPort(pData->hFile, hIOCP, reinterpret_cast<ULONG_PTR>(pData), 0);

    // ...
    // 프로그램 종료 시, 스레드 종료 및 자원 해제
    TerminateThread(hIOCPThread, 0);
    CloseHandle(hIOCPThread);
    // ...
    return 0;
}

/*
위 코드에서 IOCP 스레드는 IOCPThread 함수에서 실행되고, 작업 스레드는 WorkerThread 함수에서 실행됩니다. IOCP 스레드는 GetQueuedCompletionStatus 함수를 호출하여 IOCP 커널 객체에서 완료된 I/O 작업을 대기하고, 작업 스레드는 MyDataQueue 큐에서 데이터를 꺼내와 작업을 처리합니다. I/O 작업은 CreateIoCompletionPort 함수를 사용하여 IOCP 객체에 등록됩니다.

*/

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
