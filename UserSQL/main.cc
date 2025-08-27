#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <iostream>

using namespace drogon;
using namespace drogon::orm;

int main() {
    // Load config file (must contain DB credentials)
    app().loadConfigFile("../config.json");

    // Set listener (optional, for HTTP APIs later)
    app().addListener("0.0.0.0", 5555);

    // Start Drogon event loop
    app().run();
    return 0;
}

