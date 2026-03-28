class FinishMatchUseCases {
public:
  FinishMatchUseCases(
    IUnitOfWorkFactory& uow_factory,
    ISessionBroadcaster& broadcaster,
    SessionService& sessions);

  void Execute(const SessionId& session_id);
};
