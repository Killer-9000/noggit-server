#pragma once

#include "packet.h"
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::system;

class CSession : public std::enable_shared_from_this<CSession>
{
    ip::tcp::socket m_socket;
    CPacketData m_buffer;
    size_t m_readAmount;

    std::string m_address;
    uint16_t m_port;

    bool m_closed = false;

public:
    CSession(ip::tcp::socket&& socket);

    void StartSocket();
    void Update();
    void CloseSocket();

    std::string GetAddress() { return m_address; }
    uint16_t GetPort() { return m_port; }
    ip::tcp::socket& GetSocket() { return m_socket; }
    bool IsClosed() { return m_closed; }

    typedef std::shared_ptr<CSession> Ptr;
};