// application\ports\sipmle_id_generator.h
#include "simple_id_generator.h"
#include "domain/session/session_id.h"

SimpleIdGenerator::SimpleIdGenerator(std::string prefix) : prefix_(prefix) {}

SessionId SimpleIdGenerator::NewSessionId() {
  return SessionId(prefix_ + "-" + std::to_string(next_session_id_++));
}
