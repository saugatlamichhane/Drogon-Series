#pragma once

#include <drogon/HttpController.h>
#include <drogon/drogon.h>

namespace api
{
namespace v1
{
    class User : public drogon::HttpController<User>
    {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(User::getInfo, "/{id}", drogon::Get);          // GET /api/v1/User/{id}
        METHOD_ADD(User::newUser, "/{name}", drogon::Post);       // POST /api/v1/User/{name}
        METHOD_ADD(User::updateUser, "/{id}", drogon::Put);       // PUT /api/v1/User/{id}
        METHOD_ADD(User::deleteUser, "/{id}", drogon::Delete);    // DELETE /api/v1/User/{id}
        METHOD_LIST_END

        void getInfo(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                     int userId) const;

        void newUser(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                     std::string&& userName);

        void updateUser(const drogon::HttpRequestPtr& req,
                        std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                        int userId);

        void deleteUser(const drogon::HttpRequestPtr& req,
                        std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                        int userId);

        User();
    private:
        drogon::orm::DbClientPtr dbClient;
    };
}
}

