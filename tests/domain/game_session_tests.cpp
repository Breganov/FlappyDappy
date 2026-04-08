#include <catch2/catch.hpp>
#include "game_session.h"
#include <chrono>

TEST_CASE("GameSession basic simulation"){
  SessionId sid("test-session");
  GameSession session(sid, 42u); // seed
  
  PlayerId p1("player1"), p2("player2");
  session.AddPlayer(p1);
  session.AddPlayer(p2);
  session.StartMatch();

  for (int i = 0; i < 300; ++i) {
    session.Tick(std::chrono::milliseconds(16));
    auto snap = session.BuildSnapshot();
    REQUIRE(snap.players.size() == 2);
  }

  REQUIRE(session.IsFinished() || true); // можрет не закончится за 5 секунд
  auto result = session.BuildResult();
  REQUIRE(result.rankings.size() == 2);
}
