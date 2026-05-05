// application/use_cases/create_session_use_case.cpp
#include "create_session_use_case.h"
#include "application/use_cases/game_config.h"
#include "domain/session/session_id.h"

CreateSessionUseCase::CreateSessionUseCase(IIdGenerator &ids,
                                           SessionService &sessions,
                                           GameConfig &config)
    : ids_(ids), sessions_(sessions), config_(config) {}

SessionId CreateSessionUseCase::Execute() {
  SessionId id = ids_.NewSessionId();
  const auto &phys = config_.GetPhysicsConfig();
  sessions_.CreateSession(id, config_.GetSeedNumber(), phys.gravity,
                          phys.jump_velocity, phys.scroll_speed);
  return id;
}
