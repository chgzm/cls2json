#include "ClassFile.h"
#include "Format.h"
#include "Mmapper.h"
#include "ByteReader.h"
#include <sstream>

int ClassFile::load(const std::string& filePath) noexcept {
    Mmapper mmapper;
    const uint8_t* addr = (const uint8_t*)(mmapper.mmapReadOnly(filePath));
    if (addr == nullptr) {
        std::fprintf(stderr, "mmap failed.\n");
        return -1;
    }
     
    std::size_t pos = 0;

    this->magic_             = readUInt32(addr, pos);
    this->minorVersion_      = readUInt16(addr, pos);
    this->majorVersion_      = readUInt16(addr, pos);
    this->constantPoolCount_ = readUInt16(addr, pos);

    if (this->loadConstantPool(addr, pos) != 0) {
        std::fprintf(stderr, "Failed to load Constant Pool.\n");
        return -1;
    }

    this->accessFlags_ = readUInt16(addr, pos);
    this->thisClass_   = readUInt16(addr, pos);
    this->superClass_  = readUInt16(addr, pos);

    const uint16_t interfacesCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < interfacesCount; ++i) {
        this->interfaces_.push_back(readUInt16(addr, pos));
    }

    if (this->loadFields(addr, pos) != 0) {
        std::fprintf(stderr, "Failed to load Fields.\n");
        return -1;
    } 

    if (this->loadMethods(addr, pos) != 0) {
        std::fprintf(stderr, "Failed to load Methods.\n");
        return -1;
    }
    
    if (this->loadAttributes(addr, pos) != 0) {
        std::fprintf(stderr, "Failed to load Attributes.\n");
        return -1;
    }
     
    return 0;
}

int ClassFile::loadConstantPool(const uint8_t* addr, std::size_t& pos) noexcept {
    // 4.1. The ClassFile Structure
    // The value of the constant_pool_count item is equal to the number of entries in the constant_pool table plus one 
    // The constant_pool table is indexed from 1 to constant_pool_count - 1.
    this->constantPool_.push_back(nullptr);

    uint16_t index = 1;
    while (index < this->constantPoolCount_) {
        auto cpInfo = std::make_unique<CPInfo>();
        if (cpInfo->load(addr, pos) != 0) {
            return -1;
        }

        const uint8_t tag = cpInfo->getTag();
        this->constantPool_.push_back(std::move(cpInfo));

        if (tag == CPInfo::CONSTANT_Long || tag == CPInfo::CONSTANT_Double) {
            this->constantPool_.push_back(nullptr);
            index += 2;
        } else {
            ++index;
        }
    }

    return 0;
}

int ClassFile::loadFields(const uint8_t* addr, std::size_t& pos) noexcept {
    const uint16_t fieldsCount = readUInt16(addr, pos);

    for (uint16_t i = 0; i < fieldsCount; ++i) {
        auto fieldInfo = std::make_unique<FieldInfo>();
        if (fieldInfo->load(addr, pos, this->getConstantPool()) != 0) {
            return -1;
        }
        this->fields_.push_back(std::move(fieldInfo));
    }

    return 0;
}

int ClassFile::loadMethods(const uint8_t* addr, std::size_t& pos) noexcept {
    const uint16_t methodsCount = readUInt16(addr, pos);

    for (uint16_t i = 0; i < methodsCount; ++i) {
        auto methodInfo = std::make_unique<MethodInfo>();

        if (methodInfo->load(addr, pos, this->getConstantPool()) != 0) {
            return -1;
        }

        this->methods_.push_back(std::move(methodInfo));
    }

    return 0;
}

int ClassFile::loadAttributes(const uint8_t* addr, std::size_t& pos) noexcept {
    const uint16_t attributeCount = readUInt16(addr, pos);
  
    for (uint16_t i = 0; i < attributeCount; ++i) {
        auto attributeInfo = std::make_unique<AttributeInfo>();

        if (attributeInfo->load(addr, pos, this->getConstantPool()) != 0) {
            return -1;
        }

        this->attributes_.push_back(std::move(attributeInfo));
    }

    return 0;
}

std::string ClassFile::getAccessFlagsStr() const noexcept {
    std::string decoded;

    if (this->accessFlags_ & ClassFile::ACC_PUBLIC)     { decoded.append("ACC_PUBLIC|");     }
    if (this->accessFlags_ & ClassFile::ACC_FINAL)      { decoded.append("ACC_FINAL|");      }
    if (this->accessFlags_ & ClassFile::ACC_SUPER)      { decoded.append("ACC_SUPER|");      }
    if (this->accessFlags_ & ClassFile::ACC_INTERFACE)  { decoded.append("ACC_INTERFACE|");  }
    if (this->accessFlags_ & ClassFile::ACC_ABSTRACT)   { decoded.append("ACC_ABSTRACT|");   }
    if (this->accessFlags_ & ClassFile::ACC_SYSTHETIC)  { decoded.append("ACC_SYSTHETIC|");  }
    if (this->accessFlags_ & ClassFile::ACC_ANNOTATION) { decoded.append("ACC_ANNOTATION|"); }
    if (this->accessFlags_ & ClassFile::ACC_ENUM)       { decoded.append("ACC_ENUM|");       }
    if (this->accessFlags_ & ClassFile::ACC_MODULE)     { decoded.append("ACC_MODULE|");     }

    if (decoded.length() != 0) {
        decoded.pop_back();
    } 

    return decoded;
}

std::string ClassFile::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "{"
        "\"magic\":\"0x%0x\","
        "\"minor_version\":%hu,"
        "\"major_version\":%hu,"
        "\"constant_pool_count\":%hu,",
        this->getMagic(),
        this->getMinorVersion(),
        this->getMajorVersion(),
        this->getConstantPoolCount()
    );

    ss << "\"constant_pool\":[";
    for (uint16_t i = 0; i < this->getConstantPoolCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        const CPInfo* cp = this->getCPAt(i);
        if (cp == nullptr) {
            ss << "\"null\"";
        } else {
            ss << "{" << cp->toString() << "}";
        }
    }
    ss << "],";

    ss << fmt(
        "\"access_Flags\":\"0x%hu\","
        "\"this_class\":%hu,"
        "\"super_class\":%hu,"
        "\"interfaces_count\":%hu,",
        this->getAccessFlags(),
        this->getThisClass(),
        this->getSuperClass(),
        this->getInterfacesCount()
    );
   
    ss << "\"interfaces\":[";
    for (uint16_t i = 0; i < this->getInterfacesCount(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << this->getInterfaceAt(i);
    }     
    ss << "],";

    ss << fmt("\"fields_count\":%hu,", this->getFieldsCount());
    ss << "\"fields\":[";
    for (uint16_t i = 0; i < this->getFieldsCount(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "{" << this->getFieldAt(i)->toString() << "}";
    }
    ss << "],";

    ss << fmt("\"methods_count\":%hu,", this->getMethodsCount());

    ss << "\"methods\":[";
    for (uint16_t i = 0; i < this->getMethodsCount(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "{" << this->getMethodAt(i)->toString() << "}";
    }
    ss << "],";

    ss << fmt("\"attributes_count\":%hu,", this->getAttributesCount());

    ss << "\"attributes\":[";
    for (uint16_t i = 0; i < this->getAttributesCount(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "{" << this->getAttributeAt(i)->toString() << "}";
    }
    ss << "]" << "}";

    return ss.str();
}
