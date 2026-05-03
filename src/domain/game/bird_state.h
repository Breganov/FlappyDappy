// domain/game/bird_state.h
#pragma once

struct BirdState {
  double y = 300.0f;
  double x = 100.0f;
  double velocity_y = 0.0f;
  bool alive = true;
  int passed_pipes = 0;
  double distance = 0.0f;
  double radius = 20.0f;
};
