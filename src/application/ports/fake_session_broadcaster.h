// application\ports\fake_session_broadcaster.h
#pragma once

#include "domain/game/world_snapshot.h"
#include "domain/match/match_result.h"
#include "domain/session/session_id.h"
#include "session_broadcaster.h"

class FakeSessionBroadcaster : public ISessionBroadcaster {
public:
  void BroadcastSnapshot(const SessionId &session_id,
                         const WorldSnapshot &snapshot) override {
    snapshot_called = true;
    last_snapshot_session_id = session_id.ToString();
    last_snapshot_tick = snapshot.tick;
  }

  void BroadcastMatchFinished(const SessionId &session_id,
                              const MatchResult &result) override {
    match_finished_called = true;
    last_finished_session_id = session_id.ToString();
    last_rankings_count = result.rankings.size();
  }

  void NotifyPlayerJoined(const SessionId &session_id,
                          const PlayerId &player_id) override {
    player_joined_called = true;
    last_joined_player_id = session_id.ToString();
    last_joined_player_id = player_id.ToString();
  }

public:
  bool snapshot_called = false;
  bool match_finished_called = false;
  bool player_joined_called = false;

  std::string last_snapshot_session_id;
  std::string last_finished_session_id;
  std::string last_joined_session_id;
  std::string last_joined_player_id;

  std::uint64_t last_snapshot_tick = 0;
  std::size_t last_rankings_count = 0;
};
