#include "physics_engine.h"
#include "bird_state.h"

PhysicsEngine::PhysicsEngine(double gravity, double jump_velocity,
                             double scroll_speed)
    : gravity_(gravity), jump_velocity_(jump_velocity),
      scroll_speed_(scroll_speed) {}

void PhysicsEngine::ApplyJump(BirdState &bird) const {
  bird.velocity_y = jump_velocity_;
}

void PhysicsEngine::ApplyGravity(BirdState &bird, double dt) const {
  if (!bird.alive) {
    return;
  }
  bird.velocity_y += gravity_ * dt;
}

void PhysicsEngine::UpdatePosition(BirdState &bird, double dt) const {
  if (!bird.alive) {
    return;
  }
  bird.y += bird.velocity_y * dt;
  bird.x += scroll_speed_ * dt;
}
