#pragma once

#include <boost/asio.hpp>

using namespace boost::asio;

// CMSG : From Client
// SMSG : From Server
enum EPACKET_OPCODE : uint32_t
{
    MSG_ERROR = 0,

    /// Ping/Pong
    /// -----------------
    // Simple hello, good enough for now to test the client isn't something else.
    SMSG_HELLO,
    CMSG_HELLO,

    /// both views Data
    /// -----------------
    // Get which maps are locked.
    // This can be broadcasted if you continue to select the map.
    CMSG_GET_MAP_LOCKS,
    SMSG_SEND_MAP_LOCKS,
    // You can get this as 2D for main_window map, 
    // still contains 3 vectors, just need to do some work. 
    // Its also sent between larger intervals.
    CMSG_GET_USER_POSITIONS,
    SMSG_SEND_USER_POSITIONS,

    /// main_window Data
    /// -----------------

    // Request to open map location, can return a failure, 
    // if failure, and still get packets for in-map, will ignore, and send close.
    // if success then we lock the chunks.
    CMSG_OPEN_MAP,
    SMSG_OPEN_MAP_RESULT,

    /// map_view Data
    /// -----------------
    // Assume afterwards they aren't in map, and so ignore in-map packets.
    // we unlock any locked chunks at this point.
    CMSG_CLOSE_MAP,
    SMSG_CLOSE_MAP,
};

class CSession;

class CPacketData
{
public:
    CPacketData() 
    {
        dataPos = 0;
        data.resize(0x20);
    }
    ~CPacketData() { }

    // Will return the opcode at the start of the data,
    // if there isn't enough data, will return MSG_NOT_READY
    const EPACKET_OPCODE Opcode();

    // Handle the data if its ready.
    void HandleData(CSession* session, size_t& readAmount);

    uint8_t* GetData() { return data.data() + dataPos; }
    size_t GetFreeSpace() { return data.size() - dataPos; }

private:
    // Shift everything left by n, and replace last n with 0
    void ShiftDataLeftByN(size_t n);

    std::vector<uint8_t> data;
    size_t dataPos;
};
