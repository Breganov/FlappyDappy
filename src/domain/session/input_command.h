enum class InputType{
  Jump
};

struct InputCommand {
  PlayerId player_id;
  InputType type;
  std::uint64_t client_sequence = 0;
}
