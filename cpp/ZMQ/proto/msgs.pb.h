// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msgs.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_msgs_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_msgs_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012004 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_msgs_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_msgs_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_msgs_2eproto;
namespace MatlabPython {
class Numpy;
class NumpyDefaultTypeInternal;
extern NumpyDefaultTypeInternal _Numpy_default_instance_;
}  // namespace MatlabPython
PROTOBUF_NAMESPACE_OPEN
template<> ::MatlabPython::Numpy* Arena::CreateMaybeMessage<::MatlabPython::Numpy>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace MatlabPython {

enum Type : int {
  FLOAT64 = 0,
  INT32 = 1,
  UINT8 = 2,
  Type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  Type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool Type_IsValid(int value);
constexpr Type Type_MIN = FLOAT64;
constexpr Type Type_MAX = UINT8;
constexpr int Type_ARRAYSIZE = Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Type_descriptor();
template<typename T>
inline const std::string& Type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Type_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Type_descriptor(), enum_t_value);
}
inline bool Type_Parse(
    const std::string& name, Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Type>(
    Type_descriptor(), name, value);
}
// ===================================================================

class Numpy PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:MatlabPython.Numpy) */ {
 public:
  inline Numpy() : Numpy(nullptr) {};
  virtual ~Numpy();

  Numpy(const Numpy& from);
  Numpy(Numpy&& from) noexcept
    : Numpy() {
    *this = ::std::move(from);
  }

  inline Numpy& operator=(const Numpy& from) {
    CopyFrom(from);
    return *this;
  }
  inline Numpy& operator=(Numpy&& from) noexcept {
    if (GetArena() == from.GetArena()) {
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
  static const Numpy& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Numpy* internal_default_instance() {
    return reinterpret_cast<const Numpy*>(
               &_Numpy_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Numpy& a, Numpy& b) {
    a.Swap(&b);
  }
  inline void Swap(Numpy* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Numpy* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Numpy* New() const final {
    return CreateMaybeMessage<Numpy>(nullptr);
  }

  Numpy* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Numpy>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Numpy& from);
  void MergeFrom(const Numpy& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Numpy* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "MatlabPython.Numpy";
  }
  protected:
  explicit Numpy(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_msgs_2eproto);
    return ::descriptor_table_msgs_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDimsFieldNumber = 2,
    kDataFieldNumber = 4,
    kNumDimsFieldNumber = 1,
    kTypeFieldNumber = 3,
  };
  // repeated int32 dims = 2;
  int dims_size() const;
  private:
  int _internal_dims_size() const;
  public:
  void clear_dims();
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_dims(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
      _internal_dims() const;
  void _internal_add_dims(::PROTOBUF_NAMESPACE_ID::int32 value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
      _internal_mutable_dims();
  public:
  ::PROTOBUF_NAMESPACE_ID::int32 dims(int index) const;
  void set_dims(int index, ::PROTOBUF_NAMESPACE_ID::int32 value);
  void add_dims(::PROTOBUF_NAMESPACE_ID::int32 value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
      dims() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
      mutable_dims();

  // bytes data = 4;
  void clear_data();
  const std::string& data() const;
  void set_data(const std::string& value);
  void set_data(std::string&& value);
  void set_data(const char* value);
  void set_data(const void* value, size_t size);
  std::string* mutable_data();
  std::string* release_data();
  void set_allocated_data(std::string* data);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_data();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_data(
      std::string* data);
  private:
  const std::string& _internal_data() const;
  void _internal_set_data(const std::string& value);
  std::string* _internal_mutable_data();
  public:

  // int32 num_dims = 1;
  void clear_num_dims();
  ::PROTOBUF_NAMESPACE_ID::int32 num_dims() const;
  void set_num_dims(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_num_dims() const;
  void _internal_set_num_dims(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // .MatlabPython.Type type = 3;
  void clear_type();
  ::MatlabPython::Type type() const;
  void set_type(::MatlabPython::Type value);
  private:
  ::MatlabPython::Type _internal_type() const;
  void _internal_set_type(::MatlabPython::Type value);
  public:

  // @@protoc_insertion_point(class_scope:MatlabPython.Numpy)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 > dims_;
  mutable std::atomic<int> _dims_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
  ::PROTOBUF_NAMESPACE_ID::int32 num_dims_;
  int type_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_msgs_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Numpy

// int32 num_dims = 1;
inline void Numpy::clear_num_dims() {
  num_dims_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Numpy::_internal_num_dims() const {
  return num_dims_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Numpy::num_dims() const {
  // @@protoc_insertion_point(field_get:MatlabPython.Numpy.num_dims)
  return _internal_num_dims();
}
inline void Numpy::_internal_set_num_dims(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  num_dims_ = value;
}
inline void Numpy::set_num_dims(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_num_dims(value);
  // @@protoc_insertion_point(field_set:MatlabPython.Numpy.num_dims)
}

// repeated int32 dims = 2;
inline int Numpy::_internal_dims_size() const {
  return dims_.size();
}
inline int Numpy::dims_size() const {
  return _internal_dims_size();
}
inline void Numpy::clear_dims() {
  dims_.Clear();
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Numpy::_internal_dims(int index) const {
  return dims_.Get(index);
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Numpy::dims(int index) const {
  // @@protoc_insertion_point(field_get:MatlabPython.Numpy.dims)
  return _internal_dims(index);
}
inline void Numpy::set_dims(int index, ::PROTOBUF_NAMESPACE_ID::int32 value) {
  dims_.Set(index, value);
  // @@protoc_insertion_point(field_set:MatlabPython.Numpy.dims)
}
inline void Numpy::_internal_add_dims(::PROTOBUF_NAMESPACE_ID::int32 value) {
  dims_.Add(value);
}
inline void Numpy::add_dims(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_add_dims(value);
  // @@protoc_insertion_point(field_add:MatlabPython.Numpy.dims)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
Numpy::_internal_dims() const {
  return dims_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
Numpy::dims() const {
  // @@protoc_insertion_point(field_list:MatlabPython.Numpy.dims)
  return _internal_dims();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
Numpy::_internal_mutable_dims() {
  return &dims_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
Numpy::mutable_dims() {
  // @@protoc_insertion_point(field_mutable_list:MatlabPython.Numpy.dims)
  return _internal_mutable_dims();
}

// .MatlabPython.Type type = 3;
inline void Numpy::clear_type() {
  type_ = 0;
}
inline ::MatlabPython::Type Numpy::_internal_type() const {
  return static_cast< ::MatlabPython::Type >(type_);
}
inline ::MatlabPython::Type Numpy::type() const {
  // @@protoc_insertion_point(field_get:MatlabPython.Numpy.type)
  return _internal_type();
}
inline void Numpy::_internal_set_type(::MatlabPython::Type value) {
  
  type_ = value;
}
inline void Numpy::set_type(::MatlabPython::Type value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:MatlabPython.Numpy.type)
}

// bytes data = 4;
inline void Numpy::clear_data() {
  data_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& Numpy::data() const {
  // @@protoc_insertion_point(field_get:MatlabPython.Numpy.data)
  return _internal_data();
}
inline void Numpy::set_data(const std::string& value) {
  _internal_set_data(value);
  // @@protoc_insertion_point(field_set:MatlabPython.Numpy.data)
}
inline std::string* Numpy::mutable_data() {
  // @@protoc_insertion_point(field_mutable:MatlabPython.Numpy.data)
  return _internal_mutable_data();
}
inline const std::string& Numpy::_internal_data() const {
  return data_.Get();
}
inline void Numpy::_internal_set_data(const std::string& value) {
  
  data_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void Numpy::set_data(std::string&& value) {
  
  data_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:MatlabPython.Numpy.data)
}
inline void Numpy::set_data(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  data_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:MatlabPython.Numpy.data)
}
inline void Numpy::set_data(const void* value,
    size_t size) {
  
  data_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:MatlabPython.Numpy.data)
}
inline std::string* Numpy::_internal_mutable_data() {
  
  return data_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* Numpy::release_data() {
  // @@protoc_insertion_point(field_release:MatlabPython.Numpy.data)
  return data_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void Numpy::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    
  } else {
    
  }
  data_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), data,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:MatlabPython.Numpy.data)
}
inline std::string* Numpy::unsafe_arena_release_data() {
  // @@protoc_insertion_point(field_unsafe_arena_release:MatlabPython.Numpy.data)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return data_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void Numpy::unsafe_arena_set_allocated_data(
    std::string* data) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (data != nullptr) {
    
  } else {
    
  }
  data_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      data, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:MatlabPython.Numpy.data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace MatlabPython

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::MatlabPython::Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::MatlabPython::Type>() {
  return ::MatlabPython::Type_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_msgs_2eproto
