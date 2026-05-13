// infrastructure/net/websocket_server.h
#pragma once

#include "infrastructure/logging/logger.h"
#include "infrastructure/net/websocket_message_handler.h"

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/system/detail/error_code.hpp>

namespace net = boost::asio;
using tcp = net::ip::tcp;

class WebSocketServer {
public:
  WebSocketServer(net::io_context &io_context, const tcp::endpoint &endpoint,
                  WebSocketMessageHandler &handler, ILogger &logger);

  void Start();

private:
  void DoAccept();
  void OnAccept(boost::system::error_code ec, tcp::socket socket);

private:
  net::io_context &io_context_;
  tcp::acceptor acceptor_;

  WebSocketMessageHandler &handler_;
  // WebSocketMessageParser &parser_;
  // WebSocketRouter &router_;
  ILogger &logger_;
};
