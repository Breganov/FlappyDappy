// infrastructure/net/websocket_message_serializer.cpp
#include "websocket_message_serializer.h"

#include <boost/json.hpp>
#include <boost/json/array.hpp>

namespace json = boost::json;

std::string WebSocketMessageSerializer::SerializeSnapshot(
    const WorldSnapshot &snapshot) const {
  json::object root;

  root["type"] = "snapshot";
  root["session_id"] = snapshot.session_id;
  root["tick"] = snapshot.tick;
  root["state"] = static_cast<int>(snapshot.state);

  json::array players;
  for (const auto &player : snapshot.players) {
    json::object player_obj;

    player_obj["player_id"] = player.player_id;
    player_obj["x"] = player.x;
    player_obj["y"] = player.y;
    player_obj["velocity_y"] = player.velocity_y;
    player_obj["alive"] = player.alive;
    player_obj["passed_pipes"] = player.passed_pipes;
    player_obj["distance"] = player.distance;

    players.push_back(player_obj);
  }
  root["players"] = players;

  json::array pipes;
  for (const auto &pipe : snapshot.pipes) {
    json::object pipe_obj;
    pipe_obj["x"] = pipe.x;
    pipe_obj["gap_y"] = pipe.gap_y;
    pipe_obj["gap_height"] = pipe.gap_height;

    pipes.push_back(pipe_obj);
  }
  root["pipes"] = pipes;

  return json::serialize(root);
}

std::string WebSocketMessageSerializer::SerializeMatchResult(
    const MatchResult &result) const {
  json::object root;
  root["type"] = "match_result";

  json::array rankings;
  for (const auto &entry : result.rankings) {
    json::object entry_obj;
    entry_obj["player_id"] = entry.player_id.ToString();
    entry_obj["distance"] = entry.distance;
    entry_obj["passed_pipes"] = entry.passed_pipes;

    rankings.push_back(entry_obj);
  }
  root["rankings"] = rankings;

  return json::serialize(root);
}
