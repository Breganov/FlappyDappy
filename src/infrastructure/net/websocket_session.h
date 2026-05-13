#pragma once

#include "infrastructure/logging/logger.h"
#include "infrastructure/net/websocket_message_handler.h"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/stream.hpp>

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
  // WebSocketSession(tcp::socket socket, WebSocketMessageParser &parser,
  //                  WebSocketRouter &router, ILogger &logger);

  WebSocketSession(tcp::socket socket, WebSocketMessageHandler &handler,
                   ILogger &logger);
  void Start();
  void Send(std::string message);

private:
  void DoRead();
  void OnRead(beast::error_code ec, std::size_t bytes_transferred);

  void DoWrite();
  void OnWrite(beast::error_code ec, std::size_t bytes_transferred);

  void HandleMessage(std::string_view json);

private:
  websocket::stream<tcp::socket> ws_;
  beast::flat_buffer buffer_;
  std::deque<std::string> outgoing_messages_;

  // WebSocketMessageParser &parser_;
  // WebSocketRouter &router_;
  WebSocketMessageHandler &handler_;
  ILogger &logger_;
};
