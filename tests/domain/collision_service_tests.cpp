// collision_service_tests.cpp
// #include <catch2/catch.hpp>
#include <catch2/catch_test_macros.hpp>
#include "domain/game/collision_service.h"

TEST_CASE("Bird hits top bound") {
  CollisionService collision;

  BirdState bird;
  bird.y = 5.0f;
  bird.radius = 10.0f;
  bird.alive = true;

  PhysicsConfig phys;
  phys.world_height = 600.0f;
  phys.bird_x = 100.0f;

  // x = 0.0f; width = 60.0f; gap_y = 120.0f; gap_height = 140.0f; passed_by_player_logic_marker = false;
  REQUIRE(collision.HasCollided(bird, phys, {}));
}

TEST_CASE("Bird hits bottom bound") {
  CollisionService collision;

  BirdState bird;
  bird.y = 595.0f;
  bird.radius = 10.0f;
  bird.alive = true;

  PhysicsConfig phys;
  phys.world_height = 600.0f;
  phys.bird_x = 100.0f;

  // x = 0.0f; width = 60.0f; gap_y = 120.0f; gap_height = 140.0f; passed_by_player_logic_marker = false;
  REQUIRE(collision.HasCollided(bird, phys, {}));
}

TEST_CASE("Bird does not collide inside the gap") {
  CollisionService collision;

  BirdState bird;
  bird.y = 170.0f;
  bird.radius = 10.0f;
  bird.alive = true;

  PhysicsConfig phys;
  phys.world_height = 600.0f;
  phys.bird_x = 100.0f;


  // x = 0.0f; width = 60.0f; gap_y = 120.0f; gap_height = 140.0f; passed_by_player_logic_marker = false;
  std::vector<Pipe> pipes {
    Pipe{90.0f, 60.0f, 120.0f, 100.0f}
  };

  REQUIRE_FALSE(collision.HasCollided(bird, phys, pipes));
}

TEST_CASE("Bird collides with pipe when outside gap") {
  CollisionService collision;

  BirdState bird;
  bird.y = 110.0f;
  bird.radius = 15.0f;
  bird.alive = true;

  PhysicsConfig phys;
  phys.world_height = 600.0f;
  phys.bird_x = 100.0f;

  std::vector<Pipe> pipes{
    Pipe{90.0f, 60.0f, 120.0f, 100.0f}
  };

  // x = 0.0f; width = 60.0f; gap_y = 120.0f; gap_height = 140.0f; passed_by_player_logic_marker = false;
  REQUIRE(collision.HasCollided(bird, phys, pipes));
}
