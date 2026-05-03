// domain\session\games_session.cpp
#include "game_session.h"
#include "domain/game/bird_state.h"
#include "domain/game/collision_service.h"
#include "domain/match/match_result.h"

// =============================================================================

GameSession::GameSession(SessionId id, std::uint32_t seed, double gravity,
                         double jump_velocity, double scroll_speed)
    : id_(std::move(id)), seed_(seed), obstacle_generator_(seed),
      pipes_(obstacle_generator_.InitialPipes()), current_tick_(0),
      physics_config_(), physics_engine_(gravity, jump_velocity, scroll_speed),
      collision_() {}

// =============================================================================

const SessionId &GameSession::GetId() const { return id_; }

// =============================================================================

SessionState GameSession::GetState() const { return state_; }

// =============================================================================

void GameSession::AddPlayer(PlayerId player_id) {
  players_.emplace_back(std::move(player_id), BirdState());
}

void GameSession::MarkPlayerReady(const PlayerId &player_id) {
  for (auto &p : players_) {
    if (p.GetPlayerId() == player_id) {
      p.MarkReady();
      return;
    }
  }
}

// =============================================================================

void GameSession::EnqueueInput(const InputCommand &command) {
  pending_inputs_.push(command);
}

void GameSession::StartCountdown() {
  if (state_ == SessionState::WaitingForPlayers) {
    state_ = SessionState::Countdown;
  }
}

// =============================================================================

void GameSession::StartMatch() {
  if (state_ == SessionState::Countdown) {
    state_ = SessionState::InProgress;
  }
}

// =============================================================================

void GameSession::Tick(std::chrono::milliseconds delta) {
  if (state_ != SessionState::InProgress)
    return;

  double dt = delta.count() / 1000.0;
  current_tick_++;

  ApplyInputs_();
  UpdateBirds_(dt);
  UpdatePipes_(dt);
  DetectCollisions_();
  CheckFinishConditions_();
}

// =============================================================================

bool GameSession::IsFinished() const {
  return state_ == SessionState::Finished;
}

// =============================================================================

WorldSnapshot GameSession::BuildSnapshot() const {
  WorldSnapshot snap;
  snap.session_id = id_.ToString();
  snap.tick = current_tick_;
  snap.state = state_;

  for (const auto &player : players_) {
    const auto &bird = player.GetBird();
    PlayerSnapshot ps{player.GetPlayerId().ToString(),
                      bird.x,
                      bird.y,
                      bird.velocity_y,
                      bird.alive,
                      bird.passed_pipes, // score
                      bird.distance};
    snap.players.push_back(ps);
  }

  for (const auto &pipe : pipes_) {
    PipeSnapshot ps{pipe.x, pipe.gap_y, pipe.gap_height};
    snap.pipes.push_back(ps);
  }
  return snap;
}

// =============================================================================

MatchResult GameSession::BuildResult() const {
  MatchResult result;

  for (const auto &p : players_) {
    result.rankings.push_back(MatchResult::Entry{
        p.GetPlayerId(), p.GetBird().distance, p.GetBird().passed_pipes});
  }
  std::sort(result.rankings.begin(), result.rankings.end(),
            [](const MatchResult::Entry &a, const MatchResult::Entry &b) {
              return a.distance > b.distance;
            });
  return result;
}

// ==================== PRIVATE METHODS ====================

void GameSession::ApplyInputs_() {
  while (!pending_inputs_.empty()) {
    InputCommand cmd = pending_inputs_.front();
    pending_inputs_.pop();

    if (cmd.type != InputType::Jump)
      continue;

    for (auto &player : players_) {
      if (player.GetPlayerId() == cmd.player_id && player.GetBird().alive) {
        physics_engine_.ApplyJump(player.GetBird());
      }
    }
  }
}

// =============================================================================

void GameSession::UpdateBirds_(double dt) {
  for (auto &player : players_) {
    auto &bird = player.GetBird();
    if (!bird.alive)
      continue;

    physics_engine_.ApplyGravity(bird, dt);
    physics_engine_.UpdatePosition(bird, dt);

    // distance — просто накопленное расстояние мира (для UI/тай-брейка)
    bird.distance += physics_config_.scroll_speed * dt;
  }
}

// =============================================================================

void GameSession::UpdatePipes_(double dt) {
  double scroll = physics_config_.scroll_speed * dt;

  for (auto &pipe : pipes_) {
    pipe.x -= scroll;
  }

  // удаляем улетевшие влево
  auto it = std::remove_if(pipes_.begin(), pipes_.end(),
                           [](const Pipe &p) { return p.x + p.width < 0.0; });
  pipes_.erase(it, pipes_.end());

  // добавляем новые (видимая область ~1000px вправо от bird_x)
  obstacle_generator_.EnsureEnoughPipes(pipes_, 1000.0);
}

void GameSession::DetectCollisions_() {
  for (auto &player : players_) {
    auto &bird = player.GetBird();
    if (collision_.HasCollided(bird, physics_config_, pipes_)) {
      bird.alive = false;
    }
  }
}

// =============================================================================

void GameSession::CheckFinishConditions_() {
  bool anyone_alive = false;
  for (const auto &p : players_) {
    if (p.GetBird().alive) {
      anyone_alive = true;
      break;
    }
  }
  if (!anyone_alive && !players_.empty()) {
    state_ = SessionState::Finished;
  }
}
