// infrastructure\net\websocket_router.h
#pragma once

#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/submit_input_use_case.h"
#include "infrastructure/net/parsed_message.h"
#include "infrastructure/net/router_result.h"

class WebSocketRouter {
public:
  WebSocketRouter(JoinSessionUseCase &join, SubmitInputUseCase &submit);
  RouteResult Route(const ParsedMessage &message) const;

private:
  JoinSessionUseCase &join_;
  SubmitInputUseCase &submit_;
};
