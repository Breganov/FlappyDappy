class TickSessionUseCase {
public:
  void Execute(const SessionId& session_id, std::chrono::milliseconds delta);
};
