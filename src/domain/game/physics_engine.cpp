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

