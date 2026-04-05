// Application должна уметь рассылать сообщения не зная про WebSocket
class ISessionBroadcaster {
public:
  virtual ~ISessionBroadcaster() = default;
  virtual void BroadcastSnapshot(
    const SessionId& session_id,
    const WorldSnapshot& snapshot) = 0;

  virtual void BroadcastMatchFinished(
    const SessionId& session_id,
    const MatchResult& result) = 0;

  virtual void NotifyPlayerJoined(
    const SessionId& session_id,
    const PlayerId& player_id) = 0;
};
