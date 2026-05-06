// infrastructure/net/websocket_message_parser.cpp
#include "websocket_message_parser.h"
#include "infrastructure/net/message_type.h"
#include <boost/json.hpp>
#include <boost/json/object.hpp>
#include <exception>
#include <string_view>

ParsedMessage WebSocketMessageParser::Parse(std::string_view raw) const {
  ParsedMessage result{};

  try {
    json::value value = json::parse(raw);
    if (!value.is_object()) {
      result.error = "JSON root must be an object.";
    }
    const json::object &obj = value.as_object();

    std::string type_str;
    std::string error;

    if (!ReadRequiredStringField(obj, "type", type_str, error)) {
      return MakeError(error);
    }

    ParsedMessage result{};
    result.type = ParseMessageType(type_str);

    if (result.type == MessageType::Unknown) {
      return MakeError("Unknown message type: " + type_str);
    }

    if (!ReadRequiredStringField(obj, "session_id", type_str, error)) {
      return MakeError(error);
    }

    if (!ReadRequiredStringField(obj, "player_id", type_str, error)) {
      return MakeError(error);
    }

    result.is_valid = true;
    return result;
  } catch (const std::exception &e) {
    return MakeError("Invalid JSON: " + std::string(e.what()));
  }
}

MessageType
WebSocketMessageParser::ParseMessageType(std::string_view type_str) const {
  if (type_str == "join") {
    return MessageType::Join;
  }

  if (type_str == "jump") {
    return MessageType::Jump;
  }

  return MessageType::Unknown;
}

bool WebSocketMessageParser::ReadRequiredStringField(
    const json::object &obj, std::string_view field_name,
    std::string &out_value, std::string error) const {
  auto it = obj.find(field_name);
  if (it == obj.end()) {
    error = "Missing required field: " + std::string(field_name);
    return false;
  }

  if (!it->value().is_string()) {
    error = "Field must be a string: " + std::string(field_name);
    return false;
  }

  out_value = std::string(it->value().as_string());
  return true;
}

ParsedMessage WebSocketMessageParser::MakeError(std::string message) const {
  ParsedMessage result{};
  result.is_valid = false;
  result.error = std::move(message);
  return result;
}
