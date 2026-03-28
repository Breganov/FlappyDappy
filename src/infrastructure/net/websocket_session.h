#include <string>
class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
  void Start();
  void Send(std::string message);

private:
  void DoRead();
  void OnRead(...);
  void HandleMessage(std::string_view json);
}
