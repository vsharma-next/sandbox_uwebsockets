#include "simdjson/simdjson.h"
#include "uWebSockets/src/App.h"

#include <iostream>
#include <string>

class Counter {
public:
    int value = 0;// Initial counter value

    void increment() { value++; }
    void decrement() { value--; }
    int getValue() const { return value; }
};


class WebSocketController {
private:
    Counter& counter;
    struct PerSocketData {};// Still an empty struct
    uWS::App app;

    void onOpen(uWS::WebSocket<false, true, PerSocketData>* ws) {
        ws->subscribe("counter");
        int currentValue = counter.getValue();
        ws->send(std::to_string(currentValue), uWS::OpCode::TEXT);
    }

    void onMessage(uWS::WebSocket<false, true, PerSocketData>* ws,
                   std::string_view message, uWS::OpCode opCode) {

        simdjson::dom::parser parser;
        simdjson::dom::element msg = parser.parse(message);
        std::string_view action = msg["action"].get_string().value();

        if (action == "increment") {
            counter.increment();
        } else if (action == "decrement") {
            counter.decrement();
        }

        int currentValue = counter.getValue();
        std::string response = std::to_string(currentValue);
        app.publish("counter", response, uWS::OpCode::TEXT);
    }

    void onClose(uWS::WebSocket<false, true, PerSocketData>* ws, int code, std::string_view message) {
        std::cout << "Websocket closed, code:" << code << ", message: " << message << "\n";
    }

public:
    WebSocketController(Counter& counter): counter(counter),
                                           app(uWS::App({.key_file_name = "misc/key.pem",
                                                         .cert_file_name = "misc/cert.pem",
                                                         .passphrase = "1234"})) {}

    void run() {
        app.ws<PerSocketData>("/*", {.open = [this](auto* ws) { this->onOpen(ws); },
                                     .message = [this](auto* ws, std::string_view message, uWS::OpCode opCode) { this->onMessage(ws, message, opCode); },
                                     .close = [this](auto* ws, int code, std::string_view message) { this->onClose(ws, code, message); }})
                .listen(9001, [](auto* token) {
                    if (token) {
                        std::cout << "Server listening on port 9001\n";
                    } else {
                        std::cerr << "Failed to listen on port 9001\n";
                    }
                })
                .run();
    }
};

int main() {
    Counter counter;
    WebSocketController controller(counter);

    controller.run();
    return 0;
}