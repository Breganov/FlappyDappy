# Combined code documentation

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
│   │   ├── obstacle_generator.h
│   │   ├── physics_config.h
│   │   ├── physics_engine.cpp
│   │   ├── physics_engine.h
│   │   ├── pipe.h
│   │   ├── score_service.h
│   │   └── world_snapshot.h
│   ├── match
│   │   └── ranking_service.h
│   ├── player
│   │   ├── nickname.h
│   │   ├── player.h
│   │   └── player_id.h
│   └── session
│       ├── game_session.h
│       ├── input_command.h
│       ├── player_session_state.h
│       └── session_state.h
├── infrastructure
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

### `domain/game/obstacle_generator.h`

```cpp
#pragma once
#include <cstdint>
#include <vector>

#include "pipe.h"

class ObstacleGenerator {
public:
  explicit ObstacleGenerator(std::uint32_t seed);
  
  std::vector<Pipe> InitialPipes() const;
  void EnsureEnoughPipes(std::vector<Pipe>& pipes, double right_edge_x);
};
```

### `domain/game/physics_config.h`

```cpp
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

### `domain/session/game_session.h`

```cpp
# pragma once

#include "session_state.h"
#include "player_session_state.h"
#include "../game/pipe.h"
#include "input_command.h"
#include <chrono>
#include <cstdint>
#include <queue>
#include <vector>

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
}
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
  const BirdState& GetBirdId() const;
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

### `domain/session/session_state.h`

```cpp
enum class SessionState {
  WaitingForPlayers,
  Countdown,
  InProgress,
  Finished
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

### `main.cpp`

```cpp
```

