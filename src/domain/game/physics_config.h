#pragma once

struct PhysicsConfig {
  double gravity = 900.0;
  double jump_velocity = -300.0;
  double world_height = 600.0;
  double scroll_speed = 120.0;
  double bird_x = 100.0f;

  double initial_bird_y = 300.0;
  double bird_radius = 20.0;
  double pipe_spawn_right_edge = 1000.0;
};
