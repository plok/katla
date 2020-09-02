
#ifndef KATLA_HTTP_REQUEST_H
#define KATLA_HTTP_REQUEST_H

#include "katla/core/core.h"

#include <chrono>
#include <libwebsockets.h>
#include <memory>
#include <string>
#include <vector>
#include <variant>

namespace katla {

enum class HttpMethod
{
    Unknown,
    Get,
    Post,
    Put,
    Options
};

enum class HttpStatusCode
{
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
  std::variant<std::vector<std::byte>, std::string> payload;
};

struct HttpRequestResult {
  HttpRequest request;
  HttpStatusCode statusCode;
  std::variant<std::vector<std::byte>, std::string> payload;
};

} // namespace katla

#endif
