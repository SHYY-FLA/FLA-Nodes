#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>

extern "C" {
    #include <network/network.h>
}

class NetworkTest : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }

    bool IsPortAvailable(int port) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) return false;

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bool available = (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0);
        close(sock);
        return available;
    }

    int FindAvailablePort() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) return -1;

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = 0;

        if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            close(sock);
            return -1;
        }

        sockaddr_in bound_addr{};
        socklen_t addrlen = sizeof(bound_addr);
        if (getsockname(sock, (struct sockaddr*)&bound_addr, &addrlen) == -1) {
            close(sock);
            return -1;
        }

        int port = ntohs(bound_addr.sin_port);
        close(sock);
        return port;
    }
};

TEST_F(NetworkTest, CreateTCP) {
    int sock = createTCP();
    ASSERT_NE(sock, -1) << "TCP 소켓 생성 실패";

    int type;
    socklen_t len = sizeof(type);
    ASSERT_EQ(getsockopt(sock, SOL_SOCKET, SO_TYPE, &type, &len), 0);
    EXPECT_EQ(type, SOCK_STREAM) << "생성된 소켓이 TCP 타입이 아님";

    close(sock);
}

TEST_F(NetworkTest, CreateUDP) {
    int sock = createUDP();
    ASSERT_NE(sock, -1) << "UDP 소켓 생성 실패";

    int type;
    socklen_t len = sizeof(type);
    ASSERT_EQ(getsockopt(sock, SOL_SOCKET, SO_TYPE, &type, &len), 0);
    EXPECT_EQ(type, SOCK_DGRAM) << "생성된 소켓이 UDP 타입이 아님";

    close(sock);
}

TEST_F(NetworkTest, BindSocket) {
    int port = FindAvailablePort();
    ASSERT_GT(port, 0) << "테스트용 사용 가능한 포트를 찾을 수 없음";

    int sock = createUDP();
    ASSERT_NE(sock, -1) << "UDP 소켓 생성 실패";

    EXPECT_EQ(bindSocket(sock, port), 0) << "UDP 소켓 바인딩 실패";

    sockaddr_in addr{};
    socklen_t addrlen = sizeof(addr);
    ASSERT_EQ(getsockname(sock, (struct sockaddr*)&addr, &addrlen), 0);
    EXPECT_EQ(ntohs(addr.sin_port), port) << "바인딩된 포트가 일치하지 않음";

    close(sock);
}

TEST_F(NetworkTest, UDPSendReceive) {
    int port = FindAvailablePort();
    ASSERT_GT(port, 0) << "테스트용 사용 가능한 포트를 찾을 수 없음";

    int server_sock = createUDP();
    ASSERT_NE(server_sock, -1) << "서버 UDP 소켓 생성 실패";
    ASSERT_EQ(bindSocket(server_sock, port), 0) << "서버 UDP 소켓 바인딩 실패";

    int client_sock = createUDP();
    ASSERT_NE(client_sock, -1) << "클라이언트 UDP 소켓 생성 실패";

    std::atomic<bool> isRunning(true);
    std::thread server_thread([&]() {
        listeningUDP(server_sock, isRunning);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string test_message = "테스트 메시지";
    sendUDP(client_sock, "127.0.0.1", port, test_message);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    isRunning = false;
    if (server_thread.joinable()) {
        server_thread.join();
    }

    closeSocket(client_sock);
    closeSocket(server_sock);
}
