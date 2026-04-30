// collision_service.h
#pragma once
#include <vector>

#include "bird_state.h"
#include "physics_config.h"
#include "pipe.h"

class CollisionService {
public:
  bool HasCollided(const BirdState &bird, const PhysicsConfig &phys,
                   const std::vector<Pipe> &pipes) const;

private:
  bool HasHitPipe(const BirdState &bird, const PhysicsConfig &phys,
                  const Pipe &pipe) const;
};
