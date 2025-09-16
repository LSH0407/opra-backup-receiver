#include "config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static inline std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

AppConfig loadAppConfigFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("config not found: " + filename);
    }
    std::unordered_map<std::string, std::string> kv;
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string k = trim(line.substr(0, pos));
        std::string v = trim(line.substr(pos + 1));
        kv[k] = v;
    }
    AppConfig cfg{};
    auto itAddr = kv.find("multicast");
    if (itAddr != kv.end()) cfg.multicastAddress = itAddr->second; else cfg.multicastAddress.clear();
    auto itPort = kv.find("port");
    if (itPort != kv.end()) cfg.port = static_cast<unsigned short>(std::stoi(itPort->second)); else cfg.port = 5000;
    auto itIf = kv.find("interface");
    if (itIf != kv.end()) cfg.interfaceAddress = itIf->second; else cfg.interfaceAddress.clear();
    return cfg;
}
