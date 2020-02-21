#pragma once

#include "Asserts.hpp"
struct BoundType;
struct Any
{
  static constexpr size_t mBufferSize = 100;

  Any();
  Any(const Any& any);
  Any(const void* data, BoundType* boundType);
  ~Any();

  char* GetRawData();
  const char* GetRawData() const;
  bool IsStoredType(const BoundType* boundType) const;

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

  BoundType* GetStoredType() const
  {
    return mStoredType;
  }

private:
  char mBuffer[mBufferSize];
  BoundType* mStoredType = nullptr;
  bool mIsAllocated = false;
};
