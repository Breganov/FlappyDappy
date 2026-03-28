class IMatchRepository {
public:
  virtual ~IMatchRepository() = default;
  virtual void SaveMatchResult(const MatchResult& result) = 0;
  virtual std::optional<MatchResult> FindById(const MatchId& id) = 0;
};
