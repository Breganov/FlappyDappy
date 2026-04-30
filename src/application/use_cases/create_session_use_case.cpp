// application\use_cases\create_session_use_case.h
#include "create_session_use_case.h"

#include "domain/game/physics_config.h"
#include "domain/session/session_id.h"

CreateSessionUseCase::CreateSessionUseCase(IIdGenerator &ids,
                                           SessionService &sessions)
    : ids_(ids), sessions_(sessions) {}

SessionId CreateSessionUseCase::Execute() {
  SessionId id = ids_.NewSessionId();
  PhysicsConfig phys;
  CollisionService collision;
  sessions_.CreateSession(id, 42u, phys.gravity, phys.jump_velocity,
                          phys.scroll_speed);
  return id;
}
