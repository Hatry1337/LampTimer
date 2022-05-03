#include <Arduino.h>
#include "ESPAsyncWebServer.h"

typedef std::function<void(AsyncWebSocket * server, AsyncWebSocketClient * client, std::string message)> WebSocketMessageHandler;
typedef AwsEventHandler WebSocketErrorHandler;

class WebSocket {
    private:
        WebSocketMessageHandler handler;
        WebSocketErrorHandler errorHandler;
    public:
        WebSocket(WebSocketMessageHandler handler, WebSocketErrorHandler errorHandler);
        AwsEventHandler eventHandler;
};
