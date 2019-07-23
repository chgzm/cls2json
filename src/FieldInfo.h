#ifndef FIELDINFO_H
#define FIELDINFO_H

#include "CPInfo.h"
#include "AttributeInfo.h"

#include <cstdint>
#include <vector>
#include <memory>

class FieldInfo {
public:
    using ConstantPool = std::vector<std::unique_ptr<CPInfo>>;
    using Attributes   = std::vector<std::unique_ptr<AttributeInfo>>;

    FieldInfo()  = default;
    ~FieldInfo() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getAccessFlags() const noexcept {
        return this->accessFlags_;
    }

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

    inline uint16_t getDescriptorIndex() const noexcept {
        return this->descriptorIndex_;
    }

    inline uint16_t getAttributesCount() const noexcept {
        return this->attributes_.size();
    }

    inline const Attributes& getAttributes() const noexcept {
        return this->attributes_;
    }

    inline const AttributeInfo* getAttributeAt(uint16_t index) const noexcept {
        return this->attributes_[index].get();
    }

    static constexpr uint16_t ACC_PUBLIC     = 0x0001;
    static constexpr uint16_t ACC_PRIVATE    = 0x0002;
    static constexpr uint16_t ACC_PROTECTED  = 0x0004;
    static constexpr uint16_t ACC_STATIC     = 0x0008;
    static constexpr uint16_t ACC_FINAL      = 0x0010;
    static constexpr uint16_t ACC_VOLATILE   = 0x0040;
    static constexpr uint16_t ACC_TRANSIENT  = 0x0080;
    static constexpr uint16_t ACC_SYSTHETIC  = 0x1000;
    static constexpr uint16_t ACC_ENUM       = 0x4000;

private:
    uint16_t   accessFlags_;
    uint16_t   nameIndex_;
    uint16_t   descriptorIndex_;
    Attributes attributes_;
};

#endif
