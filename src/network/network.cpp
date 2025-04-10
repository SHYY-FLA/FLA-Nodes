//
// Created by 강시우 on 4/10/25.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include <network/network.h>

void printSocketCreation(int sock);

int create() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "소켓 생성 실패: " << strerror(errno) << std::endl;
    }

    printSocketCreation(sock);

    return sock;
}

void printSocketCreation(int sockfd) {
    if (sockfd != -1) {
        std::cout << "✅ 소켓 생성 성공 | 디스크립터: " << sockfd
                  << " | 프로토콜: TCP" << std::endl;
    }
}