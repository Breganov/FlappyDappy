// infrestructure/net/game_loop_service.h
#pragma once

#include "application/use_cases/game_loop_service.h"

#include <boost/asio/steady_timer.hpp>
#include <chrono>

// Fires GameLoopService::Execute at a fixed rate on the io_context thread.
// Use expires_at (not expires_after) so timer drift does not accumulate.

class GameLoopTimer {
public:
  GameLoopTimer(boost::asio::io_context &io_context, GameLoopService &loop,
                std::chrono::milliseconds tick_interval);

  void Start();

private:
  void ScheduleNext_();

private:
  boost::asio::steady_timer timer_;
  GameLoopService &loop_;
  std::chrono::milliseconds tick_interval_;
  std::chrono::steady_clock::time_point next_tick_;
};
