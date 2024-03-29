// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: p1.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_p1_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_p1_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_p1_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_p1_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[4]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_p1_2eproto;
namespace p1 {
class MSG_TEST;
class MSG_TESTDefaultTypeInternal;
extern MSG_TESTDefaultTypeInternal _MSG_TEST_default_instance_;
class MSG_TEST_RES;
class MSG_TEST_RESDefaultTypeInternal;
extern MSG_TEST_RESDefaultTypeInternal _MSG_TEST_RES_default_instance_;
class REG_TO_LEADER;
class REG_TO_LEADERDefaultTypeInternal;
extern REG_TO_LEADERDefaultTypeInternal _REG_TO_LEADER_default_instance_;
class REG_TO_LEADER_RES;
class REG_TO_LEADER_RESDefaultTypeInternal;
extern REG_TO_LEADER_RESDefaultTypeInternal _REG_TO_LEADER_RES_default_instance_;
}  // namespace p1
PROTOBUF_NAMESPACE_OPEN
template<> ::p1::MSG_TEST* Arena::CreateMaybeMessage<::p1::MSG_TEST>(Arena*);
template<> ::p1::MSG_TEST_RES* Arena::CreateMaybeMessage<::p1::MSG_TEST_RES>(Arena*);
template<> ::p1::REG_TO_LEADER* Arena::CreateMaybeMessage<::p1::REG_TO_LEADER>(Arena*);
template<> ::p1::REG_TO_LEADER_RES* Arena::CreateMaybeMessage<::p1::REG_TO_LEADER_RES>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace p1 {

// ===================================================================

class MSG_TEST :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:p1.MSG_TEST) */ {
 public:
  MSG_TEST();
  virtual ~MSG_TEST();

  MSG_TEST(const MSG_TEST& from);
  MSG_TEST(MSG_TEST&& from) noexcept
    : MSG_TEST() {
    *this = ::std::move(from);
  }

  inline MSG_TEST& operator=(const MSG_TEST& from) {
    CopyFrom(from);
    return *this;
  }
  inline MSG_TEST& operator=(MSG_TEST&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const MSG_TEST& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const MSG_TEST* internal_default_instance() {
    return reinterpret_cast<const MSG_TEST*>(
               &_MSG_TEST_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(MSG_TEST& a, MSG_TEST& b) {
    a.Swap(&b);
  }
  inline void Swap(MSG_TEST* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline MSG_TEST* New() const final {
    return CreateMaybeMessage<MSG_TEST>(nullptr);
  }

  MSG_TEST* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<MSG_TEST>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const MSG_TEST& from);
  void MergeFrom(const MSG_TEST& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(MSG_TEST* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "p1.MSG_TEST";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_p1_2eproto);
    return ::descriptor_table_p1_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
  };
  // string name = 1;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // @@protoc_insertion_point(class_scope:p1.MSG_TEST)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_p1_2eproto;
};
// -------------------------------------------------------------------

class MSG_TEST_RES :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:p1.MSG_TEST_RES) */ {
 public:
  MSG_TEST_RES();
  virtual ~MSG_TEST_RES();

  MSG_TEST_RES(const MSG_TEST_RES& from);
  MSG_TEST_RES(MSG_TEST_RES&& from) noexcept
    : MSG_TEST_RES() {
    *this = ::std::move(from);
  }

  inline MSG_TEST_RES& operator=(const MSG_TEST_RES& from) {
    CopyFrom(from);
    return *this;
  }
  inline MSG_TEST_RES& operator=(MSG_TEST_RES&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const MSG_TEST_RES& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const MSG_TEST_RES* internal_default_instance() {
    return reinterpret_cast<const MSG_TEST_RES*>(
               &_MSG_TEST_RES_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(MSG_TEST_RES& a, MSG_TEST_RES& b) {
    a.Swap(&b);
  }
  inline void Swap(MSG_TEST_RES* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline MSG_TEST_RES* New() const final {
    return CreateMaybeMessage<MSG_TEST_RES>(nullptr);
  }

  MSG_TEST_RES* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<MSG_TEST_RES>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const MSG_TEST_RES& from);
  void MergeFrom(const MSG_TEST_RES& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(MSG_TEST_RES* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "p1.MSG_TEST_RES";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_p1_2eproto);
    return ::descriptor_table_p1_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameResFieldNumber = 1,
    kA1FieldNumber = 3,
    kA2FieldNumber = 4,
  };
  // string name_res = 1;
  void clear_name_res();
  const std::string& name_res() const;
  void set_name_res(const std::string& value);
  void set_name_res(std::string&& value);
  void set_name_res(const char* value);
  void set_name_res(const char* value, size_t size);
  std::string* mutable_name_res();
  std::string* release_name_res();
  void set_allocated_name_res(std::string* name_res);
  private:
  const std::string& _internal_name_res() const;
  void _internal_set_name_res(const std::string& value);
  std::string* _internal_mutable_name_res();
  public:

  // int32 a1 = 3;
  void clear_a1();
  ::PROTOBUF_NAMESPACE_ID::int32 a1() const;
  void set_a1(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_a1() const;
  void _internal_set_a1(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 a2 = 4;
  void clear_a2();
  ::PROTOBUF_NAMESPACE_ID::int32 a2() const;
  void set_a2(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_a2() const;
  void _internal_set_a2(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:p1.MSG_TEST_RES)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_res_;
  ::PROTOBUF_NAMESPACE_ID::int32 a1_;
  ::PROTOBUF_NAMESPACE_ID::int32 a2_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_p1_2eproto;
};
// -------------------------------------------------------------------

class REG_TO_LEADER :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:p1.REG_TO_LEADER) */ {
 public:
  REG_TO_LEADER();
  virtual ~REG_TO_LEADER();

  REG_TO_LEADER(const REG_TO_LEADER& from);
  REG_TO_LEADER(REG_TO_LEADER&& from) noexcept
    : REG_TO_LEADER() {
    *this = ::std::move(from);
  }

  inline REG_TO_LEADER& operator=(const REG_TO_LEADER& from) {
    CopyFrom(from);
    return *this;
  }
  inline REG_TO_LEADER& operator=(REG_TO_LEADER&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const REG_TO_LEADER& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const REG_TO_LEADER* internal_default_instance() {
    return reinterpret_cast<const REG_TO_LEADER*>(
               &_REG_TO_LEADER_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(REG_TO_LEADER& a, REG_TO_LEADER& b) {
    a.Swap(&b);
  }
  inline void Swap(REG_TO_LEADER* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline REG_TO_LEADER* New() const final {
    return CreateMaybeMessage<REG_TO_LEADER>(nullptr);
  }

  REG_TO_LEADER* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<REG_TO_LEADER>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const REG_TO_LEADER& from);
  void MergeFrom(const REG_TO_LEADER& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(REG_TO_LEADER* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "p1.REG_TO_LEADER";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_p1_2eproto);
    return ::descriptor_table_p1_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
    kPacketIdFieldNumber = 2,
  };
  // string name = 1;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // int32 packet_id = 2;
  void clear_packet_id();
  ::PROTOBUF_NAMESPACE_ID::int32 packet_id() const;
  void set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_packet_id() const;
  void _internal_set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:p1.REG_TO_LEADER)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::int32 packet_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_p1_2eproto;
};
// -------------------------------------------------------------------

class REG_TO_LEADER_RES :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:p1.REG_TO_LEADER_RES) */ {
 public:
  REG_TO_LEADER_RES();
  virtual ~REG_TO_LEADER_RES();

  REG_TO_LEADER_RES(const REG_TO_LEADER_RES& from);
  REG_TO_LEADER_RES(REG_TO_LEADER_RES&& from) noexcept
    : REG_TO_LEADER_RES() {
    *this = ::std::move(from);
  }

  inline REG_TO_LEADER_RES& operator=(const REG_TO_LEADER_RES& from) {
    CopyFrom(from);
    return *this;
  }
  inline REG_TO_LEADER_RES& operator=(REG_TO_LEADER_RES&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const REG_TO_LEADER_RES& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const REG_TO_LEADER_RES* internal_default_instance() {
    return reinterpret_cast<const REG_TO_LEADER_RES*>(
               &_REG_TO_LEADER_RES_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(REG_TO_LEADER_RES& a, REG_TO_LEADER_RES& b) {
    a.Swap(&b);
  }
  inline void Swap(REG_TO_LEADER_RES* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline REG_TO_LEADER_RES* New() const final {
    return CreateMaybeMessage<REG_TO_LEADER_RES>(nullptr);
  }

  REG_TO_LEADER_RES* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<REG_TO_LEADER_RES>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const REG_TO_LEADER_RES& from);
  void MergeFrom(const REG_TO_LEADER_RES& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(REG_TO_LEADER_RES* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "p1.REG_TO_LEADER_RES";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_p1_2eproto);
    return ::descriptor_table_p1_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameResFieldNumber = 1,
    kPacketIdFieldNumber = 2,
    kA1FieldNumber = 3,
    kA2FieldNumber = 4,
  };
  // string name_res = 1;
  void clear_name_res();
  const std::string& name_res() const;
  void set_name_res(const std::string& value);
  void set_name_res(std::string&& value);
  void set_name_res(const char* value);
  void set_name_res(const char* value, size_t size);
  std::string* mutable_name_res();
  std::string* release_name_res();
  void set_allocated_name_res(std::string* name_res);
  private:
  const std::string& _internal_name_res() const;
  void _internal_set_name_res(const std::string& value);
  std::string* _internal_mutable_name_res();
  public:

  // int32 packet_id = 2;
  void clear_packet_id();
  ::PROTOBUF_NAMESPACE_ID::int32 packet_id() const;
  void set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_packet_id() const;
  void _internal_set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 a1 = 3;
  void clear_a1();
  ::PROTOBUF_NAMESPACE_ID::int32 a1() const;
  void set_a1(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_a1() const;
  void _internal_set_a1(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 a2 = 4;
  void clear_a2();
  ::PROTOBUF_NAMESPACE_ID::int32 a2() const;
  void set_a2(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_a2() const;
  void _internal_set_a2(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:p1.REG_TO_LEADER_RES)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_res_;
  ::PROTOBUF_NAMESPACE_ID::int32 packet_id_;
  ::PROTOBUF_NAMESPACE_ID::int32 a1_;
  ::PROTOBUF_NAMESPACE_ID::int32 a2_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_p1_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// MSG_TEST

// string name = 1;
inline void MSG_TEST::clear_name() {
  name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& MSG_TEST::name() const {
  // @@protoc_insertion_point(field_get:p1.MSG_TEST.name)
  return _internal_name();
}
inline void MSG_TEST::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:p1.MSG_TEST.name)
}
inline std::string* MSG_TEST::mutable_name() {
  // @@protoc_insertion_point(field_mutable:p1.MSG_TEST.name)
  return _internal_mutable_name();
}
inline const std::string& MSG_TEST::_internal_name() const {
  return name_.GetNoArena();
}
inline void MSG_TEST::_internal_set_name(const std::string& value) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void MSG_TEST::set_name(std::string&& value) {
  
  name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:p1.MSG_TEST.name)
}
inline void MSG_TEST::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:p1.MSG_TEST.name)
}
inline void MSG_TEST::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:p1.MSG_TEST.name)
}
inline std::string* MSG_TEST::_internal_mutable_name() {
  
  return name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* MSG_TEST::release_name() {
  // @@protoc_insertion_point(field_release:p1.MSG_TEST.name)
  
  return name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void MSG_TEST::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:p1.MSG_TEST.name)
}

// -------------------------------------------------------------------

// MSG_TEST_RES

// string name_res = 1;
inline void MSG_TEST_RES::clear_name_res() {
  name_res_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& MSG_TEST_RES::name_res() const {
  // @@protoc_insertion_point(field_get:p1.MSG_TEST_RES.name_res)
  return _internal_name_res();
}
inline void MSG_TEST_RES::set_name_res(const std::string& value) {
  _internal_set_name_res(value);
  // @@protoc_insertion_point(field_set:p1.MSG_TEST_RES.name_res)
}
inline std::string* MSG_TEST_RES::mutable_name_res() {
  // @@protoc_insertion_point(field_mutable:p1.MSG_TEST_RES.name_res)
  return _internal_mutable_name_res();
}
inline const std::string& MSG_TEST_RES::_internal_name_res() const {
  return name_res_.GetNoArena();
}
inline void MSG_TEST_RES::_internal_set_name_res(const std::string& value) {
  
  name_res_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void MSG_TEST_RES::set_name_res(std::string&& value) {
  
  name_res_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:p1.MSG_TEST_RES.name_res)
}
inline void MSG_TEST_RES::set_name_res(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_res_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:p1.MSG_TEST_RES.name_res)
}
inline void MSG_TEST_RES::set_name_res(const char* value, size_t size) {
  
  name_res_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:p1.MSG_TEST_RES.name_res)
}
inline std::string* MSG_TEST_RES::_internal_mutable_name_res() {
  
  return name_res_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* MSG_TEST_RES::release_name_res() {
  // @@protoc_insertion_point(field_release:p1.MSG_TEST_RES.name_res)
  
  return name_res_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void MSG_TEST_RES::set_allocated_name_res(std::string* name_res) {
  if (name_res != nullptr) {
    
  } else {
    
  }
  name_res_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name_res);
  // @@protoc_insertion_point(field_set_allocated:p1.MSG_TEST_RES.name_res)
}

// int32 a1 = 3;
inline void MSG_TEST_RES::clear_a1() {
  a1_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 MSG_TEST_RES::_internal_a1() const {
  return a1_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 MSG_TEST_RES::a1() const {
  // @@protoc_insertion_point(field_get:p1.MSG_TEST_RES.a1)
  return _internal_a1();
}
inline void MSG_TEST_RES::_internal_set_a1(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  a1_ = value;
}
inline void MSG_TEST_RES::set_a1(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_a1(value);
  // @@protoc_insertion_point(field_set:p1.MSG_TEST_RES.a1)
}

// int32 a2 = 4;
inline void MSG_TEST_RES::clear_a2() {
  a2_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 MSG_TEST_RES::_internal_a2() const {
  return a2_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 MSG_TEST_RES::a2() const {
  // @@protoc_insertion_point(field_get:p1.MSG_TEST_RES.a2)
  return _internal_a2();
}
inline void MSG_TEST_RES::_internal_set_a2(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  a2_ = value;
}
inline void MSG_TEST_RES::set_a2(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_a2(value);
  // @@protoc_insertion_point(field_set:p1.MSG_TEST_RES.a2)
}

// -------------------------------------------------------------------

// REG_TO_LEADER

// int32 packet_id = 2;
inline void REG_TO_LEADER::clear_packet_id() {
  packet_id_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER::_internal_packet_id() const {
  return packet_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER::packet_id() const {
  // @@protoc_insertion_point(field_get:p1.REG_TO_LEADER.packet_id)
  return _internal_packet_id();
}
inline void REG_TO_LEADER::_internal_set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  packet_id_ = value;
}
inline void REG_TO_LEADER::set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_packet_id(value);
  // @@protoc_insertion_point(field_set:p1.REG_TO_LEADER.packet_id)
}

// string name = 1;
inline void REG_TO_LEADER::clear_name() {
  name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& REG_TO_LEADER::name() const {
  // @@protoc_insertion_point(field_get:p1.REG_TO_LEADER.name)
  return _internal_name();
}
inline void REG_TO_LEADER::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:p1.REG_TO_LEADER.name)
}
inline std::string* REG_TO_LEADER::mutable_name() {
  // @@protoc_insertion_point(field_mutable:p1.REG_TO_LEADER.name)
  return _internal_mutable_name();
}
inline const std::string& REG_TO_LEADER::_internal_name() const {
  return name_.GetNoArena();
}
inline void REG_TO_LEADER::_internal_set_name(const std::string& value) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void REG_TO_LEADER::set_name(std::string&& value) {
  
  name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:p1.REG_TO_LEADER.name)
}
inline void REG_TO_LEADER::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:p1.REG_TO_LEADER.name)
}
inline void REG_TO_LEADER::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:p1.REG_TO_LEADER.name)
}
inline std::string* REG_TO_LEADER::_internal_mutable_name() {
  
  return name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* REG_TO_LEADER::release_name() {
  // @@protoc_insertion_point(field_release:p1.REG_TO_LEADER.name)
  
  return name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void REG_TO_LEADER::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:p1.REG_TO_LEADER.name)
}

// -------------------------------------------------------------------

// REG_TO_LEADER_RES

// string name_res = 1;
inline void REG_TO_LEADER_RES::clear_name_res() {
  name_res_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& REG_TO_LEADER_RES::name_res() const {
  // @@protoc_insertion_point(field_get:p1.REG_TO_LEADER_RES.name_res)
  return _internal_name_res();
}
inline void REG_TO_LEADER_RES::set_name_res(const std::string& value) {
  _internal_set_name_res(value);
  // @@protoc_insertion_point(field_set:p1.REG_TO_LEADER_RES.name_res)
}
inline std::string* REG_TO_LEADER_RES::mutable_name_res() {
  // @@protoc_insertion_point(field_mutable:p1.REG_TO_LEADER_RES.name_res)
  return _internal_mutable_name_res();
}
inline const std::string& REG_TO_LEADER_RES::_internal_name_res() const {
  return name_res_.GetNoArena();
}
inline void REG_TO_LEADER_RES::_internal_set_name_res(const std::string& value) {
  
  name_res_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void REG_TO_LEADER_RES::set_name_res(std::string&& value) {
  
  name_res_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:p1.REG_TO_LEADER_RES.name_res)
}
inline void REG_TO_LEADER_RES::set_name_res(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_res_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:p1.REG_TO_LEADER_RES.name_res)
}
inline void REG_TO_LEADER_RES::set_name_res(const char* value, size_t size) {
  
  name_res_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:p1.REG_TO_LEADER_RES.name_res)
}
inline std::string* REG_TO_LEADER_RES::_internal_mutable_name_res() {
  
  return name_res_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* REG_TO_LEADER_RES::release_name_res() {
  // @@protoc_insertion_point(field_release:p1.REG_TO_LEADER_RES.name_res)
  
  return name_res_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void REG_TO_LEADER_RES::set_allocated_name_res(std::string* name_res) {
  if (name_res != nullptr) {
    
  } else {
    
  }
  name_res_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name_res);
  // @@protoc_insertion_point(field_set_allocated:p1.REG_TO_LEADER_RES.name_res)
}

// int32 packet_id = 2;
inline void REG_TO_LEADER_RES::clear_packet_id() {
  packet_id_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER_RES::_internal_packet_id() const {
  return packet_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER_RES::packet_id() const {
  // @@protoc_insertion_point(field_get:p1.REG_TO_LEADER_RES.packet_id)
  return _internal_packet_id();
}
inline void REG_TO_LEADER_RES::_internal_set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  packet_id_ = value;
}
inline void REG_TO_LEADER_RES::set_packet_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_packet_id(value);
  // @@protoc_insertion_point(field_set:p1.REG_TO_LEADER_RES.packet_id)
}

// int32 a1 = 3;
inline void REG_TO_LEADER_RES::clear_a1() {
  a1_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER_RES::_internal_a1() const {
  return a1_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER_RES::a1() const {
  // @@protoc_insertion_point(field_get:p1.REG_TO_LEADER_RES.a1)
  return _internal_a1();
}
inline void REG_TO_LEADER_RES::_internal_set_a1(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  a1_ = value;
}
inline void REG_TO_LEADER_RES::set_a1(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_a1(value);
  // @@protoc_insertion_point(field_set:p1.REG_TO_LEADER_RES.a1)
}

// int32 a2 = 4;
inline void REG_TO_LEADER_RES::clear_a2() {
  a2_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER_RES::_internal_a2() const {
  return a2_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 REG_TO_LEADER_RES::a2() const {
  // @@protoc_insertion_point(field_get:p1.REG_TO_LEADER_RES.a2)
  return _internal_a2();
}
inline void REG_TO_LEADER_RES::_internal_set_a2(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  a2_ = value;
}
inline void REG_TO_LEADER_RES::set_a2(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_a2(value);
  // @@protoc_insertion_point(field_set:p1.REG_TO_LEADER_RES.a2)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace p1

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_p1_2eproto
