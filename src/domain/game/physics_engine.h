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
