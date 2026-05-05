// infrastructure/net/websocket_message_parser.h
#pragma once
#include "parsed_message.h"

class WebSocketMessageParser {
public:
  WebSocketMessageParser() = default;

  ParsedMessage Parse(std::string_view raw) const;
  // helper-методы
  MessageType ParsedMessageType();
  bool ReadRequiredStringField();
  ParsedMessage MakeError();
};
