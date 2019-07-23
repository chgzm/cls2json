#include "ByteReader.h"
#include <byteswap.h>

uint8_t readUInt8(const uint8_t* addr, std::size_t& pos) noexcept {
    const uint8_t val = addr[pos]; 
    ++pos;
    return val;
}

uint16_t readUInt16(const uint8_t* addr, std::size_t& pos) noexcept {
    const uint16_t val = __bswap_16(*(uint16_t*)(&(addr[pos])));
    pos += 2;
    return val;
}

uint32_t readUInt32(const uint8_t* addr, std::size_t& pos) noexcept {
    const uint32_t val = __bswap_32(*(uint32_t*)(&(addr[pos])));
    pos += 4;
    return val;
}
