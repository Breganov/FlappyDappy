#pragma once
#include "application\use_cases\session_service.h"

class IIdGenerator {
public:
  virtual ~IIdGenerator() = default;
  virtual SessionId NewSessionId() = 0;
};
