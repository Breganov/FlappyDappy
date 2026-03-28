#include "collision_service.h"
#include "bird_state.h"

bool CollisionService::HasCollided(const BirdState& bird, const PhysicsConfig& phys, const std::vector<Pipe>& pipes) const {
  
  if (!bird.alive) {
    return false;
  }

  if (HasHitBounds(bird, phys)) {
    return true;
  }

  return false;
}

bool CollisionService::HasHitBounds(const BirdState& bird,
                                    const PhysicsConfig& phys) const {
  return bird.y - bird.radius < 0.0f ||
         bird.y + bird.radius > phys.world_height;
}
