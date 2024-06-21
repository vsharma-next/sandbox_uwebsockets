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

public:
    WebSocketController(Counter& counter): counter(counter) {}
    struct PerSocketData {};// Still an empty struct

    void run() {

        uWS::App* app = new uWS::App({/* There are example certificates in uWebSockets.js repo */
                                      .key_file_name = "misc/key.pem",
                                      .cert_file_name = "misc/cert.pem",
                                      .passphrase = "1234"});
        /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */

        app->ws<PerSocketData>("/*", {/* Settings */
                                      .open = [this](auto* ws) {
                                            std::cout << "Connection Opened \n";
                                            ws->subscribe("counter");
                                            // Send the current value to just the newly connected client
                                            int currentValue = counter.getValue();
                                            ws->send(std::to_string(currentValue), uWS::OpCode::TEXT);
                                          /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */ },
                                      .message = [this, &app](auto* ws, std::string_view message, uWS::OpCode opCode) {
                                                            simdjson::dom::parser parser;
                // simdjson::dom::element msg = parser.parse(simdjson::padded_string(message.data(), message.size()));
                
                simdjson::dom::element msg = parser.parse(message);

                std::string_view action = msg["action"].get_string().value();

                if (action == "increment") {
                    counter.increment();
                } else if (action == "decrement") {
                    counter.decrement();
                } 

                int currentValue = counter.getValue();
                std::string response = std::to_string(currentValue);
                app->publish("counter",response, uWS::OpCode::TEXT); },
                                      .close = [](auto* ws, int code, std::string_view message) {
                                            std::cout << "Websocket closed, code:" << code << ", message: " << message << "\n";
            /* You may access ws->getUserData() here */ }})
                .listen(9001, [](auto* listen_socket) {
                    if (listen_socket) {
                        std::cout << "Listening on port " << 9001 << std::endl;
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