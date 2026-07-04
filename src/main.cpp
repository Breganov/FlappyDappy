// main.cpp
#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/finish_match_use_case.h"
#include "application/use_cases/game_loop_service.h"
#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/session_service.h"
#include "application/use_cases/submit_input_use_case.h"
#include "application/use_cases/tick_session_use_case.h"

#include "domain/session/input_command.h"
#include "domain/session/session_id.h"

#include "infrastructure/logging/console_logger.h"
// #include "infrastructure/logging/console_session_broadcaster.h"
#include "infrastructure/net/connection_registry.h"
#include "infrastructure/net/game_loop_timer.h"
#include "infrastructure/net/websocket_message_handler.h"
#include "infrastructure/net/websocket_message_parser.h"
#include "infrastructure/net/websocket_message_serializer.h"
#include "infrastructure/net/websocket_router.h"
#include "infrastructure/net/websocket_server.h"
#include "infrastructure/net/websocket_session_broadcaster.h"

// #include "application/use_cases/start_match_use_case.h"
// #include "domain/player/player_id.h"

// #include <chrono>
#include <chrono>
#include <string>

#include <boost/asio.hpp>

int main() {
  ConsoleLogger logger;
  logger.Info("Starting flappy server.");

  // --- Composition root: build the whole object graph in one place. ---
  // Order matters: dependencies are constructed before their users,
  // and everything lives on the stack for the lifetime of main().

  SimpleIdGenerator ids;
  SessionService sessions;

  ConnectionRegistry registry;
  WebSocketMessageSerializer serializer;
  WebSocketSessionBroadcaster broadcaster(registry, serializer);
  // ConsoleSessionBroadcaster broadcaster(logger);
  GameConfig game_config{PhysicsConfig{}};

  CreateSessionUseCase create_session(ids, sessions, game_config);
  JoinSessionUseCase join_session(sessions, broadcaster);
  SubmitInputUseCase submit_input(sessions);
  TickSessionUseCase tick_session(sessions, broadcaster);
  FinishMatchUseCase finish_match(sessions);
  GameLoopService game_loop(sessions, tick_session, finish_match);

  // Message pipeline (infrestructure layer):
  // raw JSON -> parser -> router -> use cases.
  WebSocketMessageParser parser;
  WebSocketRouter router(join_session, submit_input);

  WebSocketMessageHandler handler(parser, router, registry);

  // Temporary: one session created at startup so clients can join it.
  // Phase -- replace this with a "create" network message.
  const SessionId bootstrap_id = create_session.Execute();
  logger.Info("Bootstrap session created: " + bootstrap_id.ToString());

  // --- The event loop. Everything async runs on this single thread.
  boost::asio::io_context io_context;

  const auto endpoint = tcp::endpoint(tcp::v4(), 8080);
  WebSocketServer server(io_context, endpoint, handler, logger);
  GameLoopTimer game_loop_timer(io_context, game_loop,
                                std::chrono::milliseconds(16));
  game_loop_timer.Start();
  server.Start();
  logger.Info("Listening on ws://localhost:8080");

  io_context.run(); // blocks; all callbacks fire on this thread
  return 0;
}
