#include "game_session.h"

GameSession::GameSession(SessionId id, std::uint32_t seed, double gravity,
                         double jump_velocity)
    : id_(std::move(id)), seed_(seed), obstacle_generator_(seed),
      pipes_(obstacle_generator_.InitialPipes()), current_tick_(0),
      physics_config_(),
      physics_engine_(PhysicsEngine{gravity, jump_velocity}) {}

const SessionId &GameSession::GetId() const { return id_; }

SessionState GameSession::GetState() const { return state_; }

void GameSession::AddPlayer(PlayerId player_id) {
  BirdState initial_bird{
      physics_config_.initial_bird_y, 0.0f, 0.0f, true, 0, 0.0,
      physics_config_.bird_radius};

  // initial_bird.y = 300.0;           // центр экрана
  // initial_bird.x = 0.0;             // не используется (bird_x в config)
  // initial_bird.velocity_y = 0.0;
  // initial_bird.alive = true;
  // initial_bird.passed_pipes = 0;
  // initial_bird.distance = 0.0;
  // initial_bird.radius = 20.0;

  players_.emplace_back(std::move(player_id), initial_bird);
}

void GameSession::MarkPlayerReady(const PlayerId &player_id) {
  for (auto &p : players_) {
    if (p.GetPlayerId() == player_id) {
      p.MarkReady();
      return;
    }
  }
}

void GameSession::EnqueueInput(const InputCommand &command) {
  pending_inputs_.push(command);
}

void GameSession::StartCountdown() {
  if (state_ == SessionState::WaitingForPlayers) {
    state_ = SessionState::Countdown;
  }
}

void GameSession::StartMatch() {
  if (state_ == SessionState::Countdown) {
    state_ = SessionState::InProgress;
  }
}

void GameSession::Tick(std::chrono::milliseconds delta) {
  if (state_ != SessionState::InProgress)
    return;

  double dt = delta.count() / 1000.0;
  current_tick_++;

  ApplyInputs_();
  UpdateBirds_(dt);
  UpdatePipes_(dt);
  DetectCollisions_();
  UpdateScores_();
  CheckFinishConditions_();
}

bool GameSession::IsFinished() const {
  return state_ == SessionState::Finished;
}

WorldSnapshot GameSession::BuildSnapshot() const {
  WorldSnapshot snap;
  snap.session_id = id_.ToString();
  snap.tick = current_tick_;
  snap.state = state_;

  for (const auto &player : players_) {
    const auto &bird = player.GetBird();
    PlayerSnapshot ps{player.GetPlayerId().ToString(),
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

MatchResult GameSession::BuildResult() const {
  MatchResult result;
  std::vector<std::pair<PlayerId, int>> temp;
  for (const auto &p : players_) {
    temp.emplace_back(p.GetPlayerId(), p.GetBird().passed_pipes);
  }
  std::sort(temp.begin(), temp.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  for (auto &pr : temp) {
    result.rankings.push_back({std::move(pr.first), pr.second});
  }
  return result;
}

// ==================== PRIVATE ====================

void GameSession::ApplyInputs_() {
  PhysicsEngine engine(900.0, -300.0); // из PhysicsConfig

  while (!pending_inputs_.empty()) {
    InputCommand cmd = pending_inputs_.front();
    pending_inputs_.pop();

    if (cmd.type != InputType::Jump)
      continue;

    for (auto &player : players_) {
      if (player.GetPlayerId() == cmd.player_id && player.GetBird().alive) {
        engine.ApplyJump(player.GetBird());
      }
    }
  }
}

void GameSession::UpdateBirds_(double dt) {
  PhysicsEngine engine(900.0, -300.0);
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};

  for (auto &player : players_) {
    auto &bird = player.GetBird();
    if (!bird.alive)
      continue;

    engine.ApplyGravity(bird, dt);
    engine.UpdatePosition(bird, dt);

    // distance — просто накопленное расстояние мира (для UI/тай-брейка)
    bird.distance += phys.scroll_speed * dt;
  }
}

void GameSession::UpdatePipes_(double dt) {
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};
  double scroll = phys.scroll_speed * dt;

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
  CollisionService collision;
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};

  for (auto &player : players_) {
    auto &bird = player.GetBird();
    if (collision.HasCollided(bird, phys, pipes_)) {
      bird.alive = false;
    }
  }
}

void GameSession::UpdateScores_() {
  // ← Вот и решение "score rules" из next.md
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};

  for (auto &pipe : pipes_) {
    if (pipe.passed_by_player_logic_marker)
      continue;

    if (pipe.x + pipe.width < phys.bird_x) {
      // Труба прошла bird_x — даём очко всем, кто ещё жив
      for (auto &player : players_) {
        if (player.GetBird().alive) {
          player.GetBird().passed_pipes++;
        }
      }
      pipe.passed_by_player_logic_marker = true;
    }
  }
}

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
