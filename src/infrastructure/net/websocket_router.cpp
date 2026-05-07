// infrastructure\net\websocket_router.cpp
#include "websocket_router.h"
#include "domain/player/player_id.h"
#include "domain/session/input_command.h"
#include "domain/session/session_id.h"
#include "infrastructure/net/message_type.h"
#include "infrastructure/net/router_result.h"

WebSocketRouter::WebSocketRouter(JoinSessionUseCase &join,
                                 SubmitInputUseCase &submit)

    : join_(join), submit_(submit) {}

RouteResult WebSocketRouter::Route(const ParsedMessage &message) const {
  if (!message.is_valid) {
    return RouteResult{false, message.error};
  }

  const SessionId session_id(message.session_id);
  const PlayerId player_id(message.player_id);

  switch (message.type) {
  case MessageType::Join:
    join_.Execute(session_id, player_id);
    return RouteResult(true, "");
  case MessageType::Jump:
    submit_.Execute(session_id, InputCommand{player_id, InputType::Jump});
    return RouteResult(true, "");
  case MessageType::Unknown:
  default:
    return RouteResult(true, "");
  }
}
