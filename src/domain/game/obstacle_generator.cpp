#include "obstacle_generator.h"
#include "pipe.h"
#include "physics_config.h"
#include <random>
#include <algorithm>

ObstacleGenerator::ObstacleGenerator(std::uint32_t seed)
  : gen_(seed)
  , gap_dist_(80.0, 380.0)  // gap_y в пределах видимого мира 600px
                            // надо вывести эти значения в world настройки
{}

std::vector<Pipe> ObstacleGenerator::InitialPipes() const {
  std::vector<Pipe> pipes;
  double x = 400.0; // не это точно надо куда-то вытаскивать
  for (int i = 0; i < 4; ++i) {
    Pipe p;
    p.x = x;
    p.width = 60.0;
    p.gap_height = 140.0;
    p.gap_y = gap_dist_(gen_);
    p.passed_by_player_logic_marker = false;
    pipes.push_back(p);
    x += 250.0;
  }
  return pipes;
}

void ObstacleGenerator::EnsureEnoughPipes(std::vector<Pipe>& pipes, double right_edge_x) {
  while (pipes.empty() || pipes.back().x < right_edge_x) {
    double new_x = pipes.empty() ? 400.0 : pipes.back().x + 250.0;
    Pipe p;
    p.x = new_x;
    p.width = 60.0;
    p.gap_height = 140.0;
    p.gap_y = gap_dist_(gen_);
    p.passed_by_player_logic_marker = false;
    pipes.push_back(p);
  }
}
