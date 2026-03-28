#pragma once
#include <cstdint>
#include <vector>

#include "pipe.h"

class ObstacleGenerator {
public:
  explicit ObstacleGenerator(std::uint32_t seed);
  
  std::vector<Pipe> InitialPipes() const;
  void EnsureEnoughPipes(std::vector<Pipe>& pipes, double right_edge_x);
};
