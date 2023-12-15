#include "cxxopts.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
#include "katla/core-uv/uv-core-application.h"
#include "katla/core/core.h"
#include "katla/core/posix-socket.h"
#include "katla/websockets/websocket-server-client-lws.h"
#include "katla/websockets/websocket-server-lws.h"

#include <arpa/inet.h>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <functional>
#include <net/if.h>

void handleCommands(std::string command)
{

    if (command == "http-serve") {

        auto& uvCoreApp = katla::UvCoreApplication::instance();
        auto appInitResult = uvCoreApp.init();
        if (!appInitResult) {
            katla::print(stderr, "{}\n    {}-{}-{}\n", appInitResult.error().message());
        }

        katla::WebSocketServerLws server;
        server.init(8080);

        server.handle(katla::HttpMethod::Get, "echo", [](katla::WebSocketServerClient& client, const katla::HttpRequest& request) { katla::printInfo("http handler!!"); });

        server.handleWebSocket("echo", [](katla::WebSocketServerClient& client) {
            katla::printInfo("websocket handler!!");

            client.registerMessageHandler([](const katla::LwsPacket& message) {
                std::string body(reinterpret_cast<char*>(message.payload->data()), message.payload->size());
                katla::printInfo("My message: {}", body);
            });
        });

        auto runResult = uvCoreApp.run();
        if (!runResult) {
            katla::print(stderr,
                         "{}\n    {}-{}\n",
                         runResult.error().message(),
                         runResult.error().description(),
                         runResult.error().info());
        }

        server.stop();

        fflush(stdout);
        return;
    }

    fmt::print(stderr, "unrecognized command: {}!", command);
}

int main(int argc, char* argv[])
{
    cxxopts::Options options("katla-websocket-integration-tests", "to test the test websockets");

    options.positional_help("[command]").show_positional_help();

    std::string command = "";
    try {
        options.add_options()("help", "Print help")("command", "Command to run", cxxopts::value<std::string>());

        options.parse_positional({ "command" });

        auto parseResult = options.parse(argc, argv);

        if (parseResult.count("help")) {
            std::cout << options.help({ "", "Group" }) << std::endl;
            exit(0);
        }

        if (parseResult.count("command")) {
            command = parseResult["command"].as<std::string>();
        }
    } catch (cxxopts::OptionException ex) {
        katla::printError("{}\n", ex.what());
    } catch (std::exception ex) {
        katla::printError("{}\n", ex.what());
    }

    handleCommands(command);

    exit(EXIT_SUCCESS);
}
