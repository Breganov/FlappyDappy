#pragma once

#include "infrastructure/logging/logger.h"
#include "infrastructure/net/websocket_message_parser.h"
#include "infrastructure/net/websocket_router.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/websocket.hpp>

#include <deque>
#include <memory>
#include <string>
#include <string_view>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
  WebSocketSession(tcp::socket socket, WebSocketMessageParser &pasrser,
                   WebSocketRouter &router, ILogger logger);
  void Start();
  void Send(std::string message);

private:
  void DoRead();
  void OnRead();
  void HandleMessage(std::string_view json);
};
