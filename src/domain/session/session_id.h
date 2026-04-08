#pragma once
#include <string>

class SessionId {
public:
  explicit SessionId(std::string value) : value_(std::move(value)) {}
  const std::string& ToString() const { return value_; }
  bool operator==(const SessionId& other) const = default;

private:
  std::string value_;
};
