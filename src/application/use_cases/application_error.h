// application\use_cases\application_error.h
#pragma once

enum class ApplicationError {
  SessionNotFound,
  PlayerNotFound,
  InvalidSessionState,
  SessionAlreadyFinished
};
