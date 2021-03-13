#include "server.h"
#include "log.h"
#include "misc.h"

void CTCPServer::AcceptConnections()
{
    error_code error;
    ip::tcp::socket socket = m_acceptor->accept(error);

    if (!error)
    {
        CSession::Ptr session = std::make_shared<CSession>(std::move(socket));
        session->StartSocket();
        m_sessions.emplace_back(session);
    }
    else
        LOG_ERROR("Error accepting connection: %s", error.message().c_str());
    
    if (!m_stop)
        AcceptConnections();
}

void CTCPServer::Inititalize(uint16_t port)
{
    m_acceptor = new ip::tcp::acceptor(m_context, 
        ip::tcp::endpoint(ip::tcp::v4(), port));

    LOG_DEBUG("Accepting connections.");
    m_acceptorThread = std::thread([&]() { AcceptConnections(); });
    utils::SetThreadName(&m_acceptorThread, "Acceptor Thread");

    while(!m_stop)
    {
        for (int i = 0; i < m_sessions.size(); i++)
        {
            m_sessions[i]->Update();
            if (m_sessions[i]->IsClosed())
            {
                m_sessions[i].reset();
                m_sessions.erase(std::find(m_sessions.begin(), m_sessions.end(), m_sessions[i]));
                m_sessions.shrink_to_fit();
            }   
        }
    }

    m_acceptorThread.join();
}