#include "CPInfo.h"
#include "Format.h"
#include "ByteReader.h"
#include <sstream>

int CPInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->tag_ = readUInt8(addr, pos);
    switch (this->tag_) {
    case CPInfo::CONSTANT_Class:              { this->info_ = std::make_unique<ConstantClassInfo>();              break; }
    case CPInfo::CONSTANT_Fieldref:           { this->info_ = std::make_unique<ConstantFieldrefInfo>();           break; }
    case CPInfo::CONSTANT_Methodref:          { this->info_ = std::make_unique<ConstantMethodrefInfo>();          break; }
    case CPInfo::CONSTANT_InterfaceMethodref: { this->info_ = std::make_unique<ConstantInterfaceMethodrefInfo>(); break; }
    case CPInfo::CONSTANT_String:             { this->info_ = std::make_unique<ConstantStringInfo>();             break; }
    case CPInfo::CONSTANT_Integer:            { this->info_ = std::make_unique<ConstantIntegerInfo>();            break; }
    case CPInfo::CONSTANT_Float:              { this->info_ = std::make_unique<ConstantFloatInfo>();              break; }
    case CPInfo::CONSTANT_Long:               { this->info_ = std::make_unique<ConstantLongInfo>();               break; }
    case CPInfo::CONSTANT_Double:             { this->info_ = std::make_unique<ConstantDoubleInfo>();             break; }
    case CPInfo::CONSTANT_NameAndType:        { this->info_ = std::make_unique<ConstantNameAndTypeInfo>();        break; }
    case CPInfo::CONSTANT_Utf8:               { this->info_ = std::make_unique<ConstantUtf8Info>();               break; }
    case CPInfo::CONSTANT_MethodHandle:       { this->info_ = std::make_unique<ConstantMethodrefInfo>();          break; }
    case CPInfo::CONSTANT_MethodType:         { this->info_ = std::make_unique<ConstantMethodTypeInfo>();         break; }
    case CPInfo::CONSTANT_Dynamic:            { this->info_ = std::make_unique<ConstantDynamicInfo>();            break; }
    case CPInfo::CONSTANT_InvokeDynamic:      { this->info_ = std::make_unique<ConstantInvokeDynamicInfo>();      break; }
    case CPInfo::CONSTANT_Module:             { this->info_ = std::make_unique<ConstantModuleInfo>();             break; }
    case CPInfo::CONSTANT_Package:            { this->info_ = std::make_unique<ConstantPackageInfo>();            break; }
    default: {
        std::fprintf(stderr, "Invalid tag=%hu\n", this->tag_);
        return -1;
    }
    }

    this->info_->load(addr, pos);

    return 0;
}

void ConstantClassInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->nameIndex_ = readUInt16(addr, pos);
}

void ConstantFieldrefInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->classIndex_       = readUInt16(addr, pos);
    this->nameAndTypeIndex_ = readUInt16(addr, pos);
}

void ConstantMethodrefInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->classIndex_       = readUInt16(addr, pos);
    this->nameAndTypeIndex_ = readUInt16(addr, pos);
}

void ConstantInterfaceMethodrefInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->classIndex_       = readUInt16(addr, pos);
    this->nameAndTypeIndex_ = readUInt16(addr, pos);
}

void ConstantStringInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->stringIndex_ = readUInt16(addr, pos);
}

void ConstantIntegerInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->bytes_ = readUInt32(addr, pos);
}

void ConstantFloatInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->bytes_ = readUInt32(addr, pos);
}

void ConstantLongInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->highBytes_ = readUInt32(addr, pos);
    this->lowBytes_  = readUInt32(addr, pos);
}

void ConstantDoubleInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->highBytes_ = readUInt32(addr, pos);
    this->lowBytes_  = readUInt32(addr, pos);
}

void ConstantNameAndTypeInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->nameIndex_       = readUInt16(addr, pos);
    this->descriptorIndex_ = readUInt16(addr, pos);
}

void ConstantUtf8Info::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->length_ = readUInt16(addr, pos);
    for (uint16_t i = 0; i < this->getLength(); ++i) {
        this->bytes_.push_back(readUInt8(addr, pos));
    }

    this->bytesStr_ = std::string(this->bytes_.begin(), this->bytes_.end());
    // this->bytes_  = std::make_unique<uint8_t[]>(this->length_);
    // std::memcpy((void*)(this->bytes_.get()), &(addr[pos]), this->length_);
    // this->bytesStr_ = std::move(std::string((const char*)(this->bytes_.get()), this->length_));
    // pos += this->length_;
}

void ConstantMethodHandleInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->referenceKind_  = readUInt16(addr, pos);
    this->referenceIndex_ = readUInt16(addr, pos);
}

void ConstantMethodTypeInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->descriptorIndex_ = readUInt16(addr, pos);
}

void ConstantDynamicInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->bootstrapMethodAttrIndex_ = readUInt16(addr, pos);
    this->nameAndTypeIndex_         = readUInt16(addr, pos);
}

void ConstantInvokeDynamicInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->bootstrapMethodAttrIndex_ = readUInt16(addr, pos);
    this->nameAndTypeIndex_         = readUInt16(addr, pos);
}

void ConstantModuleInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->nameIndex_ = readUInt16(addr, pos);
}

void ConstantPackageInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->nameIndex_ = readUInt16(addr, pos);
}

std::string CPInfo::toString() const noexcept {
    return fmt(
        "\"tag\":%hu,"
        "%s",
        this->getTag(), 
        this->getInfo()->toString().c_str()
    );
}

const char* CPInfo::decodeTag(uint8_t tag) const noexcept {
    switch (tag) {
    case CPInfo::CONSTANT_Class:              { return "CONSTANT_Class";              }
    case CPInfo::CONSTANT_Fieldref:           { return "CONSTANT_Fieldref";           }
    case CPInfo::CONSTANT_Methodref:          { return "CONSTANT_Methodref";          }
    case CPInfo::CONSTANT_InterfaceMethodref: { return "CONSTANT_InterfaceMethodref"; }
    case CPInfo::CONSTANT_String:             { return "CONSTANT_String";             }
    case CPInfo::CONSTANT_Integer:            { return "CONSTANT_Integer";            }
    case CPInfo::CONSTANT_Float:              { return "CONSTANT_Float";              }
    case CPInfo::CONSTANT_Long:               { return "CONSTANT_Long";               }
    case CPInfo::CONSTANT_Double:             { return "CONSTANT_Double";             }
    case CPInfo::CONSTANT_NameAndType:        { return "CONSTANT_NameAndType";        }
    case CPInfo::CONSTANT_Utf8:               { return "CONSTANT_Utf8";               }
    case CPInfo::CONSTANT_MethodHandle:       { return "CONSTANT_Methodref";          }
    case CPInfo::CONSTANT_MethodType:         { return "CONSTANT_MethodType";         }
    case CPInfo::CONSTANT_Dynamic:            { return "CONSTANT_Dynamic";            }
    case CPInfo::CONSTANT_InvokeDynamic:      { return "CONSTANT_InvokeDynamic";      }
    case CPInfo::CONSTANT_Module:             { return "CONSTANT_Module";             }
    case CPInfo::CONSTANT_Package:            { return "CONSTANT_Package";            }
    default:                                  { return "INVALID";                     }
    }
}

std::string ConstantClassInfo::toString() const noexcept {
    return fmt("\"name_index\":%hu", this->nameIndex_);
}

std::string ConstantFieldrefInfo::toString() const noexcept {
    return fmt("\"class_index\":%hu,\"name_and_type_index\":%hu", this->classIndex_, this->nameAndTypeIndex_);
}

std::string ConstantMethodrefInfo::toString() const noexcept {
    return fmt("\"class_index\":%hu,\"name_and_type_index\":%hu", this->classIndex_, this->nameAndTypeIndex_);
}

std::string ConstantInterfaceMethodrefInfo::toString() const noexcept {
    return fmt("\"class_index\":%hu,\"name_and_type_index\":%hu", this->classIndex_, this->nameAndTypeIndex_);
}

std::string ConstantStringInfo::toString() const noexcept {
    return fmt("\"string_index\":%hu", this->stringIndex_);
}

std::string ConstantIntegerInfo::toString() const noexcept {
    return fmt("\"bytes\":%u", this->bytes_);
}

std::string ConstantFloatInfo::toString() const noexcept {
    return fmt("\"bytes\":%u", this->bytes_);
}

std::string ConstantLongInfo::toString() const noexcept {
    return fmt("\"high_bytes\":%u,\"low_bytes\":%u", this->highBytes_, this->lowBytes_);
}

std::string ConstantDoubleInfo::toString() const noexcept {
    return fmt("\"high_bytes\":%u,\"low_bytes\":%u", this->highBytes_, this->lowBytes_);
}

std::string ConstantNameAndTypeInfo::toString() const noexcept {
    return fmt("\"name_index\":%hu,\"descriptor_index\":%hu", this->nameIndex_, this->descriptorIndex_);
}

std::string ConstantUtf8Info::toString() const noexcept {
    return fmt("\"length\":%hu,\"bytes\":\"%s\"", this->length_, this->bytesStr_.c_str());
}

std::string ConstantMethodHandleInfo::toString() const noexcept {
    return fmt("\"reference_kind\":%hhu,\"reference_index\":%hu", this->referenceKind_, this->referenceIndex_);
}

std::string ConstantMethodTypeInfo::toString() const noexcept {
    return fmt("\"descriptor_index\":%hhu", this->descriptorIndex_);
}

std::string ConstantDynamicInfo::toString() const noexcept {
    return fmt("\"bootstrap_method_attr_index\":%hu,\"name_and_type_index\":%hu", this->bootstrapMethodAttrIndex_, this->nameAndTypeIndex_);
}

std::string ConstantInvokeDynamicInfo::toString() const noexcept {
    return fmt("\"bootstrap_method_attr_index\":%hu,\"name_and_type_index\":%hu", this->bootstrapMethodAttrIndex_, this->nameAndTypeIndex_);
}

std::string ConstantModuleInfo::toString() const noexcept {
    return fmt("\"name_index\":%hu", this->nameIndex_);
}

std::string ConstantPackageInfo::toString() const noexcept {
    return fmt("\"name_index\":%hu", this->nameIndex_);
}


