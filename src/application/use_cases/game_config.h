// application/use_cases/game_config.h
#pragma once
#include "domain/game/physics_config.h"
#include <cstdint>

class GameConfig {
public:
  GameConfig() = default;
  explicit GameConfig(PhysicsConfig config) : config_(config) {}

  PhysicsConfig &GetPhysicsConfig() { return config_; }
  const PhysicsConfig &GetPhysicsConfig() const { return config_; }

  void SetSeedUnmber(std::uint32_t seed) { seed_ = seed; }
  std::uint32_t GetSeedNumber() const { return seed_; }

private:
  PhysicsConfig config_;
  std::uint32_t seed_ = 42u;
};
