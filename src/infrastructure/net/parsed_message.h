#pragma once
#include "message_type.h"
#include <string>

struct ParsedMessage {
  MessageType type = MessageType::Unknown;
  std::string session_id;
  std::string player_id;
  bool is_valid = false;
  std::string error;
};
