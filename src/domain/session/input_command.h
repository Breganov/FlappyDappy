// src\domain\session\input_command.h
#pragma once
#include "../../domain/player/player_id.h"
enum class InputType{
  Jump
};

struct InputCommand {
  PlayerId player_id;
  InputType type;
  std::uint64_t client_sequence = 0;
}
