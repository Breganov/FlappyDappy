#pragma once
#include <string>

class PlayerId {
public:
  explicit PlayerId(std::string value);
  const std::string& ToString() const;
  bool operator==(const PlayerId&) const = default;

private:
  std::string value_;
};
