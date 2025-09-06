#include "User.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpResponse.h>
#include <drogon/orm/Mapper.h>
#include <models/Users.h>   // generated ORM model

using namespace drogon::orm;
using namespace drogon_model::mydb;   // <-- adjust "mydb" to your real database name

namespace api {
namespace v1 {

    User::User() {
        LOG_DEBUG << "User controller initialized!";
    }

    void User::getInfo(const drogon::HttpRequestPtr& req,
                       std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                       int userId) const {
        auto client = drogon::app().getDbClient();
        Mapper<Users> mp(client);

        mp.findByPrimaryKey(
            userId,
            [callback](Users user) {
                Json::Value result;
                result["userId"] = user.getValueOfId();
                result["name"] = user.getValueOfUsername();
                callback(drogon::HttpResponse::newHttpJsonResponse(result));
            },
            [callback](const DrogonDbException &e) {
                Json::Value error;
                error["error"] = e.base().what();
                callback(drogon::HttpResponse::newHttpJsonResponse(error));
            });
    }

    void User::newUser(const drogon::HttpRequestPtr& req,
                       std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                       std::string&& userName) {
        auto client = drogon::app().getDbClient();
        auto mp = drogon::orm::Mapper<Users> (client);

        Users newUser;
        newUser.setUsername(userName);

        mp.insert(
            newUser,
            [callback](Users user) {
                Json::Value result;
                result["userId"] = user.getValueOfId();
                result["name"] = user.getValueOfUsername();
                result["message"] = "User created successfully!";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(result);
                resp->setStatusCode(drogon::k201Created);
                callback(resp);
            },
            [callback](const DrogonDbException &e) {
                Json::Value error;
                error["db_error"] = e.base().what();
                callback(drogon::HttpResponse::newHttpJsonResponse(error));
            });
    }

    void User::updateUser(const drogon::HttpRequestPtr& req,
                          std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                          int userId) {
        auto client = drogon::app().getDbClient();
        Mapper<Users> mp(client);

        auto json = req->getJsonObject();
        if (!json || !(*json).isMember("name")) {
            Json::Value error;
            error["error"] = "Missing 'name' field";
            callback(drogon::HttpResponse::newHttpJsonResponse(error));
            return;
        }

        std::string newName = (*json)["name"].asString();

        mp.findByPrimaryKey(
            userId,
            [callback, newName, client](Users user) {
                Mapper<Users> mp2(client);
                user.setUsername(newName);
                mp2.update(
                    user,
                    [callback](size_t) {
                        Json::Value result;
                        result["message"] = "User updated successfully";
                        callback(drogon::HttpResponse::newHttpJsonResponse(result));
                    },
                    [callback](const DrogonDbException &e) {
                        Json::Value error;
                        error["db_error"] = e.base().what();
                        callback(drogon::HttpResponse::newHttpJsonResponse(error));
                    });
            },
            [callback](const DrogonDbException &e) {
                Json::Value error;
                error["error"] = e.base().what();
                callback(drogon::HttpResponse::newHttpJsonResponse(error));
            });
    }

    void User::deleteUser(const drogon::HttpRequestPtr& req,
                          std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                          int userId) {
        auto client = drogon::app().getDbClient();
        Mapper<Users> mp(client);

        mp.deleteByPrimaryKey(
            userId,
            [callback](size_t rows) {
                Json::Value result;
                if (rows > 0)
                    result["message"] = "User deleted successfully";
                else
                    result["error"] = "User not found";
                callback(drogon::HttpResponse::newHttpJsonResponse(result));
            },
            [callback](const DrogonDbException &e) {
                Json::Value error;
                error["db_error"] = e.base().what();
                callback(drogon::HttpResponse::newHttpJsonResponse(error));
            });
    }

}
}

