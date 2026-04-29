// C:\Code\C++\FlappyDappy\tests\domain
// #include "catch2/catch_session.hpp"
#include "domain/game/physics_engine.h"
#include "domain/session/game_session.h"
#include "domain/session/input_command.h"
#include "domain/session/session_id.h"
#include "domain/session/session_state.h"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <chrono>
#include <cstdint>
// #include <ratio>

TEST_CASE("GameSession starts match only after countdown") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

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
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

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
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

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

TEST_CASE("Tick does nothing before StartMatch") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

  session.AddPlayer(PlayerId("player1"));

  auto y_before = session.BuildSnapshot().players[0].y;

  for (int i = 0; i < 500 && !session.IsFinished(); ++i) {
    session.Tick(std::chrono::milliseconds(16));
  }

  auto y_after = session.BuildSnapshot().players[0].y;

  REQUIRE(y_before == y_after);
}

TEST_CASE("Jump affects only the targeted player") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

  session.AddPlayer(PlayerId("player1"));
  session.AddPlayer(PlayerId("player2"));

  session.StartCountdown();
  session.StartMatch();

  InputCommand p1_command = {PlayerId("player1"), InputType::Jump, 0u};
  session.EnqueueInput(p1_command);

  auto snap_before = session.BuildSnapshot();

  for (int i = 0; i < 20 && !session.IsFinished();
       ++i) { // а больше заставит птицу падать
    session.Tick(std::chrono::milliseconds(16));
  }

  auto snap_after = session.BuildSnapshot();

  REQUIRE(snap_before.players[0].y >= snap_after.players[0].y); // поднялся
  REQUIRE(snap_before.players[1].y <= snap_after.players[1].y); // упал
}

TEST_CASE("Distance increases while player is alive") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

  session.AddPlayer(PlayerId("player1"));

  session.StartCountdown();
  session.StartMatch();

  // InputCommand p1_command = {PlayerId("player1"), InputType::Jump, 0u};
  // session.EnqueueInput(p1_command);

  auto snap_before = session.BuildSnapshot();

  for (int i = 0; i < 30 && !session.IsFinished();
       ++i) { // а больше заставит птицу падать
    session.Tick(std::chrono::milliseconds(16));
  }

  auto snap_after = session.BuildSnapshot();

  REQUIRE(snap_after.players[0].alive);
  REQUIRE(snap_after.players[0].x >= snap_before.players[0].x);
  const double expected_x =
      snap_before.players[0].x + snap_after.players[0].distance;
  REQUIRE(snap_after.players[0].x == Catch::Approx(expected_x).margin(0.01));
}

TEST_CASE("Player dies on collision") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 3000.0f; // чтобы точно ударился

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

  session.AddPlayer(PlayerId("player1"));

  session.StartCountdown();
  session.StartMatch();

  auto snap_before = session.BuildSnapshot();
  REQUIRE(snap_before.players[0].alive);

  for (int i = 0; i < 500 && !session.IsFinished(); ++i) {
    session.Tick(std::chrono::milliseconds(16));
  }

  auto snap_after = session.BuildSnapshot();

  REQUIRE(!snap_after.players[0].alive);
}

TEST_CASE("BuildResult sorts players by distance") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

  session.AddPlayer(PlayerId("player1"));
  session.AddPlayer(PlayerId("player2"));

  InputCommand p2_command = {PlayerId("player2"), InputType::Jump, 0u};
  // 4 раза резко прыгаем вторым игроком, чтобы быстрее умереть
  session.EnqueueInput(p2_command);
  session.EnqueueInput(p2_command);
  session.EnqueueInput(p2_command);
  session.EnqueueInput(p2_command);

  session.StartCountdown();
  session.StartMatch();

  for (int i = 0; i < 500 && !session.IsFinished(); ++i) {
    session.Tick(std::chrono::milliseconds(16));
  }

  auto result = session.BuildResult();
  auto snap = session.BuildSnapshot();

  REQUIRE(!snap.players[1].alive);
  REQUIRE(result.rankings[0].distance >= result.rankings[1].distance);
}

TEST_CASE("BuildSnapshot conatains x position") {
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  const double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 120.0f;

  GameSession session(sid, seed, gravity, jump_velocity, scroll_speed);

  session.AddPlayer(PlayerId("player1"));
  session.AddPlayer(PlayerId("player2"));

  InputCommand p2_command = {PlayerId("player2"), InputType::Jump, 0u};
  session.EnqueueInput(p2_command);

  session.StartCountdown();
  session.StartMatch();

  for (int i = 0; i < 500 && !session.IsFinished(); ++i) {
    session.Tick(std::chrono::milliseconds(16));
  }

  auto snap = session.BuildSnapshot();
  REQUIRE(snap.players[0].x);
  REQUIRE(snap.players[1].x);
  REQUIRE(snap.players[0].x > 0);
  REQUIRE(snap.players[1].x > 0);
}
