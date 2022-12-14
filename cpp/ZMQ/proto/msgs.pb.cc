// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msgs.proto

#include "msgs.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace MatlabPython {
class NumpyDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Numpy> _instance;
} _Numpy_default_instance_;
}  // namespace MatlabPython
static void InitDefaultsscc_info_Numpy_msgs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::MatlabPython::_Numpy_default_instance_;
    new (ptr) ::MatlabPython::Numpy();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::MatlabPython::Numpy::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Numpy_msgs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Numpy_msgs_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_msgs_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_msgs_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_msgs_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_msgs_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::MatlabPython::Numpy, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::MatlabPython::Numpy, num_dims_),
  PROTOBUF_FIELD_OFFSET(::MatlabPython::Numpy, dims_),
  PROTOBUF_FIELD_OFFSET(::MatlabPython::Numpy, type_),
  PROTOBUF_FIELD_OFFSET(::MatlabPython::Numpy, data_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::MatlabPython::Numpy)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::MatlabPython::_Numpy_default_instance_),
};

const char descriptor_table_protodef_msgs_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\nmsgs.proto\022\014MatlabPython\"W\n\005Numpy\022\020\n\010n"
  "um_dims\030\001 \001(\005\022\014\n\004dims\030\002 \003(\005\022 \n\004type\030\003 \001("
  "\0162\022.MatlabPython.Type\022\014\n\004data\030\004 \001(\014*)\n\004T"
  "ype\022\013\n\007FLOAT64\020\000\022\t\n\005INT32\020\001\022\t\n\005UINT8\020\002b\006"
  "proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_msgs_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_msgs_2eproto_sccs[1] = {
  &scc_info_Numpy_msgs_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_msgs_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_msgs_2eproto = {
  false, false, descriptor_table_protodef_msgs_2eproto, "msgs.proto", 166,
  &descriptor_table_msgs_2eproto_once, descriptor_table_msgs_2eproto_sccs, descriptor_table_msgs_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_msgs_2eproto::offsets,
  file_level_metadata_msgs_2eproto, 1, file_level_enum_descriptors_msgs_2eproto, file_level_service_descriptors_msgs_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_msgs_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_msgs_2eproto)), true);
namespace MatlabPython {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Type_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_msgs_2eproto);
  return file_level_enum_descriptors_msgs_2eproto[0];
}
bool Type_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// ===================================================================

void Numpy::InitAsDefaultInstance() {
}
class Numpy::_Internal {
 public:
};

Numpy::Numpy(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  dims_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:MatlabPython.Numpy)
}
Numpy::Numpy(const Numpy& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      dims_(from.dims_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  data_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_data().empty()) {
    data_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_data(),
      GetArena());
  }
  ::memcpy(&num_dims_, &from.num_dims_,
    static_cast<size_t>(reinterpret_cast<char*>(&type_) -
    reinterpret_cast<char*>(&num_dims_)) + sizeof(type_));
  // @@protoc_insertion_point(copy_constructor:MatlabPython.Numpy)
}

void Numpy::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Numpy_msgs_2eproto.base);
  data_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&num_dims_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&type_) -
      reinterpret_cast<char*>(&num_dims_)) + sizeof(type_));
}

Numpy::~Numpy() {
  // @@protoc_insertion_point(destructor:MatlabPython.Numpy)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Numpy::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  data_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void Numpy::ArenaDtor(void* object) {
  Numpy* _this = reinterpret_cast< Numpy* >(object);
  (void)_this;
}
void Numpy::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Numpy::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Numpy& Numpy::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Numpy_msgs_2eproto.base);
  return *internal_default_instance();
}


void Numpy::Clear() {
// @@protoc_insertion_point(message_clear_start:MatlabPython.Numpy)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  dims_.Clear();
  data_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::memset(&num_dims_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&type_) -
      reinterpret_cast<char*>(&num_dims_)) + sizeof(type_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Numpy::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 num_dims = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          num_dims_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated int32 dims = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedInt32Parser(_internal_mutable_dims(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16) {
          _internal_add_dims(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .MatlabPython.Type type = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_type(static_cast<::MatlabPython::Type>(val));
        } else goto handle_unusual;
        continue;
      // bytes data = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          auto str = _internal_mutable_data();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Numpy::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:MatlabPython.Numpy)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 num_dims = 1;
  if (this->num_dims() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_num_dims(), target);
  }

  // repeated int32 dims = 2;
  {
    int byte_size = _dims_cached_byte_size_.load(std::memory_order_relaxed);
    if (byte_size > 0) {
      target = stream->WriteInt32Packed(
          2, _internal_dims(), byte_size, target);
    }
  }

  // .MatlabPython.Type type = 3;
  if (this->type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      3, this->_internal_type(), target);
  }

  // bytes data = 4;
  if (this->data().size() > 0) {
    target = stream->WriteBytesMaybeAliased(
        4, this->_internal_data(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:MatlabPython.Numpy)
  return target;
}

size_t Numpy::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:MatlabPython.Numpy)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated int32 dims = 2;
  {
    size_t data_size = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      Int32Size(this->dims_);
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _dims_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // bytes data = 4;
  if (this->data().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_data());
  }

  // int32 num_dims = 1;
  if (this->num_dims() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_num_dims());
  }

  // .MatlabPython.Type type = 3;
  if (this->type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_type());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Numpy::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:MatlabPython.Numpy)
  GOOGLE_DCHECK_NE(&from, this);
  const Numpy* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Numpy>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:MatlabPython.Numpy)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:MatlabPython.Numpy)
    MergeFrom(*source);
  }
}

void Numpy::MergeFrom(const Numpy& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:MatlabPython.Numpy)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  dims_.MergeFrom(from.dims_);
  if (from.data().size() > 0) {
    _internal_set_data(from._internal_data());
  }
  if (from.num_dims() != 0) {
    _internal_set_num_dims(from._internal_num_dims());
  }
  if (from.type() != 0) {
    _internal_set_type(from._internal_type());
  }
}

void Numpy::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:MatlabPython.Numpy)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Numpy::CopyFrom(const Numpy& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:MatlabPython.Numpy)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Numpy::IsInitialized() const {
  return true;
}

void Numpy::InternalSwap(Numpy* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  dims_.InternalSwap(&other->dims_);
  data_.Swap(&other->data_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Numpy, type_)
      + sizeof(Numpy::type_)
      - PROTOBUF_FIELD_OFFSET(Numpy, num_dims_)>(
          reinterpret_cast<char*>(&num_dims_),
          reinterpret_cast<char*>(&other->num_dims_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Numpy::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace MatlabPython
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::MatlabPython::Numpy* Arena::CreateMaybeMessage< ::MatlabPython::Numpy >(Arena* arena) {
  return Arena::CreateMessageInternal< ::MatlabPython::Numpy >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
