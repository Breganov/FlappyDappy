// C:\Code\C++\FlappyDappy\tests\domain
// #include "catch2/catch_session.hpp"
#include "domain/session/game_session.h"
#include "domain/session/session_id.h"
#include "domain/session/session_state.h"
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
// #include <ratio>

TEST_CASE("GameSession starts match only after countdown") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;

  GameSession session(sid, seed, gravity, jump_velocity);

  session.StartMatch();
  REQUIRE(session.GetState() == SessionState::WaitingForPlayers);

  session.StartCountdown();
  REQUIRE(session.GetState() == SessionState::Countdown);

  session.StartMatch();
  REQUIRE(session.GetState() == SessionState::InProgress);
}

TEST_CASE("GameSession tick updates snapshot when match is in progress") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;

  GameSession session(sid, seed, gravity, jump_velocity);

  session.AddPlayer(PlayerId("player1"));
  session.AddPlayer(PlayerId("player2"));

  session.StartCountdown();
  session.StartMatch();

  auto before = session.BuildSnapshot();
  session.Tick(std::chrono::milliseconds(16));
  auto after = session.BuildSnapshot();

  REQUIRE(before.players.size() == 2);
  REQUIRE(after.players.size() == 2);
  REQUIRE(after.state == SessionState::InProgress);
  REQUIRE(after.tick == before.tick + 1);

  REQUIRE(after.players[0].y != before.players[0].y);
}

TEST_CASE("GameSession eventually finishes") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;

  GameSession session(sid, seed, gravity, jump_velocity);

  session.AddPlayer(PlayerId("player1"));
  session.AddPlayer(PlayerId("player2"));

  session.StartCountdown();
  session.StartMatch();

  for (int i = 0; i < 500 && !session.IsFinished(); ++i) {
    session.Tick(std::chrono::milliseconds(16));
  }

  REQUIRE(session.IsFinished());

  auto result = session.BuildResult();
  REQUIRE(result.rankings.size() == 2);
}
