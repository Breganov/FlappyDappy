#pragma once

struct BirdState {
  double y = 0.0;
  double x = 0.0;
  double velocity_y = 0.0;
  bool alive = true;
  int passed_pipes = 0;
  double distance = 0.0;
  double radius = 0.0;
};
