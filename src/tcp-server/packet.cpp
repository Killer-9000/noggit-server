#include "packet.h"
#include "session.h"
#include "log.h"

// TODO: Change this to handle ingoing/outgoing data, and another CPacket struct,
// which handles the actual data, can overload it with specific packet structures.

const EPACKET_OPCODE CPacketData::Opcode()
{
    if (data.size() < sizeof(EPACKET_OPCODE))
    {
        LOG_ERROR("For some reason the message buffer is soo small, we can't even get a simple opcode. What idiot messed up... oh wait me.");
        return static_cast<const EPACKET_OPCODE>(MSG_ERROR);
    }
    
    return static_cast<const EPACKET_OPCODE>(*data.data()); 
}

void CPacketData::HandleData(CSession* session, size_t& readAmount)
{
    dataPos += readAmount;
    
    if (dataPos < 4)
        return;

    switch (Opcode())
    {
    case MSG_ERROR:
        LOG_ERROR("Error handling packet, check that the correct data is being sent.");
        ShiftDataLeftByN(sizeof(EPACKET_OPCODE));
        break;

    case CMSG_HELLO:
        LOG_INFO("Session (%s:%i) has connected.", session->GetAddress(), session->GetPort());
        ShiftDataLeftByN(sizeof(EPACKET_OPCODE));
        break;
    
    default:
        LOG_ERROR("Error handling packet, unknown opcode.");
        ShiftDataLeftByN(sizeof(EPACKET_OPCODE));
        break;
    }
}

void CPacketData::ShiftDataLeftByN(size_t n)
{
    if (n > data.size())
    {
        LOG_ERROR("Tried to shift data over, further than the data.");
        return;
    }

    memcpy(&data[0], &data[n], data.size());
    memset(&data[data.size() - n], 0x0, n);

    dataPos -= n;
}
