// src\domain\player\nickname.h
#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

class Nickname {
public:
  Nickname() = default;

  explicit Nickname(std::string value) : value_(std::move(value)) {
    Validate_(value_);
  }

  const std::string &ToString() const noexcept { return value_; }

  bool Empty() const noexcept { return value_.empty(); }

  bool operator==(const Nickname &) const = default;
  bool operator!=(const Nickname &) const = default;

private:
  static void Validate_(std::string_view value) {
    if (value.empty()) {
      throw std::invalid_argument("Nickname must not be empty");
    }

    if (value.size() > 32) {
      throw std::invalid_argument("Nickname must be at most 32 characters");
    }
  }

private:
  std::string value_;
};
