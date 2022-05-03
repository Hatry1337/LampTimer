#include "WebSocket.h"

WebSocket::WebSocket(WebSocketMessageHandler handler, WebSocketErrorHandler errorHandler) {
    this->handler = handler;
    this->errorHandler = errorHandler;

    this->eventHandler = [this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            client->ping();
        } else if (type == WS_EVT_ERROR) {
            if(this->errorHandler) {
                this->errorHandler(server, client, type, arg, data, len);
            }
        } else if (type == WS_EVT_DATA) {
            AwsFrameInfo *info = (AwsFrameInfo *)arg;
            std::string msg = "";
            if (info->final && info->index == 0 && info->len == len) {
                if (info->opcode == WS_TEXT) {
                    for (size_t i = 0; i < info->len; i++) {
                        msg += (char)data[i];
                    }
                } else {
                    char buff[3];
                    for (size_t i = 0; i < info->len; i++) {
                        sprintf(buff, "%02x ", (uint8_t)data[i]);
                        msg += buff;
                    }
                }
            } else {
                if (info->opcode == WS_TEXT) {
                    for (size_t i = 0; i < len; i++) {
                        msg += (char)data[i];
                    }
                } else {
                    char buff[3];
                    for (size_t i = 0; i < len; i++) {
                        sprintf(buff, "%02x ", (uint8_t)data[i]);
                        msg += buff;
                    }
                }
            }
            if(msg.empty()) return;

            if(this->handler) {
                this->handler(server, client, msg);
            }
            return;
        }
    };
}