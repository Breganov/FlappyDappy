#include "../game/bird_state.h" // для BirdState

class PlayerSessionState {
public:
  PlayerSessionState(PlayerId player_id, BirdState bird);

  const PlayerId& GetPlayerId() const;
  const BirdState& GetBirdId() const;
  BirdState& GetBird();

  bool IsReady() const;
  void MarkReady();

  bool IsConnected() const;
  void MarkDisconnected();
  void MarkConnected();
  
private:
  PlayerId player_id_;
  BirdState bird_;
  bool ready_ = false;
  bool connecte_ = true;
}
