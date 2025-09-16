#include "receiver.h"
#include <iostream>
#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
#else
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <unistd.h>
  #define closesocket close
#endif

#ifndef IP_ADD_MEMBERSHIP
#define IP_ADD_MEMBERSHIP 35
#endif

static int joinMulticastIfNeeded(int sock, const ReceiverConfig& cfg) {
    if (cfg.multicastAddress.empty()) return 0;
    ip_mreq mreq{};
    mreq.imr_multiaddr.s_addr = inet_addr(cfg.multicastAddress.c_str());
    mreq.imr_interface.s_addr = cfg.interfaceAddress.empty() ? htonl(INADDR_ANY) : inet_addr(cfg.interfaceAddress.c_str());
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0) {
        std::perror("setsockopt(IP_ADD_MEMBERSHIP)");
        return -1;
    }
    return 0;
}

int runUdpReceiver(const ReceiverConfig& cfg) {
#ifdef _WIN32
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif
    int sockfd = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));\n    // enlarge receive buffer for burst traffic\n    { int buf=1<<20; setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&buf, sizeof(buf)); }
    if (sockfd < 0) { std::perror("socket"); return 1; }

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(cfg.port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::perror("bind");
        closesocket(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (!cfg.multicastAddress.empty()) {
        if (joinMulticastIfNeeded(sockfd, cfg) != 0) {
            closesocket(sockfd);
#ifdef _WIN32
            WSACleanup();
#endif
            return 1;
        }
    }

    std::cout << "[receiver] listening on "
              << (cfg.multicastAddress.empty()? std::string("0.0.0.0") : cfg.multicastAddress)
              << ":" << cfg.port
              << (cfg.interfaceAddress.empty()? "" : std::string(" via ") + cfg.interfaceAddress)
              << std::endl;

    char buffer[2048];
    for (;;) {
        sockaddr_in src{};
        socklen_t srclen = sizeof(src);
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&src, &srclen);
        if (n < 0) { std::perror("recvfrom"); break; }
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &src.sin_addr, ip, sizeof(ip));
        std::cout << "[receiver] got " << n << " bytes from " << ip << ":" << ntohs(src.sin_port) << std::endl;
        // 파싱/전달 직전까지만. 실제 전송은 별도 프로그램에서 처리.
    }

    closesocket(sockfd);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

