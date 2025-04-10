//
// Created by 강시우 on 4/10/25.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>

#include <network/network.h>

void printSocketCreation(int sock);
void printBindInfo(int sockfd);

int createTCP() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "소켓 생성 실패: " << strerror(errno) << std::endl;
    }

    printSocketCreation(sock);

    return sock;
}

int createUDP() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "소켓 생성 실패: " << strerror(errno) << std::endl;
    }

    printSocketCreation(sock);

    return sock;
}

void printSocketCreation(int sockfd) {
    if (sockfd != -1) {
        int type;
        socklen_t optlen = sizeof(type);

        // 소켓 타입 확인
        if (getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &type, &optlen) == 0) {
            std::string protocol = (type == SOCK_STREAM) ? "TCP" : (type == SOCK_DGRAM) ? "UDP" : "알 수 없음";
            std::cout << "✅ 소켓 생성 성공 | 디스크립터: " << sockfd
                      << " | 프로토콜: " << protocol << std::endl;
        } else {
            std::cerr << "소켓 타입 조회 실패: " << strerror(errno) << std::endl;
        }
    } else {
        std::cerr << "📛 소켓 생성 실패" << std::endl;
    }
}

int bind(int sockfd, int port) {
    sockaddr_in serverAddr{};
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
        printBindInfo(sockfd);
        return 0;
    }
    return -1;
}

void printBindInfo(int sockfd) {
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);

    if(getsockname(sockfd, (struct sockaddr*)&addr, &len) == 0) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);

        std::cout << "🔗 바인드 정보 | IP: " << ip
                  << " | 포트: " << ntohs(addr.sin_port)
                  << " | 프로토콜: "
                  << (addr.sin_family == AF_INET ? "IPv4" : "IPv6")
                  << std::endl;
    } else {
        std::cerr << "바인드 정보 조회 실패: " << strerror(errno) << std::endl;
    }
}

void listeningUDP(int sockfd) {
    char buffer[65536]; // 최대 UDP 패킷 사이즈
    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);

    // 소켓의 바인딩된 포트 조회
    sockaddr_in serverAddr{};
    socklen_t serverAddrLen = sizeof(serverAddr);
    if (getsockname(sockfd, (struct sockaddr*)&serverAddr, &serverAddrLen) == -1) {
        std::cerr << "📛 바인드된 포트 조회 실패: " << strerror(errno) << std::endl;
        return;
    }
    int port = ntohs(serverAddr.sin_port); // 네트워크 바이트 오더 → 호스트 바이트 오더

    std::cout << "\n🎧 UDP 리스닝 시작 | 바인드된 포트: " << port << std::endl;

    while (true) {
        ssize_t recvLen = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                   (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (recvLen == -1) {
            std::cerr << "📛 수신 오류: " << strerror(errno) << std::endl;
            continue;
        }

        // 송신자 정보 출력
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        std::cout << "\n🔔 수신 패킷 | 출처: " << clientIP
                  << ":" << ntohs(clientAddr.sin_port)
                  << " | 크기: " << recvLen << " bytes" << std::endl;

        // HEX 덤프 출력
        std::cout << "📦 패킷 내용 (HEX):" << std::endl;
        for (int i = 0; i < recvLen; ++i) {
            printf("%02X ", (unsigned char)buffer[i]);
            if ((i + 1) % 16 == 0) std::cout << std::endl;
        }
        std::cout << "\n\n";
    }
}