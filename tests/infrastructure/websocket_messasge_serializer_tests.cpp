#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/game_config.h"
#include "application/use_cases/session_service.h"

#include "catch2/catch_test_macros.hpp"
#include "domain/match/match_result.h"
#include "domain/session/session_id.h"
#include "infrastructure/net/websocket_message_serializer.h"
#include <boost/json/parse.hpp>

namespace json = boost::json;

class SerializerTestFixture {
protected:
  SessionService sessions;
  SimpleIdGenerator ids;
  GameConfig game_config{PhysicsConfig{}};
  WebSocketMessageSerializer serializer;
  const SessionId session_id;

public:
  SerializerTestFixture() : session_id(CreateSession()) {}

protected:
  std::string serialize_snapshot() {
    auto session_opt = sessions.FindSession(session_id);
    REQUIRE(session_opt.has_value());
    return serializer.SerializeSnapshot(session_opt->get().BuildSnapshot());
  }

  std::string serialize_match_result(const MatchResult &result) {
    return serializer.SerializeMatchResult(result);
  }

private:
  SessionId CreateSession() {
    CreateSessionUseCase create(ids, sessions, game_config);
    return create.Execute();
  }
};

TEST_CASE_METHOD(
    SerializerTestFixture,
    "WebSocketMessageSerializer serializes snapshot and match result",
    "[serializer]") {
  const std::string json_str = serialize_snapshot();

  REQUIRE_FALSE(json_str.empty());

  const auto value = json::parse(json_str);
  const auto &obj = value.as_object();

  REQUIRE(value.is_object());

  CHECK(obj.contains("type"));
  CHECK(std::string(obj.at("type").as_string()) == "snapshot");

  CHECK(obj.contains("session_id"));
  CHECK(obj.at("session_id").as_string() == session_id.ToString());

  CHECK(obj.contains("tick"));
  CHECK(obj.at("tick").is_int64());
  CHECK(obj.at("tick").as_int64() == 0);

  CHECK(obj.contains("players"));
  CHECK(obj.at("players").is_array());
  CHECK(obj.at("players").as_array().empty());
  // CHECK(obj.at("players").as_array().size());

  CHECK(obj.contains("pipes"));
  CHECK(obj.at("pipes").is_array());
  CHECK_FALSE(obj.at("pipes").as_array().empty());

  CHECK(obj.contains("state"));
  CHECK(obj.at("state").is_int64());
}

TEST_CASE_METHOD(
    SerializerTestFixture,
    "WebSocketMessageSerializer::SerializeMatchResult produces valid JSON",
    "[serializer]") {
  MatchResult result;
  result.rankings.push_back(MatchResult::Entry{PlayerId("player-1"), 120.5, 3});
  result.rankings.push_back(MatchResult::Entry{PlayerId("player-2"), 95.0, 2});

  const std::string json_str = serialize_match_result(result);

  REQUIRE_FALSE(json_str.empty());

  const auto value = json::parse(json_str);
  REQUIRE(value.is_object());

  const auto &obj = value.as_object();

  CHECK(obj.contains("type"));
  CHECK(std::string(obj.at("type").as_string()) == "match_result");

  CHECK(obj.contains("rankings"));
  CHECK(obj.at("rankings").is_array());

  const auto &rankings = obj.at("rankings").as_array();
  REQUIRE(rankings.size() == 2);
}

TEST_CASE_METHOD(
    SerializerTestFixture,
    "WebSocketMessageSerializer::SerializeMatchResult contains ranking fields",
    "[serializer]") {
  MatchResult result;
  result.rankings.push_back(MatchResult::Entry{PlayerId("player-1"), 120.5, 3});

  const std::string json_str = serialize_match_result(result);

  const auto value = json::parse(json_str);
  REQUIRE(value.is_object());

  const auto &obj = value.as_object();
  REQUIRE(obj.contains("rankings"));
  REQUIRE(obj.at("rankings").is_array());

  const auto &rankings = obj.at("rankings").as_array();
  REQUIRE(rankings.size() == 1);
  REQUIRE(rankings[0].is_object());

  const auto &first = rankings[0].as_object();

  CHECK(first.contains("player_id"));
  CHECK(std::string(first.at("player_id").as_string()) == "player-1");

  CHECK(first.contains("distance"));
  CHECK(first.at("distance").is_double());

  CHECK(first.contains("passed_pipes"));
  CHECK(first.at("passed_pipes").is_int64());

  CHECK(first.at("passed_pipes").as_int64() == 3);
}
