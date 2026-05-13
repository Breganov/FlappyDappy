// infrastructure/net/websocket_server.cpp
#include "infrastructure/net/websocket_server.h"
#include "infrastructure/net/websocket_session.h"
#include <boost/system/detail/error_code.hpp>

#include <memory>
#include <string_view>

WebSocketServer::WebSocketServer(net::io_context &io_context,
                                 const tcp::endpoint &endpoint,
                                 WebSocketMessageHandler &handler,
                                 ILogger &logger)
    : io_context_(io_context), acceptor_(io_context), handler_(handler),
      logger_(logger) {
  boost::system::error_code ec;

  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    logger_.Error(std::string("Failed to open acceptor: ") + ec.message());
    return;
  }

  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    logger_.Error(std::string("Failed to set reuse_address: ") + ec.message());
    return;
  }

  acceptor_.bind(endpoint, ec);
  if (ec) {
    logger_.Error(std::string("Failed to bind acceptor: ") + ec.message());
    return;
  }

  acceptor_.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
    logger_.Error(std::string("Failed to listen: ") + ec.message());
    return;
  }
}

void WebSocketServer::Start() {
  logger_.Info("WebSocket server starting.");
  DoAccept();
}

void WebSocketServer::DoAccept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        OnAccept(ec, std::move(socket));
      });
}

void WebSocketServer::OnAccept(boost::system::error_code ec,
                               tcp::socket socket) {
  if (ec) {
    logger_.Error(std::string("Accept failed: ") + ec.message());
  } else {
    logger_.Info("Accepted incoming TCP connection.");

    std::make_shared<WebSocketSession>(std::move(socket), handler_, logger_)
        ->Start();
  }

  DoAccept();
}
