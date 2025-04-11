//
// Created by 강시우 on 4/10/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int createTCP();

int createUDP();

int bindSocket(int sockfd, int port);

void listeningUDP(int sockfd, std::atomic<bool>& isRunning);

void sendUDP(int sockfd, const std::string& ip, int port, const std::string& message);

int closeSocket(int sockfd);

#ifdef __cplusplus
}
#endif