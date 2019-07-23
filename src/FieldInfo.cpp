#include "FieldInfo.h"
#include "Format.h"
#include "ByteReader.h"

#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

int FieldInfo::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp) noexcept {
    this->accessFlags_     = readUInt16(addr, pos);
    this->nameIndex_       = readUInt16(addr, pos);
    this->descriptorIndex_ = readUInt16(addr, pos);

    const uint16_t attributesCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < attributesCount; ++i) {
        std::unique_ptr<AttributeInfo> attrInfo = std::make_unique<AttributeInfo>();
        if (attrInfo->load(addr, pos, cp) != 0) {
            return -1;
        }
        
        this->attributes_.push_back(std::move(attrInfo)); 
    }

    return 0;
}

std::string FieldInfo::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"access_flags\":%hu,"
        "\"name_index\":%hu,"
        "\"descriptor_index\":%hu,"
        "\"attributes_count\":%hu,",
        this->accessFlags_,
        this->nameIndex_,
        this->descriptorIndex_,
        this->getAttributesCount()
    );

    ss << "\"attributes\":[";
    for (uint16_t i = 0; i < this->getAttributesCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getAttributeAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}
