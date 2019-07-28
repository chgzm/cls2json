#ifndef ATTRIBUTEINFO_H
#define ATTRIBUTEINFO_H

#include "CPInfo.h"
#include <cstdint>
#include <vector>
#include <memory>

enum class AttributeType : uint8_t {
    ConstantValue,
    Code,
    StackMapTable,
    Exceptions,
    InnerClasses, 
    EnclosingMethod,
    Synthetic,
    Signature,
    SourceFile,
    SourceDebugExtension,
    LineNumberTable,
    LocalVariableTable,
    LocalVariableTypeTable,
    Deprecated,
    RuntimeVisibleAnnotations,
    RuntimeInvisibleAnnotations,
    RuntimeVisibleParameterAnnotations,
    RuntimeInvisibleParameterAnnotations,
    RuntimeVisibleTypeAnnotations,
    RuntimeInvisibleTypeAnnotations,
    AnnotationDefault,
    BootstrapMethods,
    MethodParameters,
    Module,
    ModulePackages,
    ModuleMainClass,
    NestHost,
    NestMembers,
};

class AttributeInfoImpl;

using ConstantPool = std::vector<std::unique_ptr<CPInfo>>;

class AttributeInfo {
public:
    AttributeInfo()  = default;
    ~AttributeInfo() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp) noexcept;
    std::string toString() const noexcept;

    inline AttributeType getAttributeType() const noexcept {
        return this->type_;
    }

    inline uint16_t getAttributeNameIndex() const noexcept {
        return this->attributeNameIndex_;
    }

    inline uint32_t getAttributeLength() const noexcept {
        return this->attributeLength_;
    }

    inline const std::unique_ptr<AttributeInfoImpl>& getInfo() const noexcept {
        return this->info_;
    }

    inline const std::string& getAttributeName() const noexcept {
        return this->attributeName_;
    }

private:
    AttributeType                      type_;
    uint16_t                           attributeNameIndex_;
    uint32_t                           attributeLength_;
    std::unique_ptr<AttributeInfoImpl> info_;
    std::string                        attributeName_;
};

class AttributeInfoImpl {
public:
    virtual ~AttributeInfoImpl() = default;
    virtual int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept = 0;
    virtual std::string toString() const noexcept = 0;
};

class ConstantValueAttribute : public AttributeInfoImpl {
public:
    ConstantValueAttribute()  = default;
    ~ConstantValueAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getConstantValueIndex() const noexcept {
        return this->constantValueIndex_;
    }

private:
    uint16_t constantValueIndex_;
};

class Exception {
public:
    Exception()  = default;
    ~Exception() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getStartPC() const noexcept {
        return this->startPC_;
    }

    inline uint16_t getEndPC() const noexcept {
        return this->endPC_;
    }

    inline uint16_t getHandlerPC() const noexcept {
        return this->handlerPC_;
    }

    inline uint16_t getCatchType() const noexcept {
        return this->catchType_;
    }

private:
    uint16_t startPC_;
    uint16_t endPC_;
    uint16_t handlerPC_;
    uint16_t catchType_;
};

class CodeAttribute : public AttributeInfoImpl {
public:
    CodeAttribute()  = default;
    ~CodeAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getMaxStack() const noexcept {
        return this->maxStack_;
    }

    inline uint16_t getMaxLocals() const noexcept {
        return this->maxLocals_;
    }

    inline uint32_t getCodeLength() const noexcept {
        return this->code_.size();
    }

    inline const std::vector<uint8_t>& getCode() const noexcept {
        return this->code_;
    }

    inline uint8_t getCodeAt(uint32_t index) const noexcept {
        return this->code_[index];
    }

    inline uint16_t getExceptionTableLength() const noexcept {
        return this->exceptionTable_.size();
    }

    inline const std::vector<std::unique_ptr<Exception>>& getExceptionTable() const noexcept {
        return this->exceptionTable_;
    }

    inline const Exception* getExceptionAt(uint16_t index) const noexcept {
        return this->exceptionTable_[index].get();
    }

    inline uint16_t getAttributesCount() const noexcept {
        return this->attributes_.size();
    }

    inline const std::vector<std::unique_ptr<AttributeInfo>>& getAttributes() const noexcept {
        return this->attributes_;
    }

    inline const AttributeInfo* getAttributeAt(uint16_t index) const noexcept {
        return this->attributes_[index].get();
    }

private:
    uint16_t                                    maxStack_;
    uint16_t                                    maxLocals_;
    std::vector<uint8_t>                        code_;
    std::vector<std::unique_ptr<Exception>>     exceptionTable_;
    std::vector<std::unique_ptr<AttributeInfo>> attributes_;
};

class VerificationTypeInfo {
public:
    VerificationTypeInfo()  = default;
    ~VerificationTypeInfo() = default;

    static constexpr uint8_t ITEM_Top               = 0;
    static constexpr uint8_t ITEM_Integer           = 1;
    static constexpr uint8_t ITEM_Float             = 2;
    static constexpr uint8_t ITEM_Double            = 3;
    static constexpr uint8_t ITEM_Long              = 4;
    static constexpr uint8_t ITEM_Null              = 5;
    static constexpr uint8_t ITEM_UninitializedThis = 6;
    static constexpr uint8_t ITEM_Object            = 7;
    static constexpr uint8_t ITEM_Uninitialized     = 8;

    int load(const uint8_t* addr, std::size_t& pos) noexcept;
    std::string toString() const noexcept;

    inline uint8_t getTag() const noexcept {
        return this->tag_;
    }

    inline uint16_t getMoreInfo() const noexcept {
        return this->moreInfo_;
    }

private:
    uint8_t  tag_;  
    uint16_t moreInfo_;
};

class FrameImpl {
public:
    virtual ~FrameImpl() = default;
    virtual int load(const uint8_t* addr, std::size_t& pos) noexcept = 0;
    virtual std::string toString() const noexcept = 0;
};

class SameFrame : public FrameImpl {
public:
    SameFrame()  = default;
    ~SameFrame() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;
};

class SameLocals1StackItemFrame : public FrameImpl {
public:
    SameLocals1StackItemFrame()  = default;
    ~SameLocals1StackItemFrame() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;
    
private:
    std::unique_ptr<VerificationTypeInfo> stack_;
};

class SameLocals1StackItemFrameExtended : public FrameImpl {
public:
    SameLocals1StackItemFrameExtended()  = default;
    ~SameLocals1StackItemFrameExtended() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getOffsetDelta() const noexcept {
        return this->offsetDelta_;
    }

private:
    uint16_t                              offsetDelta_;
    std::unique_ptr<VerificationTypeInfo> stack_;
};

class ChopFrame : public FrameImpl {
public:
    ChopFrame()   = default;
    ~ChopFrame()  = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getOffsetDelta() const noexcept {
        return this->offsetDelta_;
    }

private:
    uint16_t offsetDelta_;
};

class SameFrameExtended : public FrameImpl {
public:
    SameFrameExtended()  = default;
    ~SameFrameExtended() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getOffsetDelta() const noexcept {
        return this->offsetDelta_;
    }

private:
    uint16_t offsetDelta_;
};

class AppendFrame : public FrameImpl {
public:
    AppendFrame(uint8_t frameType) : frameType_(frameType) {}
    ~AppendFrame() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getOffsetDelta() const noexcept {
        return this->offsetDelta_;
    }

    inline uint8_t getLocalsSize() const noexcept {
        return this->locals_.size();
    }

    inline const VerificationTypeInfo* getLocalAt(uint16_t index) const noexcept {
        return this->locals_[index].get();
    }

private:
    uint8_t                                            frameType_;
    uint16_t                                           offsetDelta_;
    std::vector<std::unique_ptr<VerificationTypeInfo>> locals_;
};

class FullFrame : public FrameImpl {
public:
    FullFrame()  = default;
    ~FullFrame() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getOffsetDelta() const noexcept {
        return this->offsetDelta_;
    }

    inline uint16_t getNumberOfLocals() const noexcept {
        return this->locals_.size();
    }

    inline const VerificationTypeInfo* getLocalAt(uint16_t index) const noexcept {
        return this->locals_[index].get();
    }

    inline uint16_t getNumberOfStackItems() const noexcept {
        return this->stack_.size();
    }

    inline const VerificationTypeInfo* getStackAt(uint16_t index) const noexcept {
        return this->stack_[index].get();
    }

private:
    uint16_t                                           offsetDelta_;
    std::vector<std::unique_ptr<VerificationTypeInfo>> locals_;
    std::vector<std::unique_ptr<VerificationTypeInfo>> stack_;
};

class StackMapFrame {
public:
    StackMapFrame()  = default;
    ~StackMapFrame() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept;
    std::string toString() const noexcept;

    inline uint8_t getFrameType() const noexcept {
        return this->frameType_;
    }

    inline const FrameImpl* getFrame() const noexcept {
        return this->frame_.get();
    }

private:
    uint8_t                    frameType_;
    std::unique_ptr<FrameImpl> frame_;
};


class StackMapTableAttribute : public AttributeInfoImpl {
public:
    StackMapTableAttribute()  = default;
    ~StackMapTableAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNumberOfEntries() const noexcept {
        return this->entries_.size();
    }

    inline const std::vector<std::unique_ptr<StackMapFrame>>& getEntries() const noexcept {
        return this->entries_;
    }

    inline const StackMapFrame* getEntryAt(uint16_t index) const noexcept {
        return this->entries_[index].get();
    }  

private:
    std::vector<std::unique_ptr<StackMapFrame>> entries_;
};

class ExceptionsAttribute : public AttributeInfoImpl {
public:
    ExceptionsAttribute()  = default;
    ~ExceptionsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline const std::vector<uint16_t>& getExceptionIndexTable() const noexcept {
        return this->exceptionIndexTable_;
    }

    inline uint16_t getNumberOfExceptions() const noexcept {
        return this->exceptionIndexTable_.size();
    }

    inline uint16_t getExceptionIndexAt(uint16_t index) const noexcept {
        return this->exceptionIndexTable_[index];
    }

private:
    std::vector<uint16_t> exceptionIndexTable_;
};

class Class {
public:
    Class()  = default;
    ~Class() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getInnerClassInfoIndex() const noexcept {
        return this->innerClassInfoIndex_;
    }

    inline uint16_t getOuterClassInfoIndex() const noexcept {
        return this->outerClassInfoIndex_;
    }

    inline uint16_t getInnerNameIndex() const noexcept {
        return this->innerNameIndex_;
    }

    inline uint16_t getInnerClassAccessFlags() const noexcept {
        return this->innerClassAccessFlags_;
    }

private:
    uint16_t innerClassInfoIndex_;
    uint16_t outerClassInfoIndex_;
    uint16_t innerNameIndex_;
    uint16_t innerClassAccessFlags_;
};

class InnerClassesAttribute : public AttributeInfoImpl {
public:
    InnerClassesAttribute()  = default;
    ~InnerClassesAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    static constexpr uint16_t ACC_PUBLIC       = 0x0001;
    static constexpr uint16_t ACC_PRIVATE      = 0x0002;
    static constexpr uint16_t ACC_PROTECTED    = 0x0004;
    static constexpr uint16_t ACC_STATIC       = 0x0008;
    static constexpr uint16_t ACC_FINAL        = 0x0010;
    static constexpr uint16_t ACC_INTERFACE    = 0x0200;
    static constexpr uint16_t ACC_ABSTRACT     = 0x0400;
    static constexpr uint16_t ACC_STRICT       = 0x0800;
    static constexpr uint16_t ACC_SYSTHETIC    = 0x1000;
    static constexpr uint16_t ACC_ANNOTATION   = 0x2000;
    static constexpr uint16_t ACC_ENUM         = 0x4000;

    inline uint16_t getNumberOfClasses() const noexcept {
        return this->classes_.size();
    }

    inline const std::vector<std::unique_ptr<Class>>& getClasses() const noexcept {
        return this->classes_;
    }

    inline const Class* getClassAt(uint16_t index) const noexcept {
        return this->classes_[index].get();
    }

private:
    std::vector<std::unique_ptr<Class>> classes_;
};

class EnclosingMethodAttribute : public AttributeInfoImpl {
public:
    EnclosingMethodAttribute()  = default;
    ~EnclosingMethodAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getClassIndex() const noexcept {
        return this->classIndex_;
    }

    inline uint16_t getMethodIndex() const noexcept {
        return this->methodIndex_;
    }

private:
    uint16_t classIndex_;
    uint16_t methodIndex_;
};

class SyntheticAttribute : public AttributeInfoImpl {
public:
    SyntheticAttribute()  = default;
    ~SyntheticAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;
};

class SignatureAttribute : public AttributeInfoImpl {
public:
    SignatureAttribute()  = default;
    ~SignatureAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getSignagureIndex() const noexcept {
        return this->signatureIndex_;
    }

private:
    uint16_t signatureIndex_;
};

class SourceFileAttribute : public AttributeInfoImpl {
public:
    SourceFileAttribute()  = default;
    ~SourceFileAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getSourceFileIndex() const noexcept {
        return this->sourceFileIndex;
    }

private:
    uint16_t sourceFileIndex;
};

class SourceDebugExtensionAttribute : public AttributeInfoImpl {
public:
    SourceDebugExtensionAttribute()  = default;
    ~SourceDebugExtensionAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept override;

    inline const std::vector<uint8_t>& getDebugExtension() const noexcept {
        return this->debugExtension_;
    }

    inline const std::string& getDebugExtensionStr() const noexcept {
        return this->debugExtensionStr_;
    }

private:
    std::vector<uint8_t> debugExtension_;
    std::string          debugExtensionStr_;
};

class LineNumber {
public:
    LineNumber()  = default;
    ~LineNumber() = default;

    int load(const uint8_t* addr, std::size_t& pos) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getStartPC() const noexcept {
        return this->startPC_;
    }

    inline uint16_t getLineNumber() const noexcept {
        return this->lineNumber_;
    }

private:
    uint16_t startPC_;
    uint16_t lineNumber_;
};

class LineNumberTableAttribute : public AttributeInfoImpl {
public:
    LineNumberTableAttribute()  = default;
    ~LineNumberTableAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept override;

    inline uint16_t getLineNumberTableLength() const noexcept {
        return this->lineNumberTable_.size();
    }

    inline const std::vector<std::unique_ptr<LineNumber>>& getLineNumberTable() const noexcept {
        return this->lineNumberTable_;
    }

    inline const LineNumber* getLineNumberAt(uint16_t index) const noexcept {
        return this->lineNumberTable_[index].get();
    }

private:
    std::vector<std::unique_ptr<LineNumber>> lineNumberTable_;
};

class LocalVariable {
public:
    LocalVariable()  = default;
    ~LocalVariable() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getStartPC() const noexcept {
        return this->startPC_;
    }

    inline uint16_t getLength() const noexcept {
        return this->length_;
    }

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

    inline uint16_t getDescriptorIndex() const noexcept {
        return this->descriptorIndex_;
    }

    inline uint16_t getIndex() const noexcept {
        return this->index_;
    }

private:
    uint16_t startPC_;
    uint16_t length_;
    uint16_t nameIndex_;
    uint16_t descriptorIndex_;
    uint16_t index_;
};

class LocalVariableTableAttribute : public AttributeInfoImpl {
public:
    LocalVariableTableAttribute()  = default;
    ~LocalVariableTableAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getLocalVariableTableLength() const noexcept {
        return this->localVariableTable_.size();
    }

    inline const std::vector<std::unique_ptr<LocalVariable>>& getLocalVariableTable() const noexcept {
        return this->localVariableTable_;
    }

    inline const LocalVariable* getLocalVariableAt(uint16_t index) const noexcept {
        return this->localVariableTable_[index].get();
    }

private:
    std::vector<std::unique_ptr<LocalVariable>> localVariableTable_;
};

class LocalVariableType {
public:
    LocalVariableType()  = default;
    ~LocalVariableType() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getStartPC() const noexcept {
        return this->startPC_;
    }

    inline uint16_t getLength() const noexcept {
        return this->length_;
    }

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

    inline uint16_t getSignatureIndex() const noexcept {
        return this->signatureIndex_;
    }

    inline uint16_t getIndex() const noexcept {
        return this->index_;
    }

private:
    uint16_t startPC_;
    uint16_t length_;
    uint16_t nameIndex_;
    uint16_t signatureIndex_;
    uint16_t index_;
};

class LocalVariableTypeTableAttribute : public AttributeInfoImpl {
public:
    LocalVariableTypeTableAttribute()  = default;
    ~LocalVariableTypeTableAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getLocalVariableTypeTableLength() const noexcept {
        return this->localVariableTypeTable_.size();
    }

    inline const std::vector<std::unique_ptr<LocalVariableType>>& getLocalVariableTypeTable() const noexcept {
        return this->localVariableTypeTable_;
    }

    inline const LocalVariableType* getLocalVariableTypeAt(uint16_t index) const noexcept {
        return this->localVariableTypeTable_[index].get();
    }

private:
    std::vector<std::unique_ptr<LocalVariableType>> localVariableTypeTable_;
};

class DeprecatedAttribute : public AttributeInfoImpl {
public:
    DeprecatedAttribute()  = default;
    ~DeprecatedAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;
};

class EnumConstValue {
public:
    EnumConstValue()  = default;
    ~EnumConstValue() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getTypeNameIndex() const noexcept {
        return this->typeNameIndex_;
    }

    inline uint16_t getConstNameIndex() const noexcept {
        return this->constNameIndex_;
    }

private:
    uint16_t typeNameIndex_;
    uint16_t constNameIndex_;
};

class ElementValue;

class ArrayValue {
public:
    ArrayValue()  = default;
    ~ArrayValue() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getNumValues() const noexcept {
        return this->values_.size();
    }

    inline const std::vector<std::unique_ptr<ElementValue>>& getValues() const noexcept {
        return this->values_;
    }

    inline const ElementValue* getValueAt(uint16_t index) const noexcept {
        return this->values_[index].get();
    }

private:
    std::vector<std::unique_ptr<ElementValue>> values_;
};

class Annotation;

#if 0
union Value {
    uint16_t                         constValueIndex_;
    std::unique_ptr<EnumConstValue>  enumConstValue_;
    uint16_t                         classInfoIndex_;
    std::unique_ptr<Annotation>      annotationValue;
};
#endif

class ElementValue {
public:
    ElementValue() = default;
    ~ElementValue();

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint8_t getTag() const noexcept {
        return this->tag_;
    }

    inline const void* getValue() const noexcept {
        return this->value_;
    }

private:
    uint8_t tag_;
    void*   value_;
};

class ElementValuePair {
public:
    ElementValuePair()  = default;
    ~ElementValuePair() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getElementNameIndex() const noexcept {
        return this->elementNameIndex_;
    }

    inline const ElementValue* getValue() const noexcept {
        return this->value_.get();
    }

private:
    uint16_t                      elementNameIndex_;
    std::unique_ptr<ElementValue> value_;
};

class Annotation {
public:
    Annotation()  = default;
    ~Annotation() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getTypeIndex() const noexcept {
        return this->typeIndex_;
    }

    inline uint16_t getNumElementValuePairs() const noexcept {
        return this->elementValuePairs_.size();
    }

    inline const std::vector<std::unique_ptr<ElementValuePair>>& getelementvaluepairs() const noexcept {
        return this->elementValuePairs_;
    }

    inline const ElementValuePair* getElementValuePairAt(uint16_t index) const noexcept {
        this->elementValuePairs_[index].get();
    }

private:
    uint16_t                                       typeIndex_;
    std::vector<std::unique_ptr<ElementValuePair>> elementValuePairs_;
};

class RuntimeVisibleAnnotationsAttribute : public AttributeInfoImpl {
public:
    RuntimeVisibleAnnotationsAttribute()  = default;
    ~RuntimeVisibleAnnotationsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNumAnnotations() const noexcept {
        return this->annotations_.size();
    }

    inline const std::vector<std::unique_ptr<Annotation>>& getAnnotations() const noexcept {
        return this->annotations_;
    }

private:
    std::vector<std::unique_ptr<Annotation>> annotations_;
};

class RuntimeInvisibleAnnotationsAttribute : public AttributeInfoImpl {
public:
    RuntimeInvisibleAnnotationsAttribute()  = default;
    ~RuntimeInvisibleAnnotationsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNumAnnotations() const noexcept {
        return this->annotations_.size();
    }

    inline const std::vector<std::unique_ptr<Annotation>>& getAnnotations() const noexcept {
        return this->annotations_;
    }

private:
    std::vector<std::unique_ptr<Annotation>> annotations_;
};

class ParameterAnnotation {
public:
    ParameterAnnotation()  = default;
    ~ParameterAnnotation() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getNumAnnotations() const noexcept {
        return this->annotations_.size();
    }

    inline const std::vector<std::unique_ptr<Annotation>>& getAnnotations() const noexcept {
        return this->annotations_;
    }

private:
    std::vector<std::unique_ptr<Annotation>> annotations_;
};

class RuntimeVisibleParameterAnnotationsAttribute : public AttributeInfoImpl {
public:
    RuntimeVisibleParameterAnnotationsAttribute()  = default;
    ~RuntimeVisibleParameterAnnotationsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint8_t getNumParameters() const noexcept {
        return this->parameterAnnotations_.size();
    }

    inline const std::vector<std::unique_ptr<ParameterAnnotation>>& getParameterAnnotaions() const noexcept {
        return this->parameterAnnotations_;
    }

private:
    std::vector<std::unique_ptr<ParameterAnnotation>> parameterAnnotations_;
};

class RuntimeInvisibleParameterAnnotationsAttribute : public AttributeInfoImpl {
public:
    RuntimeInvisibleParameterAnnotationsAttribute()  = default;
    ~RuntimeInvisibleParameterAnnotationsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint8_t getNumParameters() const noexcept {
        return this->parameterAnnotations_.size();
    }

    inline const std::vector<std::unique_ptr<ParameterAnnotation>>& getParameterAnnotaions() const noexcept {
        return this->parameterAnnotations_;
    }

private:
    std::vector<std::unique_ptr<ParameterAnnotation>> parameterAnnotations_;
};

class TargetImpl {
public:
    virtual ~TargetImpl() = default;
    virtual int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept = 0;
    virtual std::string toString() const noexcept = 0;
};

class TypeParameterTarget : public TargetImpl {
public:
    TypeParameterTarget()  = default;
    ~TypeParameterTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint8_t getTypeParameterIndex() const noexcept {
        return this->typeParameterIndex_;
    }

private:
    uint8_t typeParameterIndex_;
};

class SupertypeTarget : public TargetImpl {
public:
    SupertypeTarget()  = default;
    ~SupertypeTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getSupertypeIndex() const noexcept {
        return this->supertypeIndex_;
    }

private:
    uint16_t supertypeIndex_;
};

class TypeParameterBoundTarget : public TargetImpl {
public:
    TypeParameterBoundTarget()  = default;
    ~TypeParameterBoundTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint8_t getTypeParameterIndex() const noexcept {
        return this->typeParameterIndex_;
    }

    inline uint8_t getBoundIndex() const noexcept {
        return this->boundIndex_;
    }

private:
    uint8_t typeParameterIndex_;
    uint8_t boundIndex_;
};

class EmptyTarget : public TargetImpl {
public:
    EmptyTarget()  = default;
    ~EmptyTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;
};

class FormalParameterTarget : public TargetImpl {
public:
    FormalParameterTarget()  = default;
    ~FormalParameterTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint8_t getFormalParameterIndex() const noexcept {
        return this->formalParameterIndex_;
    }

private:
    uint8_t formalParameterIndex_;
};

class ThrowsTarget : public TargetImpl {
public:
    ThrowsTarget()  = default;
    ~ThrowsTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getThrowsTypeIndex() const noexcept {
        return this->throwsTypeIndex_;
    }

private:
    uint16_t throwsTypeIndex_;
};

class Localvar {
public:
    Localvar()  = default;
    ~Localvar() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getStartPC() const noexcept {
        return this->startPC_;
    }

    inline uint16_t getLength() const noexcept {
        return this->length_;
    }

    inline uint16_t getIndex() const noexcept {
        return this->index_;
    }

private:
    uint16_t startPC_;
    uint16_t length_;
    uint16_t index_;
};

class LocalvarTarget : public TargetImpl {
public:
    LocalvarTarget()  = default;
    ~LocalvarTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getTableLength() const noexcept {
        return this->table_.size();
    }

    inline const std::vector<std::unique_ptr<Localvar>>& getTable() const noexcept {
        return this->table_;
    }

    inline const Localvar* getEntryAt(uint16_t index) const noexcept {
        return this->table_[index].get();
    }

private:
    std::vector<std::unique_ptr<Localvar>> table_;
};

class CatchTarget : public TargetImpl {
public:
    CatchTarget()  = default;
    ~CatchTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getExceptionTableIndex() const noexcept {
        return this->exceptionTableIndex_;
    }

private:
    uint16_t exceptionTableIndex_;
};

class OffsetTarget : public TargetImpl {
public:
    OffsetTarget()  = default;
    ~OffsetTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getOffset() const noexcept {
        return this->offset_;
    }

private:
    uint16_t offset_;
};

class TypeArgumentTarget : public TargetImpl {
public:
    TypeArgumentTarget()  = default;
    ~TypeArgumentTarget() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getOffset() const noexcept {
        return this->offset_;
    }

    inline uint8_t getTypeArgumentIndex() const noexcept {
        return this->typeArgumentIndex_;
    }

private:
    uint16_t offset_;
    uint8_t  typeArgumentIndex_;
};

class Path {
public:
    Path()  = default;
    ~Path() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint8_t getTypePathKind() const noexcept {
        return this->typePathKind_;
    }

    inline uint8_t getTypeArgumentIndex() const noexcept {
        return this->typeArgumentIndex_;
    }

private:
    uint8_t typePathKind_;
    uint8_t typeArgumentIndex_;
};

class TypePath {
public:
    TypePath()  = default;
    ~TypePath() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint8_t getPathLength() const noexcept {
        return this->path_.size();
    }

    inline const std::vector<std::unique_ptr<Path>>& getPath() const noexcept {
        return this->path_;
    }

    inline const Path* getPathAt(uint8_t index) const noexcept {
        return this->path_[index].get();
    }

private:
    std::vector<std::unique_ptr<Path>> path_;
};

class TypeAnnotation {
public:
    TypeAnnotation()  = default;
    ~TypeAnnotation() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint8_t getTargetType() const noexcept {
        return this->targetType_;
    }

    inline const TargetImpl* getTargetInfo() const noexcept {
        return this->targetInfo_.get();
    }

    inline const TypePath* getTypePath() const noexcept {
        return this->typePath_.get();
    }

    inline uint16_t getTypeIndex() const noexcept {
        return this->typeIndex_;
    }

    inline uint16_t getNumElementValuePairs() const noexcept {
        return this->elementValuePairs_.size();
    }

    inline const ElementValuePair* getElementValuePairAt(uint16_t index) const noexcept {
        return this->elementValuePairs_[index].get();
    }

private:
    uint8_t                                        targetType_;
    std::unique_ptr<TargetImpl>                    targetInfo_;
    std::unique_ptr<TypePath>                      typePath_;
    uint16_t                                       typeIndex_;
    std::vector<std::unique_ptr<ElementValuePair>> elementValuePairs_;
};

class RuntimeVisibleTypeAnnotationsAttribute : public AttributeInfoImpl {
public:
    RuntimeVisibleTypeAnnotationsAttribute()  = default;
    ~RuntimeVisibleTypeAnnotationsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNumAnnotations() const noexcept {
        return this->annotations_.size();
    }

    inline const std::vector<std::unique_ptr<TypeAnnotation>>& getAnnotations() const noexcept {
        return this->annotations_;
    }

    inline const TypeAnnotation* getAnnotationAt(uint16_t index) const noexcept {
        return this->annotations_[index].get();
    }

private:
    std::vector<std::unique_ptr<TypeAnnotation>> annotations_;
};

class RuntimeInvisibleTypeAnnotationsAttribute : public AttributeInfoImpl {
public:
    RuntimeInvisibleTypeAnnotationsAttribute()  = default;
    ~RuntimeInvisibleTypeAnnotationsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNumAnnotations() const noexcept {
        return this->annotations_.size();
    }

    inline const std::vector<std::unique_ptr<TypeAnnotation>>& getAnnotations() const noexcept {
        return this->annotations_;
    }

    inline const TypeAnnotation* getAnnotationAt(uint16_t index) const noexcept {
        return this->annotations_[index].get();
    }

private:
    std::vector<std::unique_ptr<TypeAnnotation>> annotations_;
};

class AnnotationDefaultAttribute : public AttributeInfoImpl {
public:
    AnnotationDefaultAttribute()  = default;
    ~AnnotationDefaultAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline const ElementValue* getDefaultValue() const noexcept {
        return this->defaultValue_.get();
    }

private:
    std::unique_ptr<ElementValue> defaultValue_;
};

class BootstrapMethod {
public:
    BootstrapMethod()  = default;
    ~BootstrapMethod() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getBootstrapMethodRef() const noexcept {
        return this->bootstrapMethodRef_;
    }

    inline uint16_t getNumBootstrapArguments() const noexcept {
        return this->bootstrapArguments_.size();
    }

    inline const std::vector<uint16_t>& getBootstrapArguments() const noexcept {
        return this->bootstrapArguments_;
    }

    inline uint16_t getBootstrapArgumentAt(uint16_t index) const noexcept {
        return this->bootstrapArguments_[index];
    }

private:
    uint16_t              bootstrapMethodRef_;
    std::vector<uint16_t> bootstrapArguments_;
};

class BootstrapMethodsAttribute : public AttributeInfoImpl {
public:
    BootstrapMethodsAttribute()  = default;
    ~BootstrapMethodsAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNumBootstrapMethods() const noexcept {
        return this->bootstrapMethods_.size();
    }

    inline const std::vector<std::unique_ptr<BootstrapMethod>>& getBootstrapMethods() const noexcept {
        return this->bootstrapMethods_;
    }

    inline const BootstrapMethod* getBootstrapMethodAt(uint16_t index) const noexcept {
        return this->bootstrapMethods_[index].get();
    }

private:
    std::vector<std::unique_ptr<BootstrapMethod>> bootstrapMethods_;
};

class Parameter {
public:
    Parameter()  = default;
    ~Parameter() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getNameIndex() const noexcept {
        return this->nameIndex_;
    }

    inline uint16_t getAccessFlags() const noexcept {
        return this->accessFlags_;
    }

private:
    uint16_t nameIndex_;
    uint16_t accessFlags_;
};

class MethodParametersAttribute : public AttributeInfoImpl {
public:
    MethodParametersAttribute()  = default;
    ~MethodParametersAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint8_t getParametersCount() const noexcept {
        return this->parameters_.size();
    }

    inline const std::vector<std::unique_ptr<Parameter>>& getParameters() const noexcept {
        return this->parameters_;
    }

    inline const Parameter* getParameterAt(uint8_t index) const noexcept {
        return this->parameters_[index].get();
    }

private:
    std::vector<std::unique_ptr<Parameter>> parameters_;
};

class Requires {
public:
    Requires()  = default;
    ~Requires() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getRequiresIndex() const noexcept {
        return this->requiresIndex_;
    }

    inline uint16_t getRequiresFlags() const noexcept {
        return this->requiresFlags_;
    }

    inline uint16_t getRequiresVersionIndex() const noexcept {
        return this->requiresVersionIndex_;
    }

private:
    uint16_t requiresIndex_;
    uint16_t requiresFlags_;
    uint16_t requiresVersionIndex_;
};

class Exports {
public:
    Exports()  = default;
    ~Exports() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getExportsIndex() const noexcept {
        return this->exportsIndex_;
    }

    inline uint16_t getExportsFlags() const noexcept {
        return this->exportsFlags_;
    }

    inline uint16_t getExportsToCount() const noexcept {
        return this->exportsToIndex_.size();
    }

    inline const std::vector<uint16_t>& getExportsToIndex() const noexcept {
        return this->exportsToIndex_;
    }

    inline uint16_t getExportsToIndexAt(uint16_t index) const noexcept {
        return this->exportsToIndex_[index];
    }

private:
    uint16_t              exportsIndex_;
    uint16_t              exportsFlags_;
    std::vector<uint16_t> exportsToIndex_;
};

class Opens {
public:
    Opens()  = default;
    ~Opens() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getOpensIndex() const noexcept {
        return this->opensIndex_;
    }

    inline uint16_t getOpensFlags() const noexcept {
        return this->opensFlags_;
    }

    inline uint16_t getOpensToCount() const noexcept {
        return this->opensToIndex_.size();
    }

    inline const std::vector<uint16_t>& getOpensToIndex() const noexcept {
        return this->opensToIndex_;
    }

    inline uint16_t getOpensToIndexAt(uint16_t index) const noexcept {
        return this->opensToIndex_[index];
    }

private:
    uint16_t              opensIndex_;
    uint16_t              opensFlags_;
    std::vector<uint16_t> opensToIndex_;
};

class Provides {
public:
    Provides()  = default;
    ~Provides() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept;
    std::string toString() const noexcept;

    inline uint16_t getProvidesIndex() const noexcept {
        return this->providesIndex_;
    }

    inline uint16_t getProvidesWithCount() const noexcept {
        return this->providesWithIndex_.size();
    }

    inline const std::vector<uint16_t>& getProvidesWithIndex() const noexcept {
        return this->providesWithIndex_;
    }

    inline uint16_t getProvidesWithIndexAt(uint16_t index) const noexcept {
        return this->providesWithIndex_[index];
    }

private:
    uint16_t              providesIndex_;
    std::vector<uint16_t> providesWithIndex_;
};

class ModuleAttribute : public AttributeInfoImpl {
public:
    ModuleAttribute()  = default;
    ~ModuleAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getModuleNameIndex() const noexcept {
        return this->moduleNameIndex_;
    }

    inline uint16_t getModuleFlags() const noexcept {
        return this->moduleFlags_;
    }

    inline uint16_t getModuleVersionIndex() const noexcept {
        return this->moduleVersionIndex_;
    }

    inline uint16_t getRequiresCount() const noexcept {
        return this->requires_.size();
    }

    inline const std::vector<std::unique_ptr<Requires>>& getRequires() const noexcept {
        return this->requires_;
    }

    inline const Requires* getRequiresAt(uint16_t index) const noexcept {
        return this->requires_[index].get();
    }

    inline uint16_t getExportsCount() const noexcept {
        return this->exports_.size();
    }

    inline const std::vector<std::unique_ptr<Exports>>& getExports() const noexcept {
        return this->exports_;
    }

    inline const Exports* getExportsAt(uint16_t index) const noexcept {
        return this->exports_[index].get();
    }

    inline uint16_t getOpensCount() const noexcept {
        return this->opens_.size();
    }

    inline const std::vector<std::unique_ptr<Opens>>& getOpens() const noexcept {
        return this->opens_;
    }

    inline const Opens* getOpensAt(uint16_t index) const noexcept {
        return this->opens_[index].get();
    }

    inline uint16_t getUsesCount() const noexcept {
        return this->usesIndex_.size();
    }

    inline const std::vector<uint16_t>& getUsesIndex() const noexcept {
        return this->usesIndex_;
    }

    inline uint16_t getUsesIndexAt(uint16_t index) const noexcept {
        return this->usesIndex_[index];
    }

    inline uint16_t getProvidesCount() const noexcept {
        return this->provides_.size();
    }

    inline const std::vector<std::unique_ptr<Provides>>& getProvides() const noexcept {
        return this->provides_;
    }

    inline const Provides* getProvidesAt(uint16_t index) const noexcept {
        return this->provides_[index].get();
    }

private:
    uint16_t                               moduleNameIndex_;
    uint16_t                               moduleFlags_;
    uint16_t                               moduleVersionIndex_;
    std::vector<std::unique_ptr<Requires>> requires_;
    std::vector<std::unique_ptr<Exports>>  exports_;
    std::vector<std::unique_ptr<Opens>>    opens_;
    std::vector<uint16_t>                  usesIndex_;
    std::vector<std::unique_ptr<Provides>> provides_;
};

class ModulePackagesAttribute : public AttributeInfoImpl {
public:
    ModulePackagesAttribute()  = default;
    ~ModulePackagesAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getPackageCount() const noexcept {
        return this->packageIndex_.size();
    }

    inline const std::vector<uint16_t>& getPackageIndex() const noexcept {
        return this->packageIndex_;
    }

    inline uint16_t getPackageIndexAt(uint16_t index) const noexcept{
        return this->packageIndex_[index];
    }

private:
    std::vector<uint16_t> packageIndex_;
};

class ModuleMainClassAttribute : public AttributeInfoImpl {
public:
    ModuleMainClassAttribute()  = default;
    ~ModuleMainClassAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getMainClassIndex() const noexcept {
        return this->mainClassIndex_;
    }

private:
    uint16_t mainClassIndex_;
};

class NestHostAttribute : public AttributeInfoImpl {
public:
    NestHostAttribute()  = default;
    ~NestHostAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getHostClassIndex() const noexcept {
        return this->hostClassIndex_;
    }

private:
    uint16_t hostClassIndex_;
};

class NestMembersAttribute : public AttributeInfoImpl {
public:
    NestMembersAttribute()  = default;
    ~NestMembersAttribute() = default;

    int load(const uint8_t* addr, std::size_t& pos, const ConstantPool& cp, const AttributeInfo& info) noexcept override;
    std::string toString() const noexcept override;

    inline uint16_t getNumberOfClasses() const noexcept {
        return this->classes_.size();
    }

    inline const std::vector<uint16_t>& getClasses() const noexcept {
        return this->classes_;
    }

    inline uint16_t getClassIndexAt(uint16_t index) const noexcept {
        return this->classes_[index];
    }

private:
    std::vector<uint16_t> classes_;
};

#endif
