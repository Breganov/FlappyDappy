// для того, чтобы придерживаться ACID
class IUnitOfWork {
public:
  virtual ~IUnitOfWork() = default;
  virtual IPlayerRepository& Players = 0;
  virtual IMatchRepository& Matches() = 0;
  virtual ILeaderboardRepository& Leaderboard() = 0;
  virtual void Commit() = 0;
  virtual void Rollback() = 0;
};
