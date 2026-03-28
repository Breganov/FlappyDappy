class IPlayerRepository {
public:
  virtual ~IPlayerRepository() = default;
  virtual std::optional<Player> FindById(const PlayerId& id) = 0;
  virtual void Save(const Player& player) = 0;
};
