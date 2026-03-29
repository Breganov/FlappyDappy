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
