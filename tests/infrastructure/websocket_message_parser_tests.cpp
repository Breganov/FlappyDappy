// infrastructure/websocket_message_parser_tests.cpp
#include "application/ports/fake_session_broadcaster.h"
#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/game_config.h"
#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/session_service.h"
#include "application/use_cases/submit_input_use_case.h"
#include "infrastructure/net/websocket_message_handler.h"
#include "infrastructure/net/websocket_message_parser.h"
#include "infrastructure/net/websocket_router.h"

#include <boost/json/detail/handler.hpp>
#include <boost/json/parser.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

namespace json = boost::json;

class MessageHandlerFixture {
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
  WebSocketMessageHandler handler_{parser_, router_};

  SessionId CreateSession() { return create_.Execute(); }
};

TEST_CASE_METHOD(
    MessageHandlerFixture,
    "ProccessIncomingMessage returns error response for invalid JSON",
    "[message_handler]") {
  const auto response = handler_.ProcessIncomingMessage("{invalid json");
  const auto value = json::parse(response);
  REQUIRE(value.is_object());

  const auto &obj = value.as_object();
  REQUIRE(std::string(obj.at("type").as_string()) == "error");
  REQUIRE(obj.contains("message"));
}

TEST_CASE_METHOD(
    MessageHandlerFixture,
    "ProccessIncomingMessage returns error response for unknown message type",
    "[message_handler]") {
  const auto response = handler_.ProcessIncomingMessage(
      R"({"type":"dance","sessions_id":"session-1","player_id":"player-1"})");

  const auto value = json::parse(response);
  REQUIRE(value.is_object());

  const auto &obj = value.as_object();
  REQUIRE(std::string(obj.at("type").as_string()) == "error");
  REQUIRE(obj.contains("message"));
}

TEST_CASE_METHOD(MessageHandlerFixture,
                 "ProcessIncomingMessage returns ack response for valid join",
                 "[message_handler]") {
  const auto session_id = CreateSession();

  const std::string request = std::string(R"({"type":"join","session_id":")") +
                              session_id.ToString() +
                              R"(","player_id":"player-1"})";

  const auto response = handler_.ProcessIncomingMessage(request);

  const auto value = json::parse(response);
  REQUIRE(value.is_object());

  const auto &obj = value.as_object();
  REQUIRE(std::string(obj.at("type").as_string()) == "ack");
  REQUIRE(std::string(obj.at("message").as_string()) == "handled");

  const auto session = sessions_.FindSession(session_id);
  REQUIRE(session.has_value());
  REQUIRE(session->get().GetPlayers().size() == 1);
}

TEST_CASE_METHOD(MessageHandlerFixture,
                 "ProcessIncomingMessage returns ack response for valid jump",
                 "[message_handler]") {
  const auto session_id = CreateSession();

  const std::string join_request =
      std::string(R"({"type":"join","session_id":")") + session_id.ToString() +
      R"(","player_id":"player-1"})";

  handler_.ProcessIncomingMessage(join_request);

  const std::string jump_request =
      std::string(R"({"type":"jump","session_id":")") + session_id.ToString() +
      R"(","player_id":"player-1"})";

  const auto response = handler_.ProcessIncomingMessage(jump_request);

  const auto value = json::parse(response);
  REQUIRE(value.is_object());

  const auto &obj = value.as_object();
  REQUIRE(std::string(obj.at("type").as_string()) == "ack");
  REQUIRE(std::string(obj.at("message").as_string()) == "handled");
}
