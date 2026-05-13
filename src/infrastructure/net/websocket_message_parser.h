// infrastructure/net/websocket_message_parser.h
#pragma once
#include "parsed_message.h"
#include <boost/json.hpp>

namespace json = boost::json;

class WebSocketMessageParser {
public:
  WebSocketMessageParser() = default;

  ParsedMessage Parse(std::string_view raw) const;

private:
  // helper-методы
  MessageType ParseMessageType(std::string_view type_str) const;
  bool ReadRequiredStringField(const json::object &obj,
                               std::string_view field_name,
                               std::string &out_value,
                               std::string &error) const;
  ParsedMessage MakeError(std::string message) const;
};
