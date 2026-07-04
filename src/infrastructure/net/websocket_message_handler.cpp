// infrastructure/net/websocket_message_handler.cpp
#include "websocket_message_handler.h"

#include "infrastructure/net/router_result.h"

#include <boost/json.hpp>
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>

namespace json = boost::json;

WebSocketMessageHandler::WebSocketMessageHandler(WebSocketMessageParser &parser,
                                                 WebSocketRouter &router,
                                                 ConnectionRegistry &registry)
    : parser_(parser), router_(router), registry_(registry) {}

std::string WebSocketMessageHandler::ProcessIncomingMessage(
    std::string_view json_text,
    std::shared_ptr<WebSocketSession> sender) const {
  auto parsed_message = parser_.Parse(json_text);
  auto router_message = router_.Route(parsed_message);

  json::object response;

  if (!router_message.is_handled) {
    response["type"] = "error";
    response["message"] = router_message.error;
    return json::serialize(response);
  }

  if (router_message.is_handled && parsed_message.is_valid &&
      parsed_message.type == MessageType::Join) {
    registry_.Register(parsed_message.session_id, sender);
  }

  response["type"] = "ack";
  response["message"] = "handled";
  return json::serialize(response);
}
