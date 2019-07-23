#ifndef CLASSFILE_H
#define CLASSFILE_H

#include "CPInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"
#include "AttributeInfo.h"

#include <cstdint>
#include <vector>
#include <memory>

class ClassFile {
public:
    using ConstantPool = std::vector<std::unique_ptr<CPInfo>>;
    using Fields       = std::vector<std::unique_ptr<FieldInfo>>;
    using Methods      = std::vector<std::unique_ptr<MethodInfo>>;
    using Attributes   = std::vector<std::unique_ptr<AttributeInfo>>;

    ClassFile()  = default;
    ~ClassFile() = default;

    int load(const std::string& filePath) noexcept;

    inline uint32_t getMagic() const noexcept {
        return this->magic_;
    }

    inline uint16_t getMinorVersion() const noexcept {
        return this->minorVersion_;
    }

    inline uint16_t getMajorVersion() const noexcept {
        return this->majorVersion_;
    }

    inline uint16_t getConstantPoolCount() const noexcept {
        return this->constantPoolCount_;
    }

    inline const ConstantPool& getConstantPool() const noexcept {
        return this->constantPool_;
    }

    inline const CPInfo* getCPAt(uint16_t index) const noexcept {
        return this->constantPool_[index].get();
    }

    inline uint16_t getAccessFlags() const noexcept {
        return this->accessFlags_;
    }

    inline uint16_t getThisClass() const noexcept {
        return this->thisClass_; }

    inline uint16_t getSuperClass() const noexcept {
        return this->superClass_;
    }

    inline uint16_t getInterfacesCount() const noexcept {
        return this->interfaces_.size();
    }

    inline const std::vector<uint16_t>& getInterfaces() const noexcept {
        return this->interfaces_;
    }

    inline uint16_t getInterfaceAt(uint16_t index) const noexcept {
        return this->interfaces_[index];
    }

    inline uint16_t getFieldsCount() const noexcept {
        return this->fields_.size();
    }

    inline const Fields& getFields() const noexcept {
        return this->fields_;
    }

    inline const FieldInfo* getFieldAt(uint16_t index) const noexcept {
        return this->fields_[index].get();
    }

    inline uint16_t getMethodsCount() const noexcept {
        return this->methods_.size();
    }

    inline const Methods& getMethods() const noexcept {
        return this->methods_;
    }

    inline const MethodInfo* getMethodAt(uint16_t index) const noexcept {
        return this->methods_[index].get();
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

    std::string toString() const noexcept;

    static constexpr uint32_t MAGIC            = 0xcafebabe;

    static constexpr uint16_t ACC_PUBLIC       = 0x0001;
    static constexpr uint16_t ACC_FINAL        = 0x0010;
    static constexpr uint16_t ACC_SUPER        = 0x0020;
    static constexpr uint16_t ACC_INTERFACE    = 0x0200;
    static constexpr uint16_t ACC_ABSTRACT     = 0x0400;
    static constexpr uint16_t ACC_SYSTHETIC    = 0x1000;
    static constexpr uint16_t ACC_ANNOTATION   = 0x2000;
    static constexpr uint16_t ACC_ENUM         = 0x4000;
    static constexpr uint16_t ACC_MODULE       = 0x8000;

private:
    int loadConstantPool(const uint8_t* addr, std::size_t& pos) noexcept;
    int loadFields(const uint8_t* addr, std::size_t& pos) noexcept;
    int loadMethods(const uint8_t* addr, std::size_t& pos) noexcept;
    int loadAttributes(const uint8_t* addr, std::size_t& pos) noexcept;

    std::string getAccessFlagsStr() const noexcept;

    uint32_t              magic_;
    uint16_t              minorVersion_;
    uint16_t              majorVersion_;
    uint16_t              constantPoolCount_;
    ConstantPool          constantPool_;
    uint16_t              accessFlags_;
    uint16_t              thisClass_;
    uint16_t              superClass_;
    std::vector<uint16_t> interfaces_;
    Fields                fields_;
    Methods               methods_;
    Attributes            attributes_;
};

#endif
