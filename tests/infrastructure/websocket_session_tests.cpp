// tests/infrastructure/websocket_session_tests.cpp

#include "application/ports/fake_session_broadcaster.h"
#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/game_config.h"
#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/session_service.h"
#include "domain/session/session_id.h"
#include "infrastructure/logging/console_logger.h"
#include "infrastructure/net/websocket_message_parser.h"
#include "infrastructure/net/websocket_router.h"
#include "infrastructure/net/websocket_session.h"

#include "catch2/catch_test_macros.hpp"
#include <boost/json/parse.hpp>

namespace json = boost::json;

class SessionMessageProcessingFixture {
  /* Arrange, Act, Assert
   * Что подготовили?
   * Что вызывали?
   * Что изменилось или что получили?
   * */
protected:
  FakeSessionBroadcaster broadcaster_;
  SessionService sessions_;
  SimpleIdGenerator ids_;
  GameConfig game_config_{PhysicsConfig{}};

  CreateSessionUseCase create_{ids_, sessions_, game_config_};
  JoinSessionUseCase join_{sessions_, broadcaster_};
  SubmitInputUseCase submit_{sessions_};

  WebSocketMessageParser parser_;
  WebSocketRouter router_{join_, submit_};

  SessionId CreateSession() { return create_.Execute(); }
};
