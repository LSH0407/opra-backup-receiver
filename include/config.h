#pragma once
#include <string>
#include <unordered_map>

struct AppConfig {
    std::string multicastAddress;
    unsigned short port{0};
    std::string interfaceAddress; // optional
};

// key=value 형식의 간단한 설정 파서 (주석: #)
AppConfig loadAppConfigFromFile(const std::string& filename);
