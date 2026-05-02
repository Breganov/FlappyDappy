// application\ports\sipmle_id_generator.h
#pragma once

#include "application/ports/id_generator.h"
#include "domain/session/session_id.h"
#include <cstdint>

class SimpleIdGenerator : public IIdGenerator {
public:
  explicit SimpleIdGenerator(std::string prefix = "session");
  SessionId NewSessionId() override;

private:
  std::string prefix_;
  std::uint64_t next_session_id_ = 1;
};
