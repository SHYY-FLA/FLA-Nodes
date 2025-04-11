//
// Created by 강시우 on 4/10/25.
//

#pragma once

int createTCP();

int createUDP();

int bind(int sockfd, int port);

void listeningUDP(int sockfd, std::atomic<bool>& isRunning);

void sendUDP(int sockfd, const std::string& ip, int port, const std::string& message);

int close(int sockfd);