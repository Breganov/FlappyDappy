// infrastructure/net/websocket_message_parser.cpp
#include "websocket_message_parser.h"
#include <boost/json.hpp>
#include <boost/json/parse.hpp>
#include <string_view>

ParsedMessage WebSocketMessageParser::Parse(std::string_view raw) const {
  ParsedMessage result{};

  // try {
  //   json::value value = json::parse(raw);
  // }
  return result;
}
