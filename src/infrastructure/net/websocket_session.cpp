// infrastructure/net/websocket_session.cpp
#include "websocket_session.h"

#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/websocket/error.hpp>
#include <boost/json.hpp>

#include <utility>

WebSocketSession::WebSocketSession(tcp::socket socket,
                                   WebSocketMessageHandler &handler,
                                   ILogger &logger)
    : ws_(std::move(socket)), handler_(handler), logger_(logger) {
  ws_.set_option(
      websocket::stream_base::timeout::suggested(beast::role_type::server));
}

void WebSocketSession::Start() {
  auto self = shared_from_this();

  ws_.async_accept([self](beast::error_code ec) {
    if (ec) {
      self->logger_.Error(std::string("WebSocket accept failed: ") +
                          ec.message());
      return;
    }

    self->logger_.Info("WebSocket session accepted.");
    self->DoRead();
  });

  // DoRead(); // Удалил по рекомендации Opus. Вроде бы нормальная идея.
}

// Отправка
// ==============================================================================

void WebSocketSession::Send(std::string message) {
  auto self = shared_from_this();

  net::post(ws_.get_executor(), [self, msg = std::move(message)]() mutable {
    const bool write_in_progress = !self->outgoing_messages_.empty();
    self->outgoing_messages_.push_back(std::move(msg));

    if (!write_in_progress) {
      self->DoWrite();
    }
  });
}

void WebSocketSession::DoRead() {
  auto self = shared_from_this();

  ws_.async_read(buffer_,
                 [self](beast::error_code ec, std::size_t bytes_transferred) {
                   self->OnRead(ec, bytes_transferred);
                 });
}

// чтение
// ==============================================================================

void WebSocketSession::OnRead(beast::error_code ec,
                              std::size_t bytes_transferred) {
  if (ec == websocket::error::closed) {
    logger_.Info("WebSocket seession closed by client.");
    return;
  }

  if (ec) {
    logger_.Error(std::string("WebSocket read failed: ") + ec.message());
    return;
  }

  const std::string incoming = beast::buffers_to_string(buffer_.data());
  buffer_.consume(buffer_.size());

  logger_.Debug(std::string("Received websocket message: ") + incoming);

  HandleMessage(incoming);

  DoRead();
}

void WebSocketSession::DoWrite() {
  auto self = shared_from_this();

  ws_.async_write(net::buffer(outgoing_messages_.front()),
                  [self](beast::error_code ec, std::size_t bytes_transferred) {
                    self->OnWrite(ec, bytes_transferred);
                  });
}

// Отправка
// ==============================================================================

void WebSocketSession::OnWrite(beast::error_code ec,
                               std::size_t bytes_transferred) {
  if (ec) {
    logger_.Error(std::string("WebSocket write failed: ") + ec.message());
    return;
  }

  outgoing_messages_.pop_front();

  if (!outgoing_messages_.empty()) {
    DoWrite();
  }
}

// Обработка сообщения
// ==============================================================================

void WebSocketSession::HandleMessage(std::string_view json_text) {
  const std::string response =
      handler_.ProcessIncomingMessage(json_text, shared_from_this());
  Send(response);
}
