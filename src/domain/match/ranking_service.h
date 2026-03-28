#include <vector>

#include "../session/player_session_state.h"
#include "../game/pipe.h"

class RankingService {
public:
  void UpdateScore(PlayerSessionState& player, const std::vector<Pipe>& pipes) const;
};
