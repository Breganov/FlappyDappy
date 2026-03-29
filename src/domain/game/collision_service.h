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
