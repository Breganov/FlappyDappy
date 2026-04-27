#pragma once

#include "domain\session\session_id.h"
#include <chrono>

class TickSessionUseCase {
public:
  void Execute(const SessionId &session_id, std::chrono::milliseconds delta);
};
