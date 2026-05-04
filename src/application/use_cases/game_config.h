// application/use_cases/game_config.h
#pragma once
#include "domain/game/physics_config.h"
#include <cstdint>

class GameConfig {
public:
  GameConfig(PhysicsConfig config);
  const PhysicsConfig &GetPhysicsConfig() { return config_; }
  void SetPhysicsConfigGravity(double gravity) { config_.gravity = gravity; }
  void SetPhysicsConfigJumpVelocity(double jump_velocity) {
    config_.jump_velocity = jump_velocity;
  }
  void SetPhysicsConfigWorldHeight(double world_height) {
    config_.world_height = world_height;
  }
  void SetPhysicsConfigScrollSpeed(double scroll_speed) {
    config_.scroll_speed = scroll_speed;
  }
  const std::uint32_t GetSeedNumber() const { return 42u; }

private:
  PhysicsConfig config_;
};
