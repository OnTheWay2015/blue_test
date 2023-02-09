// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: t1.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "t1.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace t1 {

namespace {

const ::google::protobuf::Descriptor* MSG_TEST_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MSG_TEST_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_t1_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_t1_2eproto() {
  protobuf_AddDesc_t1_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "t1.proto");
  GOOGLE_CHECK(file != NULL);
  MSG_TEST_descriptor_ = file->message_type(0);
  static const int MSG_TEST_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MSG_TEST, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MSG_TEST, p1_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MSG_TEST, p2_),
  };
  MSG_TEST_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      MSG_TEST_descriptor_,
      MSG_TEST::internal_default_instance(),
      MSG_TEST_offsets_,
      -1,
      -1,
      -1,
      sizeof(MSG_TEST),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MSG_TEST, _internal_metadata_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_t1_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      MSG_TEST_descriptor_, MSG_TEST::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_t1_2eproto() {
  MSG_TEST_default_instance_.Shutdown();
  delete MSG_TEST_reflection_;
}

void protobuf_InitDefaults_t1_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::p1::protobuf_InitDefaults_p1_2eproto();
  ::p2::protobuf_InitDefaults_p2_2eproto();
  ::google::protobuf::internal::GetEmptyString();
  MSG_TEST_default_instance_.DefaultConstruct();
  MSG_TEST_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_t1_2eproto_once_);
void protobuf_InitDefaults_t1_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_t1_2eproto_once_,
                 &protobuf_InitDefaults_t1_2eproto_impl);
}
void protobuf_AddDesc_t1_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_t1_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\010t1.proto\022\002t1\032\010p1.proto\032\010p2.proto\"H\n\010MS"
    "G_TEST\022\014\n\004name\030\001 \001(\t\022\026\n\002p1\030\002 \001(\0132\n.p1.MS"
    "G_P1\022\026\n\002p2\030\003 \001(\0132\n.p2.MSG_P2b\006proto3", 116);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "t1.proto", &protobuf_RegisterTypes);
  ::p1::protobuf_AddDesc_p1_2eproto();
  ::p2::protobuf_AddDesc_p2_2eproto();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_t1_2eproto);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_t1_2eproto_once_);
void protobuf_AddDesc_t1_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_t1_2eproto_once_,
                 &protobuf_AddDesc_t1_2eproto_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_t1_2eproto {
  StaticDescriptorInitializer_t1_2eproto() {
    protobuf_AddDesc_t1_2eproto();
  }
} static_descriptor_initializer_t1_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD GOOGLE_ATTRIBUTE_NORETURN;
static void MergeFromFail(int line) {
  ::google::protobuf::internal::MergeFromFail(__FILE__, line);
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int MSG_TEST::kNameFieldNumber;
const int MSG_TEST::kP1FieldNumber;
const int MSG_TEST::kP2FieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

MSG_TEST::MSG_TEST()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_t1_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:t1.MSG_TEST)
}

void MSG_TEST::InitAsDefaultInstance() {
  p1_ = const_cast< ::p1::MSG_P1*>(
      ::p1::MSG_P1::internal_default_instance());
  p2_ = const_cast< ::p2::MSG_P2*>(
      ::p2::MSG_P2::internal_default_instance());
}

MSG_TEST::MSG_TEST(const MSG_TEST& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:t1.MSG_TEST)
}

void MSG_TEST::SharedCtor() {
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  p1_ = NULL;
  p2_ = NULL;
  _cached_size_ = 0;
}

MSG_TEST::~MSG_TEST() {
  // @@protoc_insertion_point(destructor:t1.MSG_TEST)
  SharedDtor();
}

void MSG_TEST::SharedDtor() {
  name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != &MSG_TEST_default_instance_.get()) {
    delete p1_;
    delete p2_;
  }
}

void MSG_TEST::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MSG_TEST::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MSG_TEST_descriptor_;
}

const MSG_TEST& MSG_TEST::default_instance() {
  protobuf_InitDefaults_t1_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<MSG_TEST> MSG_TEST_default_instance_;

MSG_TEST* MSG_TEST::New(::google::protobuf::Arena* arena) const {
  MSG_TEST* n = new MSG_TEST;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void MSG_TEST::Clear() {
// @@protoc_insertion_point(message_clear_start:t1.MSG_TEST)
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (GetArenaNoVirtual() == NULL && p1_ != NULL) delete p1_;
  p1_ = NULL;
  if (GetArenaNoVirtual() == NULL && p2_ != NULL) delete p2_;
  p2_ = NULL;
}

bool MSG_TEST::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:t1.MSG_TEST)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string name = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "t1.MSG_TEST.name"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_p1;
        break;
      }

      // optional .p1.MSG_P1 p1 = 2;
      case 2: {
        if (tag == 18) {
         parse_p1:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_p1()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_p2;
        break;
      }

      // optional .p2.MSG_P2 p2 = 3;
      case 3: {
        if (tag == 26) {
         parse_p2:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_p2()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:t1.MSG_TEST)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:t1.MSG_TEST)
  return false;
#undef DO_
}

void MSG_TEST::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:t1.MSG_TEST)
  // optional string name = 1;
  if (this->name().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "t1.MSG_TEST.name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->name(), output);
  }

  // optional .p1.MSG_P1 p1 = 2;
  if (this->has_p1()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, *this->p1_, output);
  }

  // optional .p2.MSG_P2 p2 = 3;
  if (this->has_p2()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, *this->p2_, output);
  }

  // @@protoc_insertion_point(serialize_end:t1.MSG_TEST)
}

::google::protobuf::uint8* MSG_TEST::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:t1.MSG_TEST)
  // optional string name = 1;
  if (this->name().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "t1.MSG_TEST.name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }

  // optional .p1.MSG_P1 p1 = 2;
  if (this->has_p1()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        2, *this->p1_, false, target);
  }

  // optional .p2.MSG_P2 p2 = 3;
  if (this->has_p2()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        3, *this->p2_, false, target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:t1.MSG_TEST)
  return target;
}

size_t MSG_TEST::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:t1.MSG_TEST)
  size_t total_size = 0;

  // optional string name = 1;
  if (this->name().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->name());
  }

  // optional .p1.MSG_P1 p1 = 2;
  if (this->has_p1()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->p1_);
  }

  // optional .p2.MSG_P2 p2 = 3;
  if (this->has_p2()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->p2_);
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MSG_TEST::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:t1.MSG_TEST)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const MSG_TEST* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const MSG_TEST>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:t1.MSG_TEST)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:t1.MSG_TEST)
    UnsafeMergeFrom(*source);
  }
}

void MSG_TEST::MergeFrom(const MSG_TEST& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:t1.MSG_TEST)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void MSG_TEST::UnsafeMergeFrom(const MSG_TEST& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.name().size() > 0) {

    name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  if (from.has_p1()) {
    mutable_p1()->::p1::MSG_P1::MergeFrom(from.p1());
  }
  if (from.has_p2()) {
    mutable_p2()->::p2::MSG_P2::MergeFrom(from.p2());
  }
}

void MSG_TEST::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:t1.MSG_TEST)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MSG_TEST::CopyFrom(const MSG_TEST& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:t1.MSG_TEST)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool MSG_TEST::IsInitialized() const {

  return true;
}

void MSG_TEST::Swap(MSG_TEST* other) {
  if (other == this) return;
  InternalSwap(other);
}
void MSG_TEST::InternalSwap(MSG_TEST* other) {
  name_.Swap(&other->name_);
  std::swap(p1_, other->p1_);
  std::swap(p2_, other->p2_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata MSG_TEST::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MSG_TEST_descriptor_;
  metadata.reflection = MSG_TEST_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// MSG_TEST

// optional string name = 1;
void MSG_TEST::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
const ::std::string& MSG_TEST::name() const {
  // @@protoc_insertion_point(field_get:t1.MSG_TEST.name)
  return name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void MSG_TEST::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:t1.MSG_TEST.name)
}
void MSG_TEST::set_name(const char* value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:t1.MSG_TEST.name)
}
void MSG_TEST::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:t1.MSG_TEST.name)
}
::std::string* MSG_TEST::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:t1.MSG_TEST.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* MSG_TEST::release_name() {
  // @@protoc_insertion_point(field_release:t1.MSG_TEST.name)
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void MSG_TEST::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:t1.MSG_TEST.name)
}

// optional .p1.MSG_P1 p1 = 2;
bool MSG_TEST::has_p1() const {
  return this != internal_default_instance() && p1_ != NULL;
}
void MSG_TEST::clear_p1() {
  if (GetArenaNoVirtual() == NULL && p1_ != NULL) delete p1_;
  p1_ = NULL;
}
const ::p1::MSG_P1& MSG_TEST::p1() const {
  // @@protoc_insertion_point(field_get:t1.MSG_TEST.p1)
  return p1_ != NULL ? *p1_
                         : *::p1::MSG_P1::internal_default_instance();
}
::p1::MSG_P1* MSG_TEST::mutable_p1() {
  
  if (p1_ == NULL) {
    p1_ = new ::p1::MSG_P1;
  }
  // @@protoc_insertion_point(field_mutable:t1.MSG_TEST.p1)
  return p1_;
}
::p1::MSG_P1* MSG_TEST::release_p1() {
  // @@protoc_insertion_point(field_release:t1.MSG_TEST.p1)
  
  ::p1::MSG_P1* temp = p1_;
  p1_ = NULL;
  return temp;
}
void MSG_TEST::set_allocated_p1(::p1::MSG_P1* p1) {
  delete p1_;
  p1_ = p1;
  if (p1) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:t1.MSG_TEST.p1)
}

// optional .p2.MSG_P2 p2 = 3;
bool MSG_TEST::has_p2() const {
  return this != internal_default_instance() && p2_ != NULL;
}
void MSG_TEST::clear_p2() {
  if (GetArenaNoVirtual() == NULL && p2_ != NULL) delete p2_;
  p2_ = NULL;
}
const ::p2::MSG_P2& MSG_TEST::p2() const {
  // @@protoc_insertion_point(field_get:t1.MSG_TEST.p2)
  return p2_ != NULL ? *p2_
                         : *::p2::MSG_P2::internal_default_instance();
}
::p2::MSG_P2* MSG_TEST::mutable_p2() {
  
  if (p2_ == NULL) {
    p2_ = new ::p2::MSG_P2;
  }
  // @@protoc_insertion_point(field_mutable:t1.MSG_TEST.p2)
  return p2_;
}
::p2::MSG_P2* MSG_TEST::release_p2() {
  // @@protoc_insertion_point(field_release:t1.MSG_TEST.p2)
  
  ::p2::MSG_P2* temp = p2_;
  p2_ = NULL;
  return temp;
}
void MSG_TEST::set_allocated_p2(::p2::MSG_P2* p2) {
  delete p2_;
  p2_ = p2;
  if (p2) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:t1.MSG_TEST.p2)
}

inline const MSG_TEST* MSG_TEST::internal_default_instance() {
  return &MSG_TEST_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace t1

// @@protoc_insertion_point(global_scope)
