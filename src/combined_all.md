# Combined project files

**Root:** `C:/Code/C++/FlappyDappy/src`

## Project tree

```text
src
├── application
│   ├── ports
│   │   ├── id_generator.h
│   │   ├── leaderboard_repository.h
│   │   ├── match_repository.h
│   │   ├── player_repository.h
│   │   ├── session_broadcaster.h
│   │   ├── session_repository.h
│   │   └── unit_of_work.h
│   └── use_cases
│       ├── create_session_use_case.h
│       ├── finish_match_use_case.h
│       ├── game_loop_service.h
│       ├── get_leaderboard_use_case.h
│       ├── join_session_use_case.h
│       ├── leaderboard_service.h
│       ├── mark_ready_use_case.h
│       ├── matchmaking_service.h
│       ├── reconnect_player_use_case.h
│       ├── session_service.h
│       ├── start_match_use_case.h
│       ├── submit_input_use_case.h
│       └── tick_session_use_case.h
├── domain
│   ├── game
│   │   ├── bird_state.h
│   │   ├── collision_service.cpp
│   │   ├── collision_service.h
│   │   ├── obstacle_generator.cpp
│   │   ├── obstacle_generator.h
│   │   ├── physics_config.h
│   │   ├── physics_engine.cpp
│   │   ├── physics_engine.h
│   │   ├── pipe.h
│   │   ├── score_service.h
│   │   └── world_snapshot.h
│   ├── match
│   │   ├── match_result.h
│   │   └── ranking_service.h
│   ├── player
│   │   ├── nickname.h
│   │   ├── player.h
│   │   └── player_id.h
│   └── session
│       ├── game_session.cpp
│       ├── game_session.h
│       ├── input_command.h
│       ├── player_session_state.h
│       ├── session_id.h
│       └── session_state.h
├── infrastructure
│   ├── logging
│   │   ├── console_logger.cpp
│   │   ├── console_logger.h
│   │   └── logger.h
│   └── net
│       ├── websocket_message_parser.h
│       ├── websocket_message_serializer.h
│       ├── websocket_router.h
│       ├── websocket_server.h
│       └── websocket_session.h
├── presentation
│   └── http
│       ├── healthcheck_handler.h
│       ├── http_router.h
│       ├── http_server.h
│       └── leaderboard_handler.h
└── main.cpp
```

## Files

### `application/ports/id_generator.h`

```cpp
class IIdGenerator {
public:
  virtual ~IIdGenerator() = default;
  virtual SessionId NewSessionId() = 0;
  virtual MatchId NewMatchId() = 0;
};
```

### `application/ports/leaderboard_repository.h`

```cpp
#include <cstddef>
#include <vector>

class ILeaderboardRepository {
public:
  virtual ~ILeaderboardRepository() = default;
  virtual std::vector<LeaderboardEntry> GetTop(std::size_t limit) = 0;
};
```

### `application/ports/match_repository.h`

```cpp
class IMatchRepository {
public:
  virtual ~IMatchRepository() = default;
  virtual void SaveMatchResult(const MatchResult& result) = 0;
  virtual std::optional<MatchResult> FindById(const MatchId& id) = 0;
};
```

### `application/ports/player_repository.h`

```cpp
class IPlayerRepository {
public:
  virtual ~IPlayerRepository() = default;
  virtual std::optional<Player> FindById(const PlayerId& id) = 0;
  virtual void Save(const Player& player) = 0;
};
```

### `application/ports/session_broadcaster.h`

```cpp
// Application должна уметь рассылать сообщения не зная про WebSocket
class ISessionBroadcaster {
public:
  virtual ~ISessionBroadcaster() = default;
  virtual void BroadcastSnapshot(
    const SessionId& session_id,
    const WorldSnapshot& snapshot) = 0;

  virtual void BroadcastMatchFinished(
    const SessionId& session_id,
    const MatchResult& result) = 0;

  virtual void NotifyPlayerJoined(
    const SessionId& session_id,
    const PlayerId& player_id) = 0;
};
```

### `application/ports/session_repository.h`

```cpp
```

### `application/ports/unit_of_work.h`

```cpp
// для того, чтобы придерживаться ACID
#pragma once

class IUnitOfWork {
public:
  virtual ~IUnitOfWork() = default;
  // virtual IPlayerRepository& Players = 0;
  virtual IMatchRepository& Matches() = 0;
  virtual ILeaderboardRepository& Leaderboard() = 0;
  virtual void Commit() = 0;
  virtual void Rollback() = 0;
};
```

### `application/use_cases/create_session_use_case.h`

```cpp
```

### `application/use_cases/finish_match_use_case.h`

```cpp
class FinishMatchUseCases {
public:
  FinishMatchUseCases(
    IUnitOfWorkFactory& uow_factory,
    ISessionBroadcaster& broadcaster,
    SessionService& sessions);

  void Execute(const SessionId& session_id);
};
```

### `application/use_cases/game_loop_service.h`

```cpp
```

### `application/use_cases/get_leaderboard_use_case.h`

```cpp
```

### `application/use_cases/join_session_use_case.h`

```cpp
// Найти и создать сессию
// Добавить игрока
// Уведомить остальных
class JoinSessionUseCase {
public:
  JoinSessionUseCase(SessionService& session, ISessionBroadcaster& broadcaster);

  SessionId Execute(const PlayerId& player_id);
};
```

### `application/use_cases/leaderboard_service.h`

```cpp
```

### `application/use_cases/mark_ready_use_case.h`

```cpp
class MarkReadyUseCases {
public:
  void Execute(const SessionId& session_id, const PlayerId& player_id);
};
```

### `application/use_cases/matchmaking_service.h`

```cpp
// Ждём пока не наборалось N игроков
class MatchmakingService {
public:
  SessionId AssignPlayerToSession(const PlayerId& player_id);
};
```

### `application/use_cases/reconnect_player_use_case.h`

```cpp
```

### `application/use_cases/session_service.h`

```cpp
// session_service.h держит активные игровые сессии в памяти, добавляет, ищет и удаляет.
#include <functional>
#include <string>
#include <unordered_map>

class SessionService {
public:
  GameSession& CreateSession();
  std::optional<std::reference_wrapper<GameSession>> FindSession(const Session& id);
  void RemoveSession(const Session& id);

private:
  std::unordered_map<std::string, GameSession> sessions_;
};
```

### `application/use_cases/start_match_use_case.h`

```cpp
```

### `application/use_cases/submit_input_use_case.h`

```cpp
class SubmitInputUseCase {
public:
  void Execute(const SessionId& session_id, const InputCommand& command);
};
```

### `application/use_cases/tick_session_use_case.h`

```cpp
class TickSessionUseCase {
public:
  void Execute(const SessionId& session_id, std::chrono::milliseconds delta);
};
```

### `domain/game/bird_state.h`

```cpp
#pragma once

struct BirdState {
  double y = 0.0;
  double x = 0.0;
  double velocity_y = 0.0;
  bool alive = true;
  int passed_pipes = 0;
  double distance = 0.0;
  double radius = 0.0;
};
```

### `domain/game/collision_service.cpp`

```cpp
// collision_service.cpp
#include "collision_service.h"
#include "bird_state.h"

bool CollisionService::HasCollided(const BirdState& bird, const PhysicsConfig& phys, const std::vector<Pipe>& pipes) const {
  
  if (!bird.alive) {
    return false;
  }

  if (HasHitBounds(bird, phys)) {
    return true;
  }
  
  for(const Pipe& pipe : pipes) {
    if (HasHitPipe(bird, phys, pipe)) {
      return true;
    }
  }

  return false;
}

bool CollisionService::HasHitBounds(const BirdState& bird,
                                    const PhysicsConfig& phys) const {
  return bird.y - bird.radius < 0.0f ||
         bird.y + bird.radius > phys.world_height;
}

bool CollisionService::HasHitPipe(const BirdState& bird,
                                  const PhysicsConfig& phys,
                                  const Pipe& pipe) const {
  const float bird_left = phys.bird_x - bird.radius;
  const float bird_right = phys.bird_x + bird.radius;
  const float bird_top = bird.y - bird.radius;
  const float bird_bottom = bird.y + bird.radius;


  const float pipe_left = pipe.x;
  const float pipe_right = pipe.x + pipe.width;

  const bool overlaps_x =
    bird_right > pipe_left &&
    bird_left < pipe_right;

  if (!overlaps_x) {
    return false;
  }

  const float gap_top = pipe.gap_y;
  const float gap_bottom = pipe.gap_y + pipe.gap_height;

  const bool inside_gap =
    bird_top >= gap_top &&
    bird_bottom <= gap_bottom;

  if (!inside_gap) {
    return true;
  }

  return false;
}
```

### `domain/game/collision_service.h`

```cpp
// collision_service.h
#pragma once
#include <vector>

#include "bird_state.h"
#include "pipe.h"
#include "physics_config.h"

class CollisionService {
public:
  bool HasCollided(const BirdState& bird,
                   const PhysicsConfig& phys,
                   const std::vector<Pipe>& pipes) const;
private:
  bool HasHitBounds(const BirdState& bird,
                    const PhysicsConfig& phys) const;

  bool HasHitPipe(const BirdState& bird,
                  const PhysicsConfig& phys,
                  const Pipe& pipe) const;
};
```

### `domain/game/obstacle_generator.cpp`

```cpp
#include "obstacle_generator.h"
#include "pipe.h"
#include "physics_config.h"
#include <random>
#include <algorithm>

ObstacleGenerator::ObstacleGenerator(std::uint32_t seed)
  : gen_(seed)
  , gap_dist_(80.0, 380.0)  // gap_y в пределах видимого мира 600px
                            // надо вывести эти значения в world настройки
{}

std::vector<Pipe> ObstacleGenerator::InitialPipes() const {
  std::vector<Pipe> pipes;
  double x = 400.0; // не это точно надо куда-то вытаскивать
  for (int i = 0; i < 4; ++i) {
    Pipe p;
    p.x = x;
    p.width = 60.0;
    p.gap_height = 140.0;
    p.gap_y = gap_dist_(gen_);
    p.passed_by_player_logic_marker = false;
    pipes.push_back(p);
    x += 250.0;
  }
  return pipes;
}

void ObstacleGenerator::EnsureEnoughPipes(std::vector<Pipe>& pipes, double right_edge_x) {
  while (pipes.empty() || pipes.back().x < right_edge_x) {
    double new_x = pipes.empty() ? 400.0 : pipes.back().x + 250.0;
    Pipe p;
    p.x = new_x;
    p.width = 60.0;
    p.gap_height = 140.0;
    p.gap_y = gap_dist_(gen_);
    p.passed_by_player_logic_marker = false;
    pipes.push_back(p);
  }
}
```

### `domain/game/obstacle_generator.h`

```cpp
#pragma once
#include <cstdint>
#include <random>
#include <vector>

#include "pipe.h"

class ObstacleGenerator {
public:
  explicit ObstacleGenerator(std::uint32_t seed);
  
  std::vector<Pipe> InitialPipes() const;
  void EnsureEnoughPipes(std::vector<Pipe>& pipes, double right_edge_x);
private:
  mutable std::mt19937 gen_;
  std::uniform_real_distribution<double> gap_dist_;
};
```

### `domain/game/physics_config.h`

```cpp
#pragma once

struct PhysicsConfig {
  double gravity = 900.0;
  double jump_velocity = -300.0;
  double world_height = 600.0;
  double scroll_speed = 120.0;
  double bird_x = 100.0f;
};
```

### `domain/game/physics_engine.cpp`

```cpp
#include "physics_engine.h"
#include "bird_state.h"

PhysicsEngine::PhysicsEngine(double gravity, double jump_velocity) : gravity_(gravity), jump_velocity_(jump_velocity) {}

void PhysicsEngine::ApplyJump(BirdState& bird) const {
  bird.velocity_y = jump_velocity_;
}

void PhysicsEngine::ApplyGravity(BirdState& bird, double dt) const {
  if (!bird.alive) {
    return;
  }
  bird.velocity_y += gravity_ * dt;
}

void PhysicsEngine::UpdatePosition(BirdState& bird, double dt) const {
  if (!bird.alive) {
    return;
  }
  bird.y += bird.velocity_y * dt;
}

```

### `domain/game/physics_engine.h`

```cpp
#pragma once

#include "bird_state.h"

class PhysicsEngine {
public:
  PhysicsEngine(double gravity, double jump_velocity);

  void ApplyJump(BirdState& bird) const;
  void ApplyGravity(BirdState& bird, double dt) const;
  void UpdatePosition(BirdState& bird, double dt) const;

private:
  double gravity_;
  double jump_velocity_;
};
```

### `domain/game/pipe.h`

```cpp
#pragma once
// TODO: replace per-pipe global marker with per-player scoring state
struct Pipe {
  double x = 0.0f;
  double width = 60.0f;
  double gap_y = 120.0f;
  double gap_height = 140.0f;
  bool passed_by_player_logic_marker = false;
};
```

### `domain/game/score_service.h`

```cpp
#include <vector>

#include "../session/player_session_state.h"
#include "pipe.h"

class ScoreService {
public:
  void UpdateScore(PlayerSessionState& player, const std::vector<Pipe>& pipes) const;
};
```

### `domain/game/world_snapshot.h`

```cpp
#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct PlayerSnapshot {
  std::string player_id;
  double y;
  double velocity_y;
  bool alive;
  int score;
  double distance;
};

struct PipeSnapshot {
  double x;
  double gap_y;
  double gap_height;
};

struct WorldSnapshot {
  std::string session_id;
  std::uint64_t tick;
  SessionState state;
  std::vector<PlayerSnapshot> players;
  std::vector<PipeSnapshot> pipes;
};
```

### `domain/match/match_result.h`

```cpp
#pragma once
#include <vector>
#include "../player/player_id.h"

struct MatchResult {
  struct Entry {
    PlayerId player_id;
    int score;
  };
  std::vector<Entry> rankings;  // отсортировано по убыванию score
};
```

### `domain/match/ranking_service.h`

```cpp
#include <vector>

#include "../session/player_session_state.h"
#include "../game/pipe.h"

class RankingService {
public:
  void UpdateScore(PlayerSessionState& player, const std::vector<Pipe>& pipes) const;
};
```

### `domain/player/nickname.h`

```cpp
```

### `domain/player/player.h`

```cpp
class Player {
  public:
    const PlayerId& GetId() const;
    const std::string& GetNickname() const;
  private id_;
  std::string nickname_;
};
```

### `domain/player/player_id.h`

```cpp
// src\domain\player\player_id.h
#pragma once
#include <string>

class PlayerId {
public:
  explicit PlayerId(std::string value);
  const std::string& ToString() const;
  bool operator==(const PlayerId&) const = default;

private:
  std::string value_;
};
```

### `domain/session/game_session.cpp`

```cpp
#include "game_session.h"

GameSession::GameSession(SessionId id, std::uint32_t seed)
  : id_(std::move(id))
  , seed_(seed)
  , obstacle_generator_(seed)
  , pipes_(obstacle_generator_.InitialPipes())
  , current_tick_(0) {}

const SessionId& GameSession::GetId() const { return id_; }

SessionState GameSession::GetState() const { return state_; }

void GameSession::AddPlayer(PlayerId player_id) {
  BirdState initial_bird;
  initial_bird.y = 300.0;           // центр экрана
  initial_bird.x = 0.0;             // не используется (bird_x в config)
  initial_bird.velocity_y = 0.0;
  initial_bird.alive = true;
  initial_bird.passed_pipes = 0;
  initial_bird.distance = 0.0;
  initial_bird.radius = 20.0;

  players_.emplace_back(std::move(player_id), initial_bird);
}

void GameSession::MarkPlayerReady(const PlayerId& player_id) {
  for (auto& p : players_) {
    if (p.GetPlayerId() == player_id) {
      p.MarkReady();
      return;
    }
  }
}

void GameSession::EnqueueInput(const InputCommand& command) {
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
  if (state_ != SessionState::InProgress) return;

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

  for (const auto& player : players_) {
    const auto& bird = player.GetBird();
    PlayerSnapshot ps{
      player.GetPlayerId().ToString(),
      bird.y,
      bird.velocity_y,
      bird.alive,
      bird.passed_pipes,   // score
      bird.distance
    };
    snap.players.push_back(ps);
  }

  for (const auto& pipe : pipes_) {
    PipeSnapshot ps{pipe.x, pipe.gap_y, pipe.gap_height};
    snap.pipes.push_back(ps);
  }
  return snap;
}

MatchResult GameSession::BuildResult() const {
  MatchResult result;
  std::vector<std::pair<PlayerId, int>> temp;
  for (const auto& p : players_) {
    temp.emplace_back(p.GetPlayerId(), p.GetBird().passed_pipes);
  }
  std::sort(temp.begin(), temp.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

  for (auto& pr : temp) {
    result.rankings.push_back({std::move(pr.first), pr.second});
  }
  return result;
}

// ==================== PRIVATE ====================

void GameSession::ApplyInputs_() {
  PhysicsEngine engine(900.0, -300.0);  // из PhysicsConfig

  while (!pending_inputs_.empty()) {
    InputCommand cmd = pending_inputs_.front();
    pending_inputs_.pop();

    if (cmd.type != InputType::Jump) continue;

    for (auto& player : players_) {
      if (player.GetPlayerId() == cmd.player_id && player.GetBird().alive) {
        engine.ApplyJump(player.GetBird());
      }
    }
  }
}

void GameSession::UpdateBirds_(double dt) {
  PhysicsEngine engine(900.0, -300.0);
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};

  for (auto& player : players_) {
    auto& bird = player.GetBird();
    if (!bird.alive) continue;

    engine.ApplyGravity(bird, dt);
    engine.UpdatePosition(bird, dt);

    // distance — просто накопленное расстояние мира (для UI/тай-брейка)
    bird.distance += phys.scroll_speed * dt;
  }
}

void GameSession::UpdatePipes_(double dt) {
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};
  double scroll = phys.scroll_speed * dt;

  for (auto& pipe : pipes_) {
    pipe.x -= scroll;
  }

  // удаляем улетевшие влево
  auto it = std::remove_if(pipes_.begin(), pipes_.end(),
    [](const Pipe& p) { return p.x + p.width < 0.0; });
  pipes_.erase(it, pipes_.end());

  // добавляем новые (видимая область ~1000px вправо от bird_x)
  obstacle_generator_.EnsureEnoughPipes(pipes_, 1000.0);
}

void GameSession::DetectCollisions_() {
  CollisionService collision;
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};

  for (auto& player : players_) {
    auto& bird = player.GetBird();
    if (collision.HasCollided(bird, phys, pipes_)) {
      bird.alive = false;
    }
  }
}

void GameSession::UpdateScores_() {
  // ← Вот и решение "score rules" из next.md
  PhysicsConfig phys{900.0, -300.0, 600.0, 120.0, 100.0};

  for (auto& pipe : pipes_) {
    if (pipe.passed_by_player_logic_marker) continue;

    if (pipe.x + pipe.width < phys.bird_x) {
      // Труба прошла bird_x — даём очко всем, кто ещё жив
      for (auto& player : players_) {
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
  for (const auto& p : players_) {
    if (p.GetBird().alive) {
      anyone_alive = true;
      break;
    }
  }
  if (!anyone_alive && !players_.empty()) {
    state_ = SessionState::Finished;
  }
}
```

### `domain/session/game_session.h`

```cpp
// domain\session\games_session.h
# pragma once

// #include <algorithm>
#include <chrono>
#include <cstdint>
#include <queue>
// #include <utility>
#include <vector>

#include "input_command.h"
#include "input_command.h"
#include "player_session_state.h"
#include "player_session_state.h"
#include "session_state.h"

#include "../game/collision_service.h"
#include "../game/obstacle_generator.h"
#include "../game/physics_config.h"
#include "../game/physics_engine.h"
#include "../game/pipe.h"
#include "../game/world_snapshot.h"
#include "../match/match_result.h"
#include "../session/session_id.h"

class GameSession {
public:
  GameSession(SessionId id, std::uint32_t seed);
  const SessionId& GetId() const;
  SessionState GetState() const;

  void AddPlayer(PlayerId player_id);
  void MarkPlayerReady(const PlayerId& palyer_id);
  void EnqueueInput(const InputCommand& command);

  void StartCountdown();
  void StartMatch();
  void Tick(std::chrono::milliseconds delta);

  bool IsFinished() const;
  WorldSnapshot BuildSnapshot() const;
  MatchResult BuildResult() const;

private:
  void ApplyInputs_();
  void UpdateBirds_(double dt);
  void UpdatePipes_(double dt);
  void DetectCollisions_();
  void UpdateScores_();
  void CheckFinishConditions_();

private:
  SessionId id_;
  SessionState state_ = SessionState::WaitingForPlayers;
  std::vector<PlayerSessionState> players_;
  std::vector<Pipe> pipes_;
  std::queue<InputCommand> pending_inputs_;
  std::uint32_t seed_;
  std::uint64_t current_tick_ = 0;
  ObstacleGenerator obstacle_generator_;
};
```

### `domain/session/input_command.h`

```cpp
// src\domain\session\input_command.h
#pragma once
#include "../../domain/player/player_id.h"
enum class InputType{
  Jump
};

struct InputCommand {
  PlayerId player_id;
  InputType type;
  std::uint64_t client_sequence = 0;
};
```

### `domain/session/player_session_state.h`

```cpp
#pragma once

#include "../../domain/player/player_id.h"
// #include "../session/player_id.h"
#include "../game/bird_state.h" // для BirdState

class PlayerSessionState {
public:
  PlayerSessionState(PlayerId player_id, BirdState bird);

  const PlayerId& GetPlayerId() const;
  const BirdState& GetBird() const;
  BirdState& GetBird();

  bool IsReady() const;
  void MarkReady();

  bool IsConnected() const;
  void MarkDisconnected();
  void MarkConnected();
  
private:
  PlayerId player_id_;
  BirdState bird_;
  bool ready_ = false;
  bool connect_ = true;
};
```

### `domain/session/session_id.h`

```cpp
#pragma once
#include <string>

class SessionId {
public:
  explicit SessionId(std::string value) : value_(std::move(value)) {}
  const std::string& ToString() const { return value_; }
  bool operator==(const SessionId& other) const = default;

private:
  std::string value_;
};
```

### `domain/session/session_state.h`

```cpp
#pragma once

enum class SessionState {
  WaitingForPlayers,
  Countdown,
  InProgress,
  Finished
};
```

### `infrastructure/logging/console_logger.cpp`

```cpp
#include "console_logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
const char* ToString(LogLevel level) {
  switch (level) {
    case LogLevel::Trace: return "TRACE";
    case LogLevel::Debug: return "DEBUG";
    case LogLevel::Info:  return "INFO";
    case LogLevel::Warn:  return "WARN";
    case LogLevel::Error: return "ERROR";
  }
  return "UNKNOWN";
}

bool ShouldLog(LogLevel current, LogLevel min_level) {
  return static_cast<int>(current) >= static_cast<int>(min_level);
}

ConsoleLogger::ConsoleLogger(LogLevel min_level)
  : min_level_(min_level) {}

void ConsoleLogger::Log(LogLevel level, std::string_view message) {
  if (!ShouldLog(level, min_level_)) {
    return;
  }

  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm{};
#ifdef _WIN32
  locatime_s(&local_tm, &time);
#else
  localtime_r(&time, &local_tm);
#endif
  
  std::cout
    << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S")
    << " [" << ToString(level) << "] "
    << message
    << '\n';
}

} // namespace 
```

### `infrastructure/logging/console_logger.h`

```cpp
#pragma once

#include "logger.h"
#include <string_view>

class ConsoleLogger : public ILogger {
public:
  explicit ConsoleLogger(LogLevel min_level = LogLevel::Info);

  void Log(LogLevel level, std::string_view message) override;

private:
  LogLevel min_level_;
};
```

### `infrastructure/logging/logger.h`

```cpp
// src\infrastructure\logging\logger.h
#pragma once

#include <string_view>

enum class LogLevel{
  Trace,
  Debug,
  Info,
  Warn,
  Error
};

class ILogger {
public:
  virtual ~ILogger() = default;

  virtual void Log(LogLevel level, std::string_view message) = 0;
  void Trace(std::string_view message) { Log(LogLevel::Trace, message); }
  void Debug(std::string_view message) { Log(LogLevel::Debug, message); }
  void Info(std::string_view message) { Log(LogLevel::Info, message); }
  void Warn(std::string_view message) { Log(LogLevel::Warn, message); }
  void Error(std::string_view message) { Log(LogLevel::Error, message); }
};
```

### `infrastructure/net/websocket_message_parser.h`

```cpp
```

### `infrastructure/net/websocket_message_serializer.h`

```cpp
```

### `infrastructure/net/websocket_router.h`

```cpp
```

### `infrastructure/net/websocket_server.h`

```cpp
```

### `infrastructure/net/websocket_session.h`

```cpp
#include <string>
class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
  void Start();
  void Send(std::string message);

private:
  void DoRead();
  void OnRead(...);
  void HandleMessage(std::string_view json);
}
```

### `main.cpp`

```cpp
#include "infrastructure/logging/console_logger.h"

int main() {
  ConsoleLogger logger(LogLevel::Debug);
  logger.Info("FlappyDappy server starting");

  // create services, sessions, etc.
  
  logger.Info("Server initialized");
  return 0;
}
```

### `presentation/http/healthcheck_handler.h`

```cpp
```

### `presentation/http/http_router.h`

```cpp
```

### `presentation/http/http_server.h`

```cpp
```

### `presentation/http/leaderboard_handler.h`

```cpp
```

