#pragma once
// TODO: replace per-pipe global marker with per-player scoring state
struct Pipe {
  double x = 0.0f;
  double width = 60.0f;
  double gap_y = 120.0f;
  double gap_height = 140.0f;
  bool passed_by_player_logic_marker = false;
};
