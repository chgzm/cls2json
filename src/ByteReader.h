#ifndef BYTEREADER_H
#define BYTEREADER_H

#include <cstdint>

uint8_t  readUInt8(const uint8_t* addr, std::size_t& pos)  noexcept;
uint16_t readUInt16(const uint8_t* addr, std::size_t& pos) noexcept;
uint32_t readUInt32(const uint8_t* addr, std::size_t& pos) noexcept;

#endif
