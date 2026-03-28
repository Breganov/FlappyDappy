#include <cstddef>
#include <vector>

class ILeaderboardRepository {
public:
  virtual ~ILeaderboardRepository() = default;
  virtual std::vector<LeaderboardEntry> GetTop(std::size_t limit) = 0;
};
