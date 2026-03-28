#include <vector>

#include "../session/player_session_state.h"
#include "pipe.h"

class ScoreService {
public:
  void UpdateScore(PlayerSessionState& player, const std::vector<Pipe>& pipes) const;
};
