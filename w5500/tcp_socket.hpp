#pragma once
#include <cassert>
#include <cstdint>
#include <cstdlib>

#include "debug_print.hpp"
#include "w5500/w5500_defs.h"

template <typename interface_t>
class tcp_socket {
  public:
    using callback_t = void (&)(tcp_socket<interface_t>& socket, void* arg);
    struct handler_t {
        callback_t on_closed;
        callback_t on_init;
        callback_t on_listen;
        callback_t on_established;
        callback_t on_close_wait;
        void* arg;
    };

    constexpr inline tcp_socket(interface_t& d, uint8_t n, const handler_t& h);
    uint16_t send(const uint8_t* buf, size_t len) const;
    uint16_t recv(uint8_t* buf, size_t len) const;
    void open(uint16_t port) const;
    void listen() const;
    void disconn() const;
    uint16_t rxlen() const;

    void on_closed();
    void on_init();
    void on_listen();
    void on_established();
    void on_close_wait();

  private:
    interface_t& interface;
    const uint8_t sockn;
    const handler_t& handlers;
};

template <typename interface_t>
constexpr tcp_socket<interface_t>::tcp_socket(interface_t& d, uint8_t n, const handler_t& h)
    : interface(d), sockn(n), handlers(h)
{
    interface.socket_bind(this, sockn);
}

template <typename interface_t>
uint16_t tcp_socket<interface_t>::send(const uint8_t* buf, size_t len) const
{
    return interface.socket_send(sockn, buf, len);
}

template <typename interface_t>
uint16_t tcp_socket<interface_t>::recv(uint8_t* buf, size_t len) const
{
    return interface.socket_recv(sockn, buf, len);
}

template <typename interface_t>
void tcp_socket<interface_t>::open(uint16_t port) const
{
    interface.socket_open_tcp(sockn, port);
}

template <typename interface_t>
void tcp_socket<interface_t>::listen() const
{
    interface.socket_listen(sockn);
}

template <typename interface_t>
void tcp_socket<interface_t>::disconn() const
{
    interface.socket_discon(sockn);
}

template <typename interface_t>
uint16_t tcp_socket<interface_t>::rxlen() const
{
    return interface.socket_rxlen(sockn);
}

template <typename interface_t>
void tcp_socket<interface_t>::on_closed()
{
    handlers.on_closed(*this, handlers.arg);
}

template <typename interface_t>
void tcp_socket<interface_t>::on_init()
{
    handlers.on_init(*this, handlers.arg);
}

template <typename interface_t>
void tcp_socket<interface_t>::on_listen()
{
    handlers.on_listen(*this, handlers.arg);
}

template <typename interface_t>
void tcp_socket<interface_t>::on_established()
{
    handlers.on_established(*this, handlers.arg);
}

template <typename interface_t>
void tcp_socket<interface_t>::on_close_wait()
{
    handlers.on_close_wait(*this, handlers.arg);
}
