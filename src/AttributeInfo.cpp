#include "AttributeInfo.h"
#include "Format.h"
#include "ByteReader.h"
#include <sstream>

int AttributeInfo::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp) noexcept {
    this->attributeNameIndex_ = readUInt16(addr, pos);
    this->attributeLength_    = readUInt32(addr, pos);

    const std::unique_ptr<CPInfo>& cpInfo = cp[this->attributeNameIndex_];
    if (cpInfo->getTag() != CPInfo::CONSTANT_Utf8) {
        std::fprintf(stderr, "Invalid Tag.\n");
        return -1;
    }

    const ConstantUtf8Info* utf8Info = (const ConstantUtf8Info*)(cpInfo->getInfo());
    this->attributeName_ = utf8Info->getBytesStr();
    if (this->attributeName_ == "ConstantValue") {
        this->type_ = AttributeType::ConstantValue;
        this->info_ = std::make_unique<ConstantValueAttribute>();
    }
    else if (this->attributeName_ == "Code") {
        this->type_ = AttributeType::Code;
        this->info_ = std::make_unique<CodeAttribute>();
    }
    else if (this->attributeName_ == "StackMapTable") {
        this->type_ = AttributeType::StackMapTable;
        this->info_ = std::make_unique<StackMapTableAttribute>();
    }
    else if (this->attributeName_ == "Exceptions") {
        this->type_ = AttributeType::Exceptions;
        this->info_ = std::make_unique<ExceptionsAttribute>();
    }
    else if (this->attributeName_ == "InnerClasses") {
        this->type_ = AttributeType::InnerClasses;
        this->info_ = std::make_unique<InnerClassesAttribute>();
    }
    else if (this->attributeName_ == "EnclosingMethod") {
        this->type_ = AttributeType::EnclosingMethod;
        this->info_ = std::make_unique<EnclosingMethodAttribute>();
    }
    else if (this->attributeName_ == "Synthetic") {
        this->type_ = AttributeType::Synthetic;
        this->info_ = std::make_unique<SyntheticAttribute>();
    }
    else if (this->attributeName_ == "Signature") {
        this->type_ = AttributeType::Signature;
        this->info_ = std::make_unique<SignatureAttribute>();
    }
    else if (this->attributeName_ == "SourceFile") {
        this->type_ = AttributeType::SourceFile;
        this->info_ = std::make_unique<SourceFileAttribute>();
    }
    else if (this->attributeName_ == "SourceDebugExtension") {
        this->type_ = AttributeType::SourceDebugExtension;
        this->info_ = std::make_unique<SourceDebugExtensionAttribute>();
    }
    else if (this->attributeName_ == "LineNumberTable") {
        this->type_ = AttributeType::LineNumberTable;
        this->info_ = std::make_unique<LineNumberTableAttribute>();
    }
    else if (this->attributeName_ == "LocalVariableTable") {
        this->type_ = AttributeType::LocalVariableTable;
        this->info_ = std::make_unique<LocalVariableTableAttribute>();
    }
    else if (this->attributeName_ == "LocalVariableTypeTable") {
        this->type_ = AttributeType::LocalVariableTypeTable;
        this->info_ = std::make_unique<LocalVariableTypeTableAttribute>();
    }
    else if (this->attributeName_ == "Deprecated") {
        this->type_ = AttributeType::Deprecated;
        this->info_ = std::make_unique<DeprecatedAttribute>();
    }
    else if (this->attributeName_ == "RuntimeVisibleAnnotations") {
        this->type_ = AttributeType::RuntimeVisibleAnnotations;
        this->info_ = std::make_unique<RuntimeVisibleAnnotationsAttribute>();
    }
    else if (this->attributeName_ == "RuntimeInvisibleAnnotations") {
        this->type_ = AttributeType::RuntimeInvisibleAnnotations;
        this->info_ = std::make_unique<RuntimeInvisibleAnnotationsAttribute>();
    }
    else if (this->attributeName_ == "RuntimeVisibleParameterAnnotations") {
        this->type_ = AttributeType::RuntimeVisibleParameterAnnotations;
        this->info_ = std::make_unique<RuntimeVisibleParameterAnnotationsAttribute>();
    }
    else if (this->attributeName_ == "RuntimeInvisibleParameterAnnotations") {
        this->type_ = AttributeType::RuntimeInvisibleParameterAnnotations;
        this->info_ = std::make_unique<RuntimeInvisibleParameterAnnotationsAttribute>();
    }
    else if (this->attributeName_ == "RuntimeVisibleTypeAnnotations") {
        this->type_ = AttributeType::RuntimeVisibleTypeAnnotations;
        this->info_ = std::make_unique<RuntimeVisibleTypeAnnotationsAttribute>();
    }
    else if (this->attributeName_ == "RuntimeInvisibleTypeAnnotations") {
        this->type_ = AttributeType::RuntimeInvisibleTypeAnnotations;
        this->info_ = std::make_unique<RuntimeInvisibleTypeAnnotationsAttribute>();
    }
    else if (this->attributeName_ == "AnnotationDefault") {
        this->type_ = AttributeType::AnnotationDefault;
        this->info_ = std::make_unique<AnnotationDefaultAttribute>();
    }
    else if (this->attributeName_ == "BootstrapMethods") {
        this->type_ = AttributeType::BootstrapMethods;
        this->info_ = std::make_unique<BootstrapMethodsAttribute>();
    }
    else if (this->attributeName_ == "MethodParameters") {
        this->type_ = AttributeType::MethodParameters;
        this->info_ = std::make_unique<MethodParametersAttribute>();
    }
    else if (this->attributeName_ == "Module") {
        this->type_ = AttributeType::Module;
        this->info_ = std::make_unique<ModuleAttribute>();
    }
    else if (this->attributeName_ == "ModulePackages") {
        this->type_ = AttributeType::ModulePackages;
        this->info_ = std::make_unique<ModulePackagesAttribute>();
    }
    else if (this->attributeName_ == "ModuleMainClass") {
        this->type_ = AttributeType::ModuleMainClass;
        this->info_ = std::make_unique<ModuleMainClassAttribute>();
    }
    else if (this->attributeName_ == "NestHost") {
        this->type_ = AttributeType::NestHost;
        this->info_ = std::make_unique<NestHostAttribute>();
    }
    else if (this->attributeName_ == "NestMembers") {
        this->type_ = AttributeType::NestMembers;
        this->info_ = std::make_unique<NestMembersAttribute>();
    }
    else {
        std::fprintf(stderr, "Invalid BytesStr=\"%s\"\n", this->attributeName_.c_str());
        return -1;
    }

    if (this->info_->load(addr, pos, cp, *this) != 0) {
        return -1;
    }

    return 0;
}

int ConstantValueAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->constantValueIndex_ = readUInt16(addr, pos);

    return 0;
}

int Exception::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->startPC_    = readUInt16(addr, pos);
    this->endPC_      = readUInt16(addr, pos);
    this->handlerPC_  = readUInt16(addr, pos);
    this->catchType_  = readUInt16(addr, pos);

    return 0;
}

int CodeAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->maxStack_  = readUInt16(addr, pos);
    this->maxLocals_ = readUInt16(addr, pos);

    const uint32_t codeLength = readUInt32(addr, pos);
    for (uint32_t i = 0; i < codeLength; ++i) {
        this->code_.push_back(readUInt8(addr, pos));
    }

    const uint16_t exceptionTableLength = readUInt16(addr, pos);
    for (uint16_t i = 0; i < exceptionTableLength; ++i) {
        std::unique_ptr<Exception> exception = std::make_unique<Exception>();
        if (exception->load(addr, pos) != 0) {
            return -1;
        }
        this->exceptionTable_.push_back(std::move(exception));
    }

    const uint16_t attributeCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < attributeCount; ++i) {
        std::unique_ptr<AttributeInfo> attributeInfo = std::make_unique<AttributeInfo>();
        if (attributeInfo->load(addr, pos, cp) != 0) {
            return -1;
        }

        this->attributes_.push_back(std::move(attributeInfo));
    }

    return 0;
}

int VerificationTypeInfo::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->tag_ = readUInt8(addr, pos);
    
    if (this->tag_ == ITEM_Object || this->tag_ == ITEM_Uninitialized) {
        this->moreInfo_ = readUInt16(addr, pos);
    }

    return 0;
}

int SameFrame::load(const uint8_t* addr, std::size_t& pos) noexcept {
    return 0;
}

int SameLocals1StackItemFrame::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->stack_ = std::make_unique<VerificationTypeInfo>();
    if (this->stack_->load(addr, pos) != 0) {
        return -1;
    }

    return 0;
}

int SameLocals1StackItemFrameExtended::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->offsetDelta_ = readUInt16(addr, pos);  

    this->stack_ = std::make_unique<VerificationTypeInfo>();
    if (this->stack_->load(addr, pos) != 0) {
        return -1;
    }

    return 0;
}
int ChopFrame::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->offsetDelta_ = readUInt16(addr, pos);  

    return 0;
}

int SameFrameExtended::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->offsetDelta_ = readUInt16(addr, pos);  

    return 0;
}

int AppendFrame::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->offsetDelta_ = readUInt16(addr, pos);  
    
    const uint8_t numberOfLocals = this->frameType_ - 251;
    for (uint8_t i = 0; i < numberOfLocals; ++i) {
        std::unique_ptr<VerificationTypeInfo> info = std::make_unique<VerificationTypeInfo>(); 
        
        if (info->load(addr, pos) != 0) {
            return -1;
        }

        this->locals_.push_back(std::move(info));
    }

    return 0;
}

int FullFrame::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->offsetDelta_ = readUInt16(addr, pos);  
    
    const uint16_t numberOfLocals = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numberOfLocals; ++i) {
        std::unique_ptr<VerificationTypeInfo> info = std::make_unique<VerificationTypeInfo>(); 
        
        if (info->load(addr, pos) != 0) {
            return -1;
        }

        this->locals_.push_back(std::move(info));
    }

    const uint16_t numberOfStackItems = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numberOfStackItems; ++i) {
        std::unique_ptr<VerificationTypeInfo> info = std::make_unique<VerificationTypeInfo>(); 
        
        if (info->load(addr, pos) != 0) {
            return -1;
        }

        this->stack_.push_back(std::move(info));
    }

    return 0;
}

int StackMapFrame::load(const uint8_t* addr, std::size_t& pos) noexcept {
    const uint8_t type = readUInt8(addr, pos);
    this->frameType_ = readUInt8(addr, pos);

    if      (type <= 63)                 { this->frame_ = std::make_unique<SameFrame>();                         }
    else if (type <= 127)                { this->frame_ = std::make_unique<SameLocals1StackItemFrame>();         }
    else if (type == 247)                { this->frame_ = std::make_unique<SameLocals1StackItemFrameExtended>(); }
    else if (248 <= type && type <= 127) { this->frame_ = std::make_unique<SameLocals1StackItemFrame>();         }
    else if (type == 251)                { this->frame_ = std::make_unique<SameFrameExtended>();                 }
    else if (252 <= type && type <= 254) { this->frame_ = std::make_unique<AppendFrame>(type);                   }
    else if (type == 255)                { this->frame_ = std::make_unique<FullFrame>();                         }
    else {
        return -1;
    }        

    if (this->frame_->load(addr, pos) != 0) {
        return -1;
    }

    return 0;
}

int StackMapTableAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numberOfEntries = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numberOfEntries; ++i) {
        std::unique_ptr<StackMapFrame> stackMapFrame = std::make_unique<StackMapFrame>();

        if (stackMapFrame->load(addr, pos) != 0) {
            return -1;
        }

        this->entries_.push_back(std::move(stackMapFrame));
    }

    return 0;
}

int ExceptionsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numberOfExceptions = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numberOfExceptions; ++i) {
        this->exceptionIndexTable_.push_back(readUInt16(addr, pos));
    }

    return 0;
}

int Class::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->innerClassInfoIndex_   = readUInt16(addr, pos);
    this->outerClassInfoIndex_   = readUInt16(addr, pos);
    this->innerNameIndex_        = readUInt16(addr, pos);
    this->innerClassAccessFlags_ = readUInt16(addr, pos);

    return 0;
}

int InnerClassesAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numberOfClasses = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numberOfClasses; ++i) {
        auto _class = std::make_unique<Class>();
        if (_class->load(addr, pos, cp, info) != 0) {
            return -1;
        }

        this->classes_.push_back(std::move(_class));
    }

    return 0;
}

int EnclosingMethodAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->classIndex_   = readUInt16(addr, pos);
    this->methodIndex_  = readUInt16(addr, pos);

    return 0;
}

int SyntheticAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    return 0;
}

int SignatureAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->signatureIndex_ = readUInt16(addr, pos);

    return 0;
}

int SourceFileAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->sourceFileIndex = readUInt16(addr, pos);

    return 0;
}

int SourceDebugExtensionAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    for (uint32_t i = 0; i < info.getAttributeLength(); ++i) {
        this->debugExtension_.push_back(readUInt8(addr, pos));
    }

    this->debugExtensionStr_ = std::string(this->debugExtension_.begin(), this->debugExtension_.end());

    return 0;
}

int LineNumber::load(const uint8_t* addr, std::size_t& pos) noexcept {
    this->startPC_    = readUInt16(addr, pos);
    this->lineNumber_ = readUInt16(addr, pos);

    return 0;
}

int LineNumberTableAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t lineNumberTableLength = readUInt16(addr, pos);
    for (uint16_t i = 0; i < lineNumberTableLength; ++i) {
        std::unique_ptr<LineNumber> lineNumber = std::make_unique<LineNumber>();
        if (lineNumber->load(addr, pos) != 0) {
            return -1;
        }

        this->lineNumberTable_.push_back(std::move(lineNumber));
    }

    return 0;
}

int LocalVariable::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->startPC_          = readUInt16(addr, pos);
    this->length_           = readUInt16(addr, pos);
    this->nameIndex_        = readUInt16(addr, pos);
    this->descriptorIndex_  = readUInt16(addr, pos);
    this->index_            = readUInt16(addr, pos);

    return 0;
}

int LocalVariableTableAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t localVariableTableLength = readUInt16(addr, pos);
    for (uint16_t i = 0; i < localVariableTableLength; ++i) {
        auto localVariable = std::make_unique<LocalVariable>();
        if (localVariable->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->localVariableTable_.push_back(std::move(localVariable));
    }

    return 0;
}

int LocalVariableType::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->startPC_        = readUInt16(addr, pos);
    this->length_         = readUInt16(addr, pos);
    this->nameIndex_      = readUInt16(addr, pos);
    this->signatureIndex_ = readUInt16(addr, pos);
    this->index_          = readUInt16(addr, pos);

    return 0;
}

int LocalVariableTypeTableAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t localVariableTypeTableLength = readUInt16(addr,  pos);
    for (uint16_t i = 0; i < localVariableTypeTableLength; ++i) {
        auto localVariableType = std::make_unique<LocalVariableType>();
        if (localVariableType->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->localVariableTypeTable_.push_back(std::move(localVariableType));
    }

    return 0;
}

int DeprecatedAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    return 0;
}

int EnumConstValue::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->typeNameIndex_  = readUInt16(addr, pos);
    this->constNameIndex_ = readUInt16(addr, pos);

    return 0;
}

int ArrayValue::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numValues = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numValues; ++i) {
        auto elementValue = std::make_unique<ElementValue>();
        if (elementValue->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->values_.push_back(std::move(elementValue));
    }

    return 0;
}

ElementValue::~ElementValue() {
    switch (this->tag_) {
    case 'e': {
        delete (EnumConstValue*)(this->value_);
        break;
    }
    case '@': {
        delete (Annotation*)(this->value_);
        break;
    }
    case '[': {
        delete (ArrayValue*)(this->value_);
        break;
    }
    default: {
        break;
    }
    }

}

int ElementValue::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->tag_ = readUInt8(addr, pos);

    switch (this->tag_) {
    case 'B':
    case 'C':
    case 'D':
    case 'F':
    case 'I':
    case 'J':
    case 'S':
    case 'Z':
    case 's': {
        this->value_ = (void*)((uint64_t)(readUInt16(addr, pos)));
        return 0;
    }
    case 'e': {
        this->value_ = (void*)(new EnumConstValue());
        if (((EnumConstValue*)(this->value_))->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        return 0;
    }
    case 'c': {
        this->value_ = (void*)((uint64_t)(readUInt16(addr, pos)));
        return 0;
    }
    case '@': {
        this->value_ = (void*)(new Annotation());
        if (((Annotation*)(this->value_))->load(addr, pos, cp, info) != 0) {
            return -1;
        }
    }
    case '[': {
        this->value_ = (void*)(new ArrayValue());
        if (((ArrayValue*)(this->value_))->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        return 0;
    }
    default: {
        return -1;
    }
    }
}

int ElementValuePair::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->elementNameIndex_ = readUInt16(addr, pos);
    this->value_ = std::make_unique<ElementValue>();
    if (this->value_->load(addr, pos, cp, info) != 0) {
        return -1;
    }

    return 0;
}

int Annotation::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->typeIndex_ = readUInt16(addr, pos);
    const uint16_t numElementValuePairs = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numElementValuePairs; ++i) {
        auto elementValuePair = std::make_unique<ElementValuePair>();
        if (elementValuePair->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->elementValuePairs_.push_back(std::move(elementValuePair));
    }

    return 0;
}

int RuntimeVisibleAnnotationsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numAnnotations = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numAnnotations; ++i) {
        auto annotation = std::make_unique<Annotation>();
        if (annotation->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->annotations_.push_back(std::move(annotation));
    }

    return 0;
}

int RuntimeInvisibleAnnotationsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numAnnotations = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numAnnotations; ++i) {
        auto annotation = std::make_unique<Annotation>();
        if (annotation->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->annotations_.push_back(std::move(annotation));
    }

    return 0;
}

int ParameterAnnotation::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numAnnotations = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numAnnotations; ++i) {
        auto annotation = std::make_unique<Annotation>();
        if (annotation->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->annotations_.push_back(std::move(annotation));
    }

    return 0;
}

int RuntimeVisibleParameterAnnotationsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint8_t numParameters = readUInt8(addr, pos);
    for (uint8_t i = 0; i < numParameters; ++i) {
        auto parameterAnnotation = std::make_unique<ParameterAnnotation>();
        if (parameterAnnotation->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->parameterAnnotations_.push_back(std::move(parameterAnnotation));

    }

    return 0;
}

int RuntimeInvisibleParameterAnnotationsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint8_t numParameters = readUInt8(addr, pos);
    for (uint8_t i = 0; i < numParameters; ++i) {
        auto parameterAnnotation = std::make_unique<ParameterAnnotation>();
        if (parameterAnnotation->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->parameterAnnotations_.push_back(std::move(parameterAnnotation));

    }
    return 0;
}

int TypeParameterTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->typeParameterIndex_ = readUInt8(addr, pos);

    return 0;
}

int SupertypeTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->supertypeIndex_ = readUInt16(addr, pos);

    return 0;
}

int TypeParameterBoundTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->typeParameterIndex_ = readUInt8(addr, pos);
    this->boundIndex_         = readUInt8(addr, pos);

    return 0;
}

int EmptyTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    return 0;
}

int FormalParameterTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->formalParameterIndex_ = readUInt8(addr, pos);

    return 0;
}

int ThrowsTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->throwsTypeIndex_ = readUInt16(addr, pos);

    return 0;
}

int Localvar::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->startPC_ = readUInt16(addr, pos);
    this->length_  = readUInt16(addr, pos);
    this->index_   = readUInt16(addr, pos);

    return 0;
}

int LocalvarTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t tableLength = readUInt16(addr, pos);
    for (uint16_t i = 0; i < tableLength; ++i) {
        auto localvar = std::make_unique<Localvar>();
        if (localvar->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->table_.push_back(std::move(localvar));
    }

    return 0;
}

int CatchTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->exceptionTableIndex_ = readUInt16(addr, pos);

    return 0;
}

int OffsetTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->offset_ = readUInt16(addr, pos);

    return 0;
}

int TypeArgumentTarget::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->offset_ = readUInt16(addr, pos);

    return 0;
}

int Path::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->typePathKind_      = readUInt8(addr, pos);
    this->typeArgumentIndex_ = readUInt8(addr, pos);

    return 0;
}

int TypePath::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint8_t pathLength = readUInt8(addr, pos);
    for (uint8_t i = 0; i < pathLength; ++i) {
        auto path = std::make_unique<Path>();
        if (path->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->path_.push_back(std::move(path));
    }

    return 0;
}

int TypeAnnotation::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->targetType_ = readUInt8(addr, pos);
    switch (this->targetType_) {
    case 0x00: { this->targetInfo_ = std::make_unique<TypeParameterTarget>();      break; }
    case 0x01: { this->targetInfo_ = std::make_unique<TypeParameterTarget>();      break; }
    case 0x10: { this->targetInfo_ = std::make_unique<SupertypeTarget>();          break; }
    case 0x11: { this->targetInfo_ = std::make_unique<TypeParameterBoundTarget>(); break; }
    case 0x12: { this->targetInfo_ = std::make_unique<TypeParameterBoundTarget>(); break; }
    case 0x13: { this->targetInfo_ = std::make_unique<EmptyTarget>();              break; }
    case 0x14: { this->targetInfo_ = std::make_unique<EmptyTarget>();              break; }
    case 0x15: { this->targetInfo_ = std::make_unique<EmptyTarget>();              break; }
    case 0x16: { this->targetInfo_ = std::make_unique<FormalParameterTarget>();    break; }
    case 0x17: { this->targetInfo_ = std::make_unique<ThrowsTarget>();             break; }
    case 0x40: { this->targetInfo_ = std::make_unique<LocalvarTarget>();           break; }
    case 0x41: { this->targetInfo_ = std::make_unique<LocalvarTarget>();           break; }
    case 0x42: { this->targetInfo_ = std::make_unique<CatchTarget>();              break; }
    case 0x43: { this->targetInfo_ = std::make_unique<OffsetTarget>();             break; }
    case 0x44: { this->targetInfo_ = std::make_unique<OffsetTarget>();             break; }
    case 0x45: { this->targetInfo_ = std::make_unique<OffsetTarget>();             break; }
    case 0x46: { this->targetInfo_ = std::make_unique<OffsetTarget>();             break; }
    case 0x47: { this->targetInfo_ = std::make_unique<TypeArgumentTarget>();       break; }
    case 0x48: { this->targetInfo_ = std::make_unique<TypeArgumentTarget>();       break; }
    case 0x49: { this->targetInfo_ = std::make_unique<TypeArgumentTarget>();       break; }
    case 0x4a: { this->targetInfo_ = std::make_unique<TypeArgumentTarget>();       break; }
    case 0x4b: { this->targetInfo_ = std::make_unique<TypeArgumentTarget>();       break; }
    default:   { return -1;                                                               }
    }

    if (this->targetInfo_->load(addr, pos, cp, info) != 0) {
        return -1;
    }

    this->typePath_ = std::make_unique<TypePath>();
    if (this->typePath_->load(addr, pos, cp, info) != 0) {
        return -1;
    }

    this->typeIndex_ = readUInt16(addr, pos);

    const uint16_t numElementTypePairs = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numElementTypePairs; ++i) {
        auto elementValuePair = std::make_unique<ElementValuePair>();
        if (elementValuePair->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->elementValuePairs_.push_back(std::move(elementValuePair));
    }

    return 0;
}

int RuntimeVisibleTypeAnnotationsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numAnnotations = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numAnnotations; ++i) {
        auto typeAnnotation = std::make_unique<TypeAnnotation>();
        if (typeAnnotation->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->annotations_.push_back(std::move(typeAnnotation));
    }

    return 0;
}

int RuntimeInvisibleTypeAnnotationsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numAnnotations = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numAnnotations; ++i) {
        auto typeAnnotation = std::make_unique<TypeAnnotation>();
        if (typeAnnotation->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->annotations_.push_back(std::move(typeAnnotation));
    }

    return 0;
}

int AnnotationDefaultAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->defaultValue_ = std::make_unique<ElementValue>();
    if (this->defaultValue_->load(addr, pos, cp, info) != 0) {
        return -1;
    }

    return 0;
}

int BootstrapMethod::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->bootstrapMethodRef_ = readUInt16(addr, pos);
    const uint16_t numBootstrapArguments = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numBootstrapArguments; ++i) {
        this->bootstrapArguments_.push_back(readUInt16(addr, pos));
    }

    return 0;
}


int BootstrapMethodsAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numBootstrapMethods = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numBootstrapMethods; ++i) {
        auto bootstrapMethod = std::make_unique<BootstrapMethod>();
        if (bootstrapMethod->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->bootstrapMethods_.push_back(std::move(bootstrapMethod));
    }

    return 0;
}

int Parameter::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->nameIndex_   = readUInt16(addr, pos);
    this->accessFlags_ = readUInt16(addr, pos);

    return 0;
}

int MethodParametersAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint8_t parametersCount = readUInt8(addr, pos);
    for (uint8_t i = 0; i < parametersCount; ++i) {
        auto parameter = std::make_unique<Parameter>();
        if (parameter->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->parameters_.push_back(std::move(parameter));
    }

    return 0;
}

int Requires::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->requiresIndex_         = readUInt16(addr, pos);
    this->requiresFlags_         = readUInt16(addr, pos);
    this->requiresVersionIndex_  = readUInt16(addr, pos);

    return 0;
}

int Exports::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->exportsIndex_ = readUInt16(addr, pos);
    this->exportsFlags_ = readUInt16(addr, pos);

    const uint16_t exportsCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < exportsCount; ++i) {
        this->exportsToIndex_.push_back(readUInt16(addr, pos));
    }

    return 0;
}

int Opens::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->opensIndex_ = readUInt16(addr, pos);
    this->opensFlags_ = readUInt16(addr, pos);

    const uint16_t opensCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < opensCount; ++i) {
        this->opensToIndex_.push_back(readUInt16(addr, pos));
    }

    return 0;
}

int Provides::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->providesIndex_ = readUInt16(addr, pos);

    const uint16_t providesWithCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < providesWithCount; ++i) {
        this->providesWithIndex_.push_back(readUInt16(addr, pos));
    }

    return 0;
}

int ModuleAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->moduleNameIndex_     = readUInt16(addr, pos);
    this->moduleFlags_         = readUInt16(addr, pos);
    this->moduleVersionIndex_  = readUInt16(addr, pos);

    const uint16_t requiresCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < requiresCount; ++i) {
        auto requires = std::make_unique<Requires>();
        if (requires->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->requires_.push_back(std::move(requires));
    }

    const uint16_t exportsCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < exportsCount; ++i) {
        auto exports = std::make_unique<Exports>();
        if (exports->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->exports_.push_back(std::move(exports));
    }

    const uint16_t opensCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < opensCount; ++i) {
        auto opens = std::make_unique<Opens>();
        if (opens->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->opens_.push_back(std::move(opens));
    }

    const uint16_t usesCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < usesCount; ++i) {
        this->usesIndex_.push_back(readUInt16(addr, pos));
    }

    const uint16_t providesCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < providesCount; ++i) {
        auto provides = std::make_unique<Provides>();
        if (provides->load(addr, pos, cp, info) != 0) {
            return -1;
        }
        this->provides_.push_back(std::move(provides));
    }

    return 0;
}

int ModulePackagesAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t packageCount = readUInt16(addr, pos);
    for (uint16_t i = 0; i < packageCount; ++i) {
        this->packageIndex_.push_back(readUInt16(addr, pos));
    }

    return 0;
}

int ModuleMainClassAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->mainClassIndex_ = readUInt16(addr, pos);

    return 0;
}

int NestHostAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    this->hostClassIndex_ = readUInt16(addr, pos);

    return 0;
}

int NestMembersAttribute::load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept {
    const uint16_t numberOfClasses = readUInt16(addr, pos);
    for (uint16_t i = 0; i < numberOfClasses; ++i) {
        this->classes_.push_back(readUInt16(addr, pos));
    }

    return 0;
}

std::string AttributeInfo::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"attribute_name_index\":%hu,"
        "\"attribute_length\":%hu",
        this->getAttributeNameIndex(),
        this->getAttributeLength()
    );

    switch (this->getAttributeType()) {
    case AttributeType::Synthetic:
    case AttributeType::Deprecated: {
        break;
    }
    default: {
        ss << fmt(",%s", this->info_->toString().c_str());
    }
    }

    return ss.str();
}

std::string ConstantValueAttribute::toString() const noexcept {
    return fmt(
        "\"constant_value_index\":%hu",
        this->getConstantValueIndex()
    );
}

std::string Exception::toString() const noexcept {
    return fmt(
        "{"
        "\"start_pc\":%hu,"
        "\"end_pc\":%hu,"
        "\"handler_pc\":%hu,"
        "\"catch_type\":%hu"
        "}",
        this->getStartPC(),
        this->getEndPC(),
        this->getHandlerPC(),
        this->getCatchType()
    );
}

std::string CodeAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"max_stack\":%hu,"
        "\"max_locals\":%hu,"
        "\"code_length\":%u,",
        this->getMaxStack(),
        this->getMaxLocals(),
        this->getCodeLength()
    );

    ss << "\"code\":[";
    for (uint32_t i = 0; i < this->getCodeLength(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hhu", this->getCodeAt(i));
    }
    ss << "],";

    ss << fmt("\"exception_table_length\":%hu,", this->getExceptionTableLength());
    ss << "\"exception_table\":[";
    for (uint16_t i = 0; i < this->getExceptionTableLength(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getExceptionAt(i)->toString() << "}";
    }
    ss << "],";

    ss << fmt("\"attributes_count\":%hu,", this->getAttributesCount());

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

std::string VerificationTypeInfo::toString() const noexcept {
    std::ostringstream ss;
    ss << fmt("\"tag\":%hhu", this->getTag());

    switch (this->getTag()) {
    case ITEM_Object: {
        ss << fmt(",\"cpool_index\":%hu", this->getMoreInfo());
        break;
    }
    case ITEM_Uninitialized: {
        ss << fmt(",\"offset\":%hu", this->getMoreInfo());
        break;
    }
    default: {
        break;
    }
    } 

    return ss.str();
}

std::string SameFrame::toString() const noexcept {
    return "";
}

std::string SameLocals1StackItemFrame::toString() const noexcept {
    return this->stack_->toString();
}

std::string SameLocals1StackItemFrameExtended::toString() const noexcept {
    return fmt(
        "\"offset_delta\":%hu,"
        "%s",
        this->getOffsetDelta(),
        this->stack_->toString().c_str()
    );
}

std::string ChopFrame::toString() const noexcept {
    return fmt("\"offset_delta\":%hu,", this->getOffsetDelta());
}

std::string SameFrameExtended::toString() const noexcept {
    return fmt("\"offset_delta\":%hu,", this->getOffsetDelta());
}

std::string AppendFrame::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"offset_delta\":%hu,", this->getOffsetDelta());

    ss << "\"locals\":[";
    for (uint8_t i = 0; i < this->getLocalsSize(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        const VerificationTypeInfo* info = this->getLocalAt(i);
        ss << "{" << info->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string FullFrame::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"offset_delta\":%hu,", this->getOffsetDelta());

    ss << "\"locals\":[";
    for (uint16_t i = 0; i < this->getNumberOfLocals(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getLocalAt(i)->toString() << "}";
    }
    ss << "],\"stack\":[";
    for (uint16_t i = 0; i < this->getNumberOfStackItems(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getStackAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string StackMapFrame::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"frame_type\":%hhu,"
        "%s",
        this->getFrameType(),
        this->getFrame()->toString().c_str()
    );

    return ss.str();
}

std::string StackMapTableAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"number_of_entries\":%hu,", this->getNumberOfEntries());

    ss << "\"stack_frame_entries\":[";
    for (uint16_t i = 0; i < this->getNumberOfEntries(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getEntryAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string ExceptionsAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"number_of_exceptions\":%hu,", this->getNumberOfExceptions());

    ss << "\"exception_index_table\":[";
    for (uint16_t i = 0; i < this->getNumberOfExceptions(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hu", this->getExceptionIndexAt(i));
    }
    ss << "]";

    return ss.str();
}

std::string Class::toString() const noexcept {
    return fmt(
        "\"inner_class_info_index\":%hu,"
        "\"outer_class_info_index\":%hu,"
        "\"inner_name_index\":%hu,"
        "\"inner_class_access_flags\":%hu",
        this->getInnerClassInfoIndex(),
        this->getOuterClassInfoIndex(),
        this->getInnerNameIndex(),
        this->getInnerClassAccessFlags()
    );
}

std::string InnerClassesAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"number_of_classes\":%hu,", this->getNumberOfClasses());

    ss << "\"classes\":[";
    for (uint16_t i = 0; i < this->getNumberOfClasses(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getClassAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string EnclosingMethodAttribute::toString() const noexcept {
    return fmt(
        "\"class_index\":%hu,"
        "\"method_index\":%hu", 
        this->getClassIndex(),
        this->getMethodIndex()
    );
}

std::string SyntheticAttribute::toString() const noexcept {
    return "";
}

std::string SignatureAttribute::toString() const noexcept {
    return fmt("\"signature_index\":%hu", this->getSignagureIndex());
}

std::string SourceFileAttribute::toString() const noexcept {
    return fmt("\"source_file_index\":%hu", this->getSourceFileIndex());
}

std::string SourceDebugExtensionAttribute::toString() const noexcept {
    std::ostringstream ss;
    ss << "\"debug_extension\":[";
    for (std::size_t i = 0; i < this->getDebugExtension().size(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hhu", this->debugExtension_[i]);
    }
    ss << "]";

    return ss.str();
}

std::string LineNumber::toString() const noexcept {
    return fmt(
        "\"start_pc\":%hu,"
        "\"line_number\":%hu",
        this->getStartPC(),
        this->getLineNumber()
    );
}

std::string LineNumberTableAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"line_number_table_length\":%hu,", this->getLineNumberTableLength());

    ss << "\"line_number_table\":[";
    for (uint16_t i = 0; i < this->getLineNumberTableLength(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getLineNumberAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string LocalVariable::toString() const noexcept {
    return fmt(
        "\"start_pc\":%hu,"
        "\"length\":%hu,"
        "\"name_index\":%hu,"
        "\"descriptor_index\":%hu,"
        "\"index\":%hu",
        this->getStartPC(),
        this->getLength(),
        this->getNameIndex(),
        this->getDescriptorIndex(),
        this->getIndex()
    );
}

std::string LocalVariableTableAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"local_variable_table_length\":%hu,", this->getLocalVariableTableLength());
    
    ss << "\"local_variable_table\":[";
    for (uint16_t i = 0; i < this->getLocalVariableTableLength(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getLocalVariableAt(i)->toString() << "}";
    }
    ss << "]";


    return ss.str();
}

std::string LocalVariableType::toString() const noexcept {
    return fmt(
        "\"start_pc\":%hu,"
        "\"length\":%hu,"
        "\"name_index\":%hu,"
        "\"signature_index\":%hu,"
        "\"index\":%hu",
        this->getStartPC(),
        this->getLength(),
        this->getNameIndex(),
        this->getSignatureIndex(),
        this->getIndex()
    );
}

std::string LocalVariableTypeTableAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"local_variable_type_table_length\":%hu,", this->getLocalVariableTypeTableLength());
    
    ss << "\"local_variable_type_table\":[";
    for (uint16_t i = 0; i < this->getLocalVariableTypeTableLength(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getLocalVariableTypeAt(i)->toString() << "}";
    }
    ss << "]";


    return ss.str();
}

std::string DeprecatedAttribute::toString() const noexcept {
    return "";
}

std::string EnumConstValue::toString() const noexcept {
    return fmt(
        "\"type_name_index\":%hu,"
        "\"const_name_index\":%hu",
        this->getTypeNameIndex(),
        this->getConstNameIndex()
    );
}

std::string ArrayValue::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"num_values\":%hu,", this->getNumValues());

    ss << "\"values\":[";
    for (uint16_t i = 0; i < this->getNumValues(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << this->getValueAt(i)->toString();
    }
    ss << "]";
    
    return ss.str();
}

std::string ElementValue::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"tag\":%hu,", this->getTag());

    switch (this->getTag()) {
    case 'B':
    case 'C':
    case 'D':
    case 'F':
    case 'I':
    case 'J':
    case 'S':
    case 'Z':
    case 's': {
        ss << fmt("\"const_value_index\":%lu", (uint64_t)(this->getValue()));
        break;
    }
    case 'e': {
        ss << fmt("\"enum_const_value\":{%s}", ((EnumConstValue*)(this->getValue()))->toString().c_str());
        break;
    }
    case 'c': {
        ss << fmt("\"class_info_index\":%lu", (uint64_t)(this->getValue()));
        break;
    }
    case '@': {
        ss << fmt("\"annotation\":{%s}", ((Annotation*)(this->getValue()))->toString().c_str());
        break;
    }
    case '[': {
        ss << fmt("\"array_value\":{%s}", ((ArrayValue*)(this->value_))->toString().c_str());
        break;
    }
    default: {
        break;
    }
    }

    return ss.str();
}

std::string ElementValuePair::toString() const noexcept {
    return fmt(
        "\"element_name_index\":%hu,"
        "\"element_value\":{%s}",
        this->getElementNameIndex(),
        this->getValue()->toString().c_str()
    );
}

std::string Annotation::toString() const noexcept {
    std::ostringstream ss;
    
    ss << fmt(
        "\"type_index\":%hu,"
        "\"num_element_value_pairs\":%hu,", 
        this->getTypeIndex(),
        this->getNumElementValuePairs()
    );

    ss << "\"element_value_pairs\":[";
    for (uint16_t i = 0; i < this->getNumElementValuePairs(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getElementValuePairAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string TypeParameterTarget::toString() const noexcept {
    return fmt("\"type_parameter_index\":%hhu", this->getTypeParameterIndex());
}

std::string SupertypeTarget::toString() const noexcept {
    return fmt("\"supertype_index\":%hu", this->getSupertypeIndex());
}

std::string TypeParameterBoundTarget::toString() const noexcept {
    return fmt(
        "\"type_parameter_index\":%hhu,"
        "\"bound_index\":%hhu",
        this->getTypeParameterIndex(),
        this->getBoundIndex()
    );
}

std::string EmptyTarget::toString() const noexcept {
    return "";
}

std::string FormalParameterTarget::toString() const noexcept {
    return fmt("\"formal_parameter_index\":%hhu", this->getFormalParameterIndex());
}

std::string ThrowsTarget::toString() const noexcept {
    return fmt("\"throws_type_index\":%hu", this->getThrowsTypeIndex());
}

std::string Localvar::toString() const noexcept {
    return fmt(
        "\"start_pc\":%hu,"
        "\"length\":%hu,"
        "\"index\":%hu",
        this->getStartPC(),
        this->getLength(),
        this->getIndex()
    );
}

std::string LocalvarTarget::toString() const noexcept {
    std::ostringstream ss;

    ss << "\"table\":[";
    for (uint16_t i = 0; i < this->getTableLength(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getEntryAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string CatchTarget::toString() const noexcept {
    return fmt("\"exception_table_index\":%hu", this->getExceptionTableIndex());
}

std::string OffsetTarget::toString() const noexcept {
    return fmt("\"offset\":%hu", this->getOffset());
}

std::string TypeArgumentTarget::toString() const noexcept {
    return fmt(
        "\"offset\":%hu,"
        "\"type_argument_index\":%hhu",
        this->getOffset(),
        this->getTypeArgumentIndex()
    );
}

std::string Path::toString() const noexcept {
    return fmt(
        "\"type_path_kind\":%hhu,"
        "\"type_argument_index\":%hhu",
        this->getTypePathKind(),
        this->getTypeArgumentIndex()
    );
}

std::string TypePath::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"path_length\":%hhu,", this->getPathLength());
    ss << "\"path\":[";
    for (uint8_t i = 0; i < this->getPathLength(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getPathAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string TypeAnnotation::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"target_type\":%hhu,"
        "\"target_info\":{%s},"
        "\"type_path\":{%s},"
        "\"type_index\":%hu,"
        "\"num_element_vaue_pairs\":%hu,",
        this->getTargetType(),
        this->getTargetInfo()->toString(),
        this->getTypePath()->toString(),
        this->getTypeIndex(),
        this->getNumElementValuePairs()
    );

    ss << "\"element_value_pairs\":[";
    for (uint16_t i = 0; i < this->getNumElementValuePairs(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getElementValuePairAt(i)->toString() << "}";
    }
    ss << "]";
    
    return ss.str(); 
}

std::string RuntimeVisibleAnnotationsAttribute::toString() const noexcept {
    return "";
}

std::string RuntimeInvisibleAnnotationsAttribute::toString() const noexcept {
    return "";
}

std::string RuntimeVisibleParameterAnnotationsAttribute::toString() const noexcept {
    return "";
}

std::string RuntimeInvisibleParameterAnnotationsAttribute::toString() const noexcept {
    return "";
}

std::string RuntimeVisibleTypeAnnotationsAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"num_annotations\":%hu,", this->getNumAnnotations());

    ss << "\"annotations\":[";
    for (uint16_t i = 0; i < this->getNumAnnotations(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getAnnotationAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string RuntimeInvisibleTypeAnnotationsAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"num_annotations\":%hu,", this->getNumAnnotations());

    ss << "\"annotations\":[";
    for (uint16_t i = 0; i < this->getNumAnnotations(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getAnnotationAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string AnnotationDefaultAttribute::toString() const noexcept {
    return fmt("\"element_value\":{%s}", this->getDefaultValue()->toString());
}

std::string BootstrapMethod::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"bootstrap_method_ref\":%hu," 
        "\"num_bootstrap_arguments\":%hu,", 
        this->getBootstrapMethodRef(),
        this->getNumBootstrapArguments()
    ); 

    ss << "\"bootstrap_arguments\":[";
    for (uint16_t i = 0; i < this->getNumBootstrapArguments(); ++i) {
        if (i != 0) {   
            ss << ",";
        }
        ss << fmt("%hu", this->getBootstrapArgumentAt(i));
    }
    ss << "]";

    return ss.str();
}

std::string BootstrapMethodsAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"num_bootstrap_methods\":%hu,", this->getNumBootstrapMethods());
    ss << "\"bootstrap_methods\"[";

    for (uint16_t i = 0; i < this->getNumBootstrapMethods(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getBootstrapMethodAt(i)->toString() << "}"; 
    }
    ss << "]";
    
    return ss.str();
}

std::string Parameter::toString() const noexcept {
    return fmt(
        "\"name_index\":%hu,"
        "\"access_flags\":%hu",
        this->getNameIndex(),
        this->getAccessFlags()
    );
}

std::string MethodParametersAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"parameters_count\":%hhu,", this->getParametersCount());

    ss << "\"parameters\":[";
    for (uint8_t i = 0; i < this->getParametersCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getParameterAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string Requires::toString() const noexcept {
    return fmt(
        "\"requires_index:\":%hu,"
        "\"requires_flags:\":%hu,"
        "\"requires_version_index:\":%hu,",
        this->getRequiresIndex(),
        this->getRequiresFlags(),
        this->getRequiresVersionIndex()
    );
}  

std::string Exports::toString() const noexcept {
    std::ostringstream ss;
    ss << fmt(
        "\"exports_index:\":%hu,"
        "\"exports_flags:\":%hu,"
        "\"exports_to_count:\":%hu,",
        this->getExportsIndex(),
        this->getExportsFlags(),
        this->getExportsToCount()
    );

    ss << "\"exports_to_index\":[";
    for (uint16_t i = 0; i < this->getExportsToCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hu", this->getExportsToIndexAt(i));
    }
    ss << "]";

    return ss.str();
}  

std::string Opens::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"opens_index:\":%hu,"
        "\"opens_flags:\":%hu,"
        "\"opens_to_count:\":%hu,",
        this->getOpensIndex(),
        this->getOpensFlags(),
        this->getOpensToCount()
    );

    ss << "\"opens_to_index\":[";
    for (uint16_t i = 0; i < this->getOpensToCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hu", this->getOpensToIndexAt(i));
    }

    return ss.str();
}  

std::string Provides::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"provides_index\":%hu,"
        "\"provides_with_count\":%hu,",
        this->getProvidesIndex(),
        this->getProvidesWithCount()
    );

    ss << "\"provides_with_index\":[";
    for (uint16_t i = 0; i < this->getProvidesWithCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hu", this->getProvidesWithIndexAt(i));
    } 

    ss << "]";

    return ss.str();
}

std::string ModuleAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt(
        "\"module_name_index\":%hu,"
        "\"module_flags\":%hu,"
        "\"module_version_index\":%hu,",
        this->getModuleNameIndex(),
        this->getModuleFlags(),
        this->getModuleVersionIndex()
    );

    ss << fmt("\"requires_count\":%hu,", this->getRequiresCount());
    ss << "\"requires\":[";
    for (uint16_t i = 0; i < this->getRequiresCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getRequiresAt(i)->toString() << "}";
    }
    ss << "],";

    ss << fmt("\"exports_count\":%hu,", this->getExportsCount());
    ss << "\"exports\":[";
    for (uint16_t i = 0; i < this->getExportsCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getExportsAt(i)->toString() << "}";
    }
    ss << "],";

    ss << fmt("\"opens_count\":%hu,", this->getOpensCount());
    ss << "\"opens\":[";
    for (uint16_t i = 0; i < this->getOpensCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getOpensAt(i)->toString() << "}";
    }
    ss << "],";

    ss << fmt("\"uses_count\":%hu,", this->getUsesCount());
    ss << "\"uses_index\":[";
    for (uint16_t i = 0; i < this->getUsesCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hu", this->getUsesIndexAt(i));
    }
    ss << "],";

    ss << fmt("\"provides_count\":%hu,", this->getProvidesCount());
    ss << "\"provides\":[";
    for (uint16_t i = 0; i < this->getProvidesCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << "{" << this->getProvidesAt(i)->toString() << "}";
    }
    ss << "]";

    return ss.str();
}

std::string ModulePackagesAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"package_count\":%hu,", this->getPackageCount());
    ss << "\"package_index\":[";
    for (uint16_t i = 0; i < this->getPackageCount(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hu", this->getPackageIndexAt(i));
    }
    ss << "]";

    return ss.str();
}

std::string ModuleMainClassAttribute::toString() const noexcept {
    return fmt("\"main_class_index\":%hu", this->getMainClassIndex());
}

std::string NestHostAttribute::toString() const noexcept {
    return fmt("\"host_class_index\":%hu", this->getHostClassIndex());
}

std::string NestMembersAttribute::toString() const noexcept {
    std::ostringstream ss;

    ss << fmt("\"number_of_classes\":%hu,", this->getNumberOfClasses());

    ss << "\"classes\":[";
    for (uint16_t i = 0; i < this->getNumberOfClasses(); ++i) {
        if (i != 0) {
            ss << ",";
        }
        ss << fmt("%hu", this->getClassIndexAt(i));
    }
    ss << "]";

    return ss.str();
}
