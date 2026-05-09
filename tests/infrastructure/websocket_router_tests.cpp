// tests/infrastructure/websocket_router_tests.cpp
#include "application/ports/fake_session_broadcaster.h"
#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/game_config.h"
#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/session_service.h"
#include "application/use_cases/start_match_use_case.h"
#include "application/use_cases/tick_session_use_case.h"
#include "domain/game/physics_config.h"
#include "domain/player/player_id.h"
#include "infrastructure/net/websocket_router.h"

#include "catch2/catch_test_macros.hpp"
#include <chrono>
#include <sys/stat.h>

TEST_CASE("WebSocketRouter returns error for invalid message.") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);
  SubmitInputUseCase input(sessions);

  WebSocketRouter router(join_session_use_case, input);
  auto result = router.Route(
      ParsedMessage{MessageType::Unknown, "lol", "player-1", false, "Unknown"});

  REQUIRE_FALSE(result.is_handled);
  REQUIRE(result.error == "Unknown");
}

TEST_CASE("WebSocketRouter routes join message to JoinSessionUseCase") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;
  SimpleIdGenerator ids;
  GameConfig game_config{PhysicsConfig{}};

  CreateSessionUseCase create(ids, sessions, game_config);
  SessionId id = create.Execute();

  JoinSessionUseCase join(sessions, broadcaster);
  SubmitInputUseCase submit(sessions);

  WebSocketRouter router(join, submit);

  auto session = sessions.FindSession(id);
  REQUIRE(session->get().GetPlayers().size() == 0);

  auto result = router.Route(
      ParsedMessage(MessageType::Join, id.ToString(), "player-1", true, ""));

  REQUIRE(result.is_handled);
  REQUIRE(result.error.empty());
  REQUIRE(session->get().GetPlayers().size() == 1);

  REQUIRE(session.has_value());
  REQUIRE(session->get().GetPlayers()[0].GetPlayerId().ToString() ==
          "player-1");
  REQUIRE(broadcaster.player_joined_called);
}

TEST_CASE("WebSocketRouter routes jump message to SubmitInputUseCase") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;
  SimpleIdGenerator ids;
  GameConfig game_config{PhysicsConfig{}};

  CreateSessionUseCase create(ids, sessions, game_config);
  SessionId id = create.Execute();

  JoinSessionUseCase join(sessions, broadcaster);
  SubmitInputUseCase submit(sessions);

  WebSocketRouter router(join, submit);

  auto session = sessions.FindSession(id);
  REQUIRE(session.has_value());

  auto result = router.Route(
      ParsedMessage(MessageType::Join, id.ToString(), "player-1", true, ""));
  REQUIRE(result.is_handled);
  REQUIRE(result.error.empty());
  REQUIRE(session->get().GetPlayers().size() == 1);
  REQUIRE(session->get().GetPlayers()[0].GetPlayerId().ToString() ==
          "player-1");

  auto &bird = session->get().GetPlayers()[0].GetBird();

  StartMatchUseCase start(sessions);
  start.Execute(id);

  REQUIRE(session->get().GetState() == SessionState::InProgress);

  const auto start_y_velocity = bird.velocity_y;
  const auto start_y = bird.velocity_y;
  result = router.Route(
      ParsedMessage(MessageType::Jump, id.ToString(), "player-1", true, ""));
  REQUIRE(result.is_handled);
  REQUIRE(result.error.empty());

  TickSessionUseCase tick(sessions, broadcaster);
  tick.Execute(id, std::chrono::milliseconds(16));

  const auto end_y_velocity = bird.velocity_y;
  const auto end_y = bird.velocity_y;
  REQUIRE(end_y != start_y);
  REQUIRE(end_y_velocity < start_y_velocity);
}

TEST_CASE("WebSocketRouter returns error for unknown message type") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);
  SubmitInputUseCase input(sessions);

  WebSocketRouter router(join_session_use_case, input);
  auto result = router.Route(
      ParsedMessage{MessageType::Unknown, "lol", "aasdf", true, "Unknown"});

  REQUIRE(result.is_handled);
  REQUIRE(result.error == "");
}
