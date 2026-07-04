#include "game_loop_timer.h"

GameLoopTimer::GameLoopTimer(boost::asio::io_context &io_context,
                             GameLoopService &loop,
                             std::chrono::milliseconds tick_interval)
    : timer_(io_context), loop_(loop), tick_interval_(tick_interval) {}

void GameLoopTimer::Start() {
  next_tick_ = std::chrono::steady_clock::now();
  ScheduleNext_();
}

void GameLoopTimer::ScheduleNext_() {
  // Absolute deadline: if one tick runs late, the next one is still
  // scheduled on the original grid, so the simulation rate stays 62.5 Hz.
  next_tick_ += tick_interval_;
  timer_.expires_at(next_tick_);

  timer_.async_wait([this](boost::system::error_code ec) {
    if (ec) {
      return; // timer cancelled -> server shutting down
    }
    loop_.Execute(tick_interval_); // fixed delta = deterministic physics
    ScheduleNext_();
  });
}
