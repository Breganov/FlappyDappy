#pragma once

#include "application/use_cases/session_service.h"
#include "domain/session/input_command.h"

class SubmitInputUseCase {
public:
  explicit SubmitInputUseCase(SessionService &sessions);

  void Execute(const SessionId &session_id, const InputCommand &command);

private:
  SessionService &sessions_;
};

/*
 * находит сессию
 * проверяет, что она существует
 * передаёт `InputCommand` в `GameSession::Enqueue(...)`
 */
