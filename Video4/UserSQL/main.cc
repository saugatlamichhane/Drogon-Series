#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <iostream>

using namespace drogon;
using namespace drogon::orm;

void setupCors()
{
    // Register sync advice to handle CORS preflight (OPTIONS) requests
    drogon::app().registerSyncAdvice([](const drogon::HttpRequestPtr &req)
                                         -> drogon::HttpResponsePtr {
        if (req->method() == drogon::HttpMethod::Options)
        {
            auto resp = drogon::HttpResponse::newHttpResponse();

            // Set Access-Control-Allow-Origin header based on the Origin
            // request header
            const auto &origin = req->getHeader("Origin");
            if (!origin.empty())
            {
                resp->addHeader("Access-Control-Allow-Origin", origin);
            }

            // Set Access-Control-Allow-Methods based on the requested method
            const auto &requestMethod =
                req->getHeader("Access-Control-Request-Method");
            if (!requestMethod.empty())
            {
                resp->addHeader("Access-Control-Allow-Methods", requestMethod);
            }

            // Allow credentials to be included in cross-origin requests
            resp->addHeader("Access-Control-Allow-Credentials", "true");

            // Set allowed headers from the Access-Control-Request-Headers
            // header
            const auto &requestHeaders =
                req->getHeader("Access-Control-Request-Headers");
            if (!requestHeaders.empty())
            {
                resp->addHeader("Access-Control-Allow-Headers", requestHeaders);
            }

            return std::move(resp);
        }
        return {};
    });

    // Register post-handling advice to add CORS headers to all responses
    drogon::app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr &req,
           const drogon::HttpResponsePtr &resp) -> void {
            // Set Access-Control-Allow-Origin based on the Origin request
            // header
            const auto &origin = req->getHeader("Origin");
            if (!origin.empty())
            {
                resp->addHeader("Access-Control-Allow-Origin", origin);
            }

            // Reflect the requested Access-Control-Request-Method back in the
            // response
            const auto &requestMethod =
                req->getHeader("Access-Control-Request-Method");
            if (!requestMethod.empty())
            {
                resp->addHeader("Access-Control-Allow-Methods", requestMethod);
            }

            // Allow credentials to be included in cross-origin requests
            resp->addHeader("Access-Control-Allow-Credentials", "true");

            // Reflect the requested Access-Control-Request-Headers back
            const auto &requestHeaders =
                req->getHeader("Access-Control-Request-Headers");
            if (!requestHeaders.empty())
            {
                resp->addHeader("Access-Control-Allow-Headers", requestHeaders);
            }
        });
}

int main() {
    // Load config file (must contain DB credentials)
    app().loadConfigFile("../config.json");
    setupCors();
    // Set listener (optional, for HTTP APIs later)
    app().addListener("0.0.0.0", 5555);

    // Start Drogon event loop
    app().run();
    return 0;
}

