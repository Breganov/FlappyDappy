#include "submit_input_use_case.h"

SubmitInputUseCase::SubmitInputUseCase(SessionService &sessions)
    : sessions_(sessions) {}

void SubmitInputUseCase::Execute(const SessionId &session_id,
                                 const InputCommand &command) {
  auto session = sessions_.FindSession(session_id);
  if (!session) {
    return;
  }
  session->get().EnqueueInput(command);
}
