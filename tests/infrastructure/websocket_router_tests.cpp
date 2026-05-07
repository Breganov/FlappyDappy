// tests/infrastructure/websocket_router_tests.cpp
#include "application/ports/fake_session_broadcaster.h"
#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/game_config.h"
#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/start_match_use_case.h"
#include "application/use_cases/tick_session_use_case.h"
#include "catch2/catch_test_macros.hpp"
#include "infrastructure/net/websocket_router.h"

TEST_CASE("WebSocketRouter returns error for invalid message.") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;
  SimpleIdGenerator ids;
  GameConfig game_config{PhysicsConfig{}};

  CreateSessionUseCase create(ids, sessions, game_config);
  SessionId id = create.Execute();
  PlayerId pid = PlayerId("player-1");

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);
  join_session_use_case.Execute(id, pid);

  StartMatchUseCase start_match(sessions);
  start_match.Execute(id);

  TickSessionUseCase tick(sessions, broadcaster);
  SubmitInputUseCase input(sessions);

  WebSocketRouter websocket_router_tests(join_session_use_case, input);
  websocket_router_tests.Route(
      ParsedMessage{MessageType::Unknown, "lol", "player-1", true, "Unknown"});
}
