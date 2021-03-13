#pragma once

#include "session.h"
#include <boost/asio.hpp>
#include <optional>

using namespace boost::asio;
using namespace boost::system;

class CTCPServer
{
    CTCPServer()
    {  }
    ~CTCPServer() {}

    void AcceptConnections();

public:
    static CTCPServer& instance() 
    {
        static CTCPServer instance;
        return instance;
    }

    void Inititalize(uint16_t port);

private:
    io_context m_context;
    ip::tcp::acceptor* m_acceptor;
    std::vector<CSession::Ptr> m_sessions;
    std::thread m_acceptorThread;

    std::atomic_bool m_stop = false;
};

#define sTCPServer CTCPServer::instance()