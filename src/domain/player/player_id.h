// domain\player\player_id.h
#pragma once
#include <string>

class PlayerId {
public:
  explicit PlayerId(std::string value) : value_(std::move(value)) {}
  const std::string &ToString() const { return value_; }
  // Операторы сравнения - генерируются компилятором
  bool operator==(const PlayerId &) const = default;
  bool operator!=(const PlayerId &) const = default;

private:
  std::string value_;
};
