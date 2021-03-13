#include "session.h"
#include "log.h"
#include <string_view>

CSession::CSession(ip::tcp::socket&& socket)
    : m_socket(std::move(socket))
{
    m_address = m_socket.remote_endpoint().address().to_string();
    m_port = m_socket.remote_endpoint().port();
    m_socket.non_blocking(true);
}

void CSession::StartSocket()
{
    m_closed = false;

    LOG_INFO("Started connection: %s:%i", GetAddress(), GetPort());

    error_code error;
    uint32_t helloOpcode[] = {SMSG_HELLO, 0};
    m_socket.write_some(buffer(&helloOpcode, sizeof(helloOpcode)), error);
    
    if (error)
    {
        LOG_ERROR("Error writing packet, message: (%s), closing socket.", error.message());
        CloseSocket();
    }
}

void CSession::Update()
{
    m_readAmount = 0;
    error_code error;
    m_readAmount = m_socket.read_some(buffer(
        m_buffer.GetData(), 
        m_buffer.GetFreeSpace()), error);

    if (error && error != boost::asio::error::would_block)
    {
        LOG_ERROR("Error handling session data, message: (%s), closing socket.", error.message());
        CloseSocket();
        return;
    }

    if (m_readAmount > 0)
    {
        LOG_DEBUG("Got packet with opcode: (%i)", m_buffer.Opcode());
        m_buffer.HandleData(this, m_readAmount);
    }
}

void CSession::CloseSocket()
{
    LOG_INFO("Closed connection: %s:%i", GetAddress(),GetPort());

    if (m_socket.is_open())
    {
        // Can do something here if we close the session, instead of just dropping.
        // Like notifying other sessions.
        m_socket.shutdown(socket_base::shutdown_both);
        m_socket.close();
    }

    m_closed = true;
}
