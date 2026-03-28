class IIdGenerator {
public:
  virtual ~IIdGenerator() = default;
  virtual SessionId NewSessionId() = 0;
  virtual MatchId NewMatchId() = 0;
};
