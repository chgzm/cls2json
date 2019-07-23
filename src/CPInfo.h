#ifndef CPINFO_H
#define CPINFO_H

#include <cstdint>
#include <cstring>
#include <vector>
#include <memory>

class CPInfoImpl;

class CPInfo {
public:
    CPInfo()  = default;
    ~CPInfo() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept;

    inline uint8_t getTag() const noexcept {
        return this->tag_;
    }

    inline const CPInfoImpl* getInfo() const noexcept {
        return this->info_.get();
    }

    std::string toString() const noexcept;

    static constexpr uint8_t CONSTANT_Class              =  7;
    static constexpr uint8_t CONSTANT_Fieldref           =  9;
    static constexpr uint8_t CONSTANT_Methodref          = 10;
    static constexpr uint8_t CONSTANT_InterfaceMethodref = 11;
    static constexpr uint8_t CONSTANT_String             =  8;
    static constexpr uint8_t CONSTANT_Integer            =  3;
    static constexpr uint8_t CONSTANT_Float              =  4;
    static constexpr uint8_t CONSTANT_Long               =  5;
    static constexpr uint8_t CONSTANT_Double             =  6;
    static constexpr uint8_t CONSTANT_NameAndType        = 12;
    static constexpr uint8_t CONSTANT_Utf8               =  1;
    static constexpr uint8_t CONSTANT_MethodHandle       = 15;
    static constexpr uint8_t CONSTANT_MethodType         = 16;
    static constexpr uint8_t CONSTANT_Dynamic            = 17;
    static constexpr uint8_t CONSTANT_InvokeDynamic      = 18;
    static constexpr uint8_t CONSTANT_Module             = 19;
    static constexpr uint8_t CONSTANT_Package            = 20;

private:
    const char* decodeTag(uint8_t tag) const noexcept; 

    uint8_t                     tag_;
    std::unique_ptr<CPInfoImpl> info_;
};

class CPInfoImpl {
public:
    virtual ~CPInfoImpl() = default;
    virtual void load(const uint8_t* addr, std::size_t& pos) noexcept = 0;
    virtual std::string toString() const noexcept = 0;
};

class ConstantClassInfo : public CPInfoImpl {
public:
    ConstantClassInfo()  = default;
    ~ConstantClassInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

private:
    uint16_t nameIndex_;
};

class ConstantFieldrefInfo : public CPInfoImpl {
public:
    ConstantFieldrefInfo()  = default;
    ~ConstantFieldrefInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getClassIndex() const noexcept {
        return this->classIndex_;
    }

    inline uint16_t getNameAndTypeIndex() const noexcept {
        return this->nameAndTypeIndex_;
    }

private:
    uint16_t classIndex_;
    uint16_t nameAndTypeIndex_;
};

class ConstantMethodrefInfo : public CPInfoImpl {
public:
    ConstantMethodrefInfo()  = default;
    ~ConstantMethodrefInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getClassIndex() const noexcept {
        return this->classIndex_;
    }

    inline uint16_t getNameAndTypeIndex() const noexcept {
        return this->nameAndTypeIndex_;
    }

private:
    uint16_t classIndex_;
    uint16_t nameAndTypeIndex_;
};

class ConstantInterfaceMethodrefInfo : public CPInfoImpl {
public:
    ConstantInterfaceMethodrefInfo()  = default;
    ~ConstantInterfaceMethodrefInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getClassIndex() const noexcept {
        return this->classIndex_;
    }

    inline uint16_t getNameAndTypeIndex() const noexcept {
        return this->nameAndTypeIndex_;
    }

private:
    uint16_t classIndex_;
    uint16_t nameAndTypeIndex_;
};

class ConstantStringInfo : public CPInfoImpl {
public:
    ConstantStringInfo()  = default;
    ~ConstantStringInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getStringIndex() const noexcept {
        return this->stringIndex_;
    }

private:
    uint16_t stringIndex_;
};

class ConstantIntegerInfo : public CPInfoImpl {
public:
    ConstantIntegerInfo()  = default;
    ~ConstantIntegerInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint32_t getBytes() const noexcept {
        return this->bytes_;
    }

private:
    uint32_t bytes_;
};

class ConstantFloatInfo : public CPInfoImpl {
public:
    ConstantFloatInfo()  = default;
    ~ConstantFloatInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    bool isNan() const noexcept;
    float getFloatValue() const noexcept;

    static constexpr uint32_t POSITIVE_INFINITY = 0x7f800000;
    static constexpr uint32_t NEGATIVE_INFINITY = 0xff800000;

private:
    uint32_t bytes_;
};

class ConstantLongInfo : public CPInfoImpl {
public:
    ConstantLongInfo()  = default;
    ~ConstantLongInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    int64_t getLongValue() const noexcept;

    inline uint32_t getHighBytes() const noexcept {
        return this->highBytes_;
    }

    inline uint32_t getLowBytes() const noexcept {
        return this->lowBytes_;
    }

private:
    uint32_t highBytes_;
    uint32_t lowBytes_;
};

class ConstantDoubleInfo : public CPInfoImpl {
public:
    ConstantDoubleInfo()  = default;
    ~ConstantDoubleInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    bool isNan() const noexcept;
    double getDoubleValue() const noexcept;

    inline uint32_t getHighBytes() const noexcept {
        return this->highBytes_;
    }

    inline uint32_t getLowBytes() const noexcept {
        return this->lowBytes_;
    }

    static constexpr uint64_t POSITIVE_INFINITY = 0x7ff0000000000000L;
    static constexpr uint64_t NEGATIVE_INFINITY = 0xfff0000000000000L;

private:
    uint32_t highBytes_;
    uint32_t lowBytes_;
};

class ConstantNameAndTypeInfo : public CPInfoImpl {
public:
    ConstantNameAndTypeInfo()  = default;
    ~ConstantNameAndTypeInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

    inline uint16_t getDescriptorIndex() const noexcept {
        return this->descriptorIndex_;
    }

private:
    uint16_t nameIndex_;
    uint16_t descriptorIndex_;
};

class ConstantUtf8Info : public CPInfoImpl {
public:
    ConstantUtf8Info()  = default;
    ~ConstantUtf8Info() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getLength() const noexcept {
        return this->length_;
    }

    inline const std::vector<uint8_t>& getBytes() const noexcept {
        return this->bytes_;
    }

    inline const std::string& getBytesStr() const noexcept {
        return this->bytesStr_;
    }

private:
    uint16_t             length_;
    std::vector<uint8_t> bytes_;
    std::string          bytesStr_;
};

class ConstantMethodHandleInfo : public CPInfoImpl {
public:
    ConstantMethodHandleInfo()  = default;
    ~ConstantMethodHandleInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint8_t getReferenceKind() const noexcept {
        return this->referenceKind_;
    }

    inline uint16_t getReferenceIndex() const noexcept {
        return this->referenceIndex_;
    }

private:
    uint8_t  referenceKind_;
    uint16_t referenceIndex_;
};

class ConstantMethodTypeInfo : public CPInfoImpl {
public:
    ConstantMethodTypeInfo()  = default;
    ~ConstantMethodTypeInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getDescriptorIndex() const noexcept {
        return this->descriptorIndex_;
    }

private:
    uint16_t descriptorIndex_;
};

class ConstantDynamicInfo : public CPInfoImpl {
public:
    ConstantDynamicInfo()  = default;
    ~ConstantDynamicInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getBootStrapMethodAttrIndex() const noexcept {
        return this->bootstrapMethodAttrIndex_;
    }

    inline uint16_t getNameAndTypeIndex() const noexcept {
        return this->nameAndTypeIndex_;
    }

private:
    uint16_t bootstrapMethodAttrIndex_;
    uint16_t nameAndTypeIndex_;
};

class ConstantInvokeDynamicInfo : public CPInfoImpl {
public:
    ConstantInvokeDynamicInfo()  = default;
    ~ConstantInvokeDynamicInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getBootStrapMethodAttrIndex() const noexcept {
        return this->bootstrapMethodAttrIndex_;
    }

    inline uint16_t getNameAndTypeIndex() const noexcept {
        return this->nameAndTypeIndex_;
    }

private:
    uint16_t bootstrapMethodAttrIndex_;
    uint16_t nameAndTypeIndex_;
};

class ConstantModuleInfo : public CPInfoImpl {
public:
    ConstantModuleInfo()  = default;
    ~ConstantModuleInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

private:
    uint16_t nameIndex_;
};

class ConstantPackageInfo : public CPInfoImpl {
public:
    ConstantPackageInfo()  = default;
    ~ConstantPackageInfo() = default;

    void load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

private:
    uint16_t nameIndex_;
};

#endif
