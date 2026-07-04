// infrastructure/net/websocket_message_handler.h
#pragma once

#include "infrastructure/net/connection_registry.h"
#include "infrastructure/net/websocket_message_parser.h"
#include "infrastructure/net/websocket_router.h"

#include <memory>
#include <string>
#include <string_view>

class WebSocketMessageHandler {
public:
  WebSocketMessageHandler(WebSocketMessageParser &parser,
                          WebSocketRouter &router,
                          ConnectionRegistry &registry);

  std::string
  ProcessIncomingMessage(std::string_view json_text,
                         std::shared_ptr<WebSocketSession> sender) const;

private:
  WebSocketMessageParser &parser_;
  WebSocketRouter &router_;
  ConnectionRegistry &registry_;
};
