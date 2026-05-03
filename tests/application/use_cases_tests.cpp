// tests/application/use_cases_tests.cpp
#include "application/ports/fake_session_broadcaster.h"
#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/session_service.h"
#include "application/use_cases/start_match_use_case.h"
#include "application/use_cases/submit_input_use_case.h"
#include "application/use_cases/tick_session_use_case.h"
#include "domain/game/bird_state.h"
#include "domain/player/player_id.h"
#include "domain/session/input_command.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("CreateSessionUseCase creates session in SessionService") {
  SessionService sessions;
  SimpleIdGenerator ids;

  CreateSessionUseCase create(ids, sessions);
  SessionId id = create.Execute();

  auto session = sessions.FindSession(id);

  REQUIRE(session.has_value());
  REQUIRE(session->get().GetId().ToString() == id.ToString());
}

TEST_CASE("JoinSessionUseCase adds player and notifies broadcaster") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;
  SimpleIdGenerator ids;

  CreateSessionUseCase create(ids, sessions);
  SessionId id = create.Execute();

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);
  join_session_use_case.Execute(id, PlayerId("player-1"));

  auto session = sessions.FindSession(id);
  REQUIRE(session.has_value());

  REQUIRE(broadcaster.player_joined_called);
  REQUIRE(broadcaster.last_joined_session_id == id.ToString());
  REQUIRE(broadcaster.last_joined_player_id == "player-1");

  REQUIRE(session->get().GetPlayers().size() == 1);
  REQUIRE(session->get().GetPlayers()[0].GetPlayerId().ToString() ==
          "player-1");
}

TEST_CASE("StartMatchUseCase transitions session to running state") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;
  SimpleIdGenerator ids;

  CreateSessionUseCase create(ids, sessions);
  SessionId id = create.Execute();
  PlayerId pid =
      PlayerId("player-1"); // надо написать такую же систему для формирования
                            // как и для SessionId только для PlayerId

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);
  join_session_use_case.Execute(id, pid);
  StartMatchUseCase start_match(sessions);
  start_match.Execute(id);

  auto session = sessions.FindSession(id);
  REQUIRE(session.has_value());
  REQUIRE(session->get().GetState() == SessionState::InProgress);
}

TEST_CASE("SubmitInputUseCase applies jump after tick") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;
  SimpleIdGenerator ids;

  CreateSessionUseCase create(ids, sessions);
  SessionId id = create.Execute();
  PlayerId pid =
      PlayerId("player-1"); // надо написать такую же систему для формирования
                            // как и для SessionId только для PlayerId

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);
  join_session_use_case.Execute(id, pid);

  StartMatchUseCase start_match(sessions);
  start_match.Execute(id);

  TickSessionUseCase tick(sessions, broadcaster);
  SubmitInputUseCase input(sessions);

  auto session = sessions.FindSession(id);
  REQUIRE(session.has_value());

  const auto &initial_bird = session->get().GetPlayers()[0].GetBird();
  const auto y = initial_bird.y;
  const auto velocity_y = initial_bird.velocity_y;

  input.Execute(id, InputCommand{pid, InputType::Jump});
  tick.Execute(id, std::chrono::milliseconds(16));

  const auto &updated_bird = session->get().GetPlayers()[0].GetBird();
  REQUIRE(y > updated_bird.y);
  REQUIRE(velocity_y > updated_bird.velocity_y);
}

TEST_CASE("TickSessionUseCase broadcasts snapshot") {
  FakeSessionBroadcaster broadcaster;
  SessionService sessions;
  SimpleIdGenerator ids;

  CreateSessionUseCase create(ids, sessions);
  SessionId id = create.Execute();
  PlayerId pid =
      PlayerId("player-1"); // надо написать такую же систему для формирования
                            // как и для SessionId только для PlayerId

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);
  join_session_use_case.Execute(id, pid);

  StartMatchUseCase start_match(sessions);
  start_match.Execute(id);

  TickSessionUseCase tick(sessions, broadcaster);
  tick.Execute(id, std::chrono::milliseconds(16));
}

TEST_CASE(
    "TickSessionUseCase broadcasts match finished when session is finished") {}

TEST_CASE("FinishMatchUseCase removes finished session") {}

TEST_CASE("GameLoopService ticks all sessions and cleans finished ones") {}
