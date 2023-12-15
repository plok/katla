
#ifndef KATLA_HTTP_REQUEST_H
#define KATLA_HTTP_REQUEST_H

#include "katla/core/core.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <map>

namespace katla {

enum class HttpMethod
{
    Unknown,
    Get,
    Post,
    Put,
    Options,
    Delete,
    Head,
    Patch
};

enum class HttpStatusCode
{
    Continue = 100,
    Ok = 200,
    Created = 201,
    BadRequest = 400,
    NotFound = 404,
    InternalServerError = 500,
    ServiceUnavailable = 503
};

struct HttpRequest {
  HttpMethod method;
  std::string url;
  std::string contentType;
  int contentLength {0};
  std::vector<std::byte> payload;
};

class HttpRequestResult {
public:
    HttpRequestResult(const HttpRequest& request_) {
        request = request_;
        statusCode = katla::HttpStatusCode::InternalServerError;
        contentType = "application/json";

        // TODO not for public use!
        headers["Access-Control-Allow-Origin"] = "*";
        headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, PATCH, OPTIONS";
        headers["Access-Control-Allow-Headers"] = "X-Requested-With, content-type, Authorization";
    };
    virtual ~HttpRequestResult() = default;

    HttpRequest request {};
    HttpStatusCode statusCode {};
    std::map<std::string, std::string> headers;
    std::string contentType;
    std::vector<std::byte> payload;
};

} // namespace katla

#endif
