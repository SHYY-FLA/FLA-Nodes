#include <gtest/gtest.h>
#include <network/network.h>

TEST(NetworkTest, CreateUDPSocket) {
    int sock = createUDP();
    ASSERT_NE(sock, -1);
    close(sock);
}

TEST(NetworkTest, BindPort) {
    int sock = createUDP();
    EXPECT_EQ(bind(sock, 74638), 0);
    close(sock);
}

TEST(NetworkTest, SendReceivePacket) {
    // 테스트 환경 설정
    int sender = createUDP();
    int receiver = createUDP();
    bind(receiver, 9102);

    // 패킷 전송 및 수신 검증
    sendUDP(sender, "127.0.0.1", 9102, "TEST");
    // 수신 검증 로직 추가 필요
}
