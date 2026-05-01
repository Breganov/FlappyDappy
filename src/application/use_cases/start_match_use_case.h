#pragma once

#include "application/use_cases/session_service.h"
class StartMatchUseCase {
public:
  explicit StartMatchUseCase(SessionService &sessions);
  void Execute(const SessionId &session_id);

private:
  SessionService &sessions_;
};

/* 1. Находит сессию
 * 2. Если нет сессии, return
 * 3. Начинает отсчёт
 * 4. Запускает матч
 */
