// infrastructure/net/websocket_message_handler.cpp
#include "websocket_message_handler.h"

#include "infrastructure/net/router_result.h"

#include <boost/json.hpp>
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>

namespace json = boost::json;

WebSocketMessageHandler::WebSocketMessageHandler(WebSocketMessageParser &parser,
                                                 WebSocketRouter &router)
    : parser_(parser), router_(router) {}

std::string WebSocketMessageHandler::ProcessIncomingMessage(
    std::string_view json_text) const {
  auto parsed_message = parser_.Parse(json_text);
  auto router_message = router_.Route(parsed_message);

  json::object response;

  if (!router_message.is_handled) {
    response["type"] = "error";
    response["message"] = router_message.error;
    return json::serialize(response);
  }
  response["type"] = "ack";
  response["message"] = "handled";
  return json::serialize(response);
}
