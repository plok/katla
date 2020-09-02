
#ifndef KATLA_INCOMING_HTTP_REQUEST_H
#define KATLA_INCOMING_HTTP_REQUEST_H

#include "katla/core/core.h"

#include "http-request.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace katla {

class IncomingHttpRequest {
  public:
    IncomingHttpRequest(const HttpRequest& request);
    ~IncomingHttpRequest() = default;

    void insert(gsl::span<std::byte> bytes);
    std::unique_ptr<HttpRequest> completedRequest();

  private:
    IncomingHttpRequest(const IncomingHttpRequest&) = delete;

    std::unique_ptr<HttpRequest> m_request;
    std::unique_ptr<std::vector<std::byte>> m_buffer;
};

} // namespace katla

#endif
