#include "receiver.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    ReceiverConfig cfg{};
    cfg.multicastAddress = argc > 1 ? argv[1] : "233.0.0.1";
    cfg.port = static_cast<unsigned short>(argc > 2 ? std::stoi(argv[2]) : 5000);
    cfg.interfaceAddress = argc > 3 ? argv[3] : "";

    std::cout << "[main] starting receiver...\n";
    return runUdpReceiver(cfg);
}
