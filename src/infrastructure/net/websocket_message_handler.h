// infrastructure/net/websocket_message_handler.h
#pragma once

#include "infrastructure/net/websocket_message_parser.h"
#include "infrastructure/net/websocket_router.h"

#include <string>
#include <string_view>

class WebSocketMessageHandler {
public:
  WebSocketMessageHandler(WebSocketMessageParser &parser,
                          WebSocketRouter &router);

  std::string ProcessIncomingMessage(std::string_view json_text) const;

private:
  WebSocketMessageParser &parser_;
  WebSocketRouter &router_;
};
