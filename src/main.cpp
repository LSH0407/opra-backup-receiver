#include "receiver.h"`r`n#include "config.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    std::string ex = std::getenv("EXCHANGE") ? std::getenv("EXCHANGE") : "NASDAQ";`r`n    AppConfig app = loadAppConfigFromFile(ex + ".config");`r`n    ReceiverConfig cfg{};`r`n    cfg.multicastAddress = app.multicastAddress;`r`n    cfg.port = app.port;`r`n    cfg.interfaceAddress = app.interfaceAddress;`r`n    std::cout << "[main] starting receiver...\n";`r`n    return runUdpReceiver(cfg);
}


