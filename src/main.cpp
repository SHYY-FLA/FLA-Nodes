#include <future>
#include <iostream>

#include <version.h>
#include <network/network.h>

using namespace std;

int main() {

    cout << version() << endl;

    int socket = createUDP();
    if (bind(socket, 74638) != 0) return -1;

    // 종료 플래그 설정
    std::atomic<bool> isRunning{true};

    // 리스닝 작업을 별도 스레드로 실행
    std::thread listeningThread([&]() {
        listeningUDP(socket, isRunning);
    });

    cout << "메인 스레드는 다른 작업을 수행합니다. 종료하려면 Enter를 누르세요.\n";

    cin.get();
    isRunning = false;

    cout << "stop" << endl;

    listeningThread.join();

    cout << "프로그램이 종료되었습니다.\n";
    return 0;
}