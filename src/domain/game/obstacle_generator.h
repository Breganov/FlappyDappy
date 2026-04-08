#pragma once
#include <cstdint>
#include <random>
#include <vector>

#include "pipe.h"

class ObstacleGenerator {
public:
  explicit ObstacleGenerator(std::uint32_t seed);
  
  std::vector<Pipe> InitialPipes() const;
  void EnsureEnoughPipes(std::vector<Pipe>& pipes, double right_edge_x);
private:
  mutable std::mt19937 gen_;
  std::uniform_real_distribution<double> gap_dist_;
};
