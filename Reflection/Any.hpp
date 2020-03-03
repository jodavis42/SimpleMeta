#pragma once

#include "Asserts.hpp"

#include "StaticTypeId.hpp"

namespace SimpleReflection
{

struct BoundType;

/// A type that can store any type. Used to more easily pass around arbitrary data without relying on templates.
struct Any
{
  static constexpr size_t mBufferSize = 100;

  Any();
  Any(const Any& any);
  Any(const void* data, BoundType* boundType);
  ~Any();

  /// Returns the pointer to the raw data stored for the object
  /// (may be an allocation if the object is bigger than mBufferSize)
  char* GetRawData();
  const char* GetRawData() const;

  /// Checks if the given bound type is the same as what's stored
  bool IsStoredType(const BoundType* boundType) const;
  BoundType* GetStoredType() const;

  void AssignFrom(const void* data, BoundType* boundType);
  void operator=(const Any& any);
  bool operator==(const Any& any) const;
  
  void Clear();

  template <typename T>
  T Get()
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    ErrorIf(!IsStoredType(boundType), "Invalid Type in Any::Get");
    return *reinterpret_cast<T*>(GetRawData());
  }

  template <typename T>
  T Get() const
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    ErrorIf(!IsStoredType(boundType), "Invalid Type in Any::Get");
    return *reinterpret_cast<T*>(GetRawData());
  }

  template <typename T>
  void Set(const T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    AssignFrom(&data, boundType);
  }

private:
  char mBuffer[mBufferSize];
  BoundType* mStoredType = nullptr;
  bool mIsAllocated = false;
};

}//namespace SimpleReflection
