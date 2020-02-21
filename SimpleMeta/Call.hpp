#pragma once

#include "StaticTypeId.hpp"

struct BoundType;
struct Function;

/// Used to call functions generically
struct Call
{
  // These expressions are used to mark special locations for writing.
  static constexpr int This = -1;
  static constexpr int Return = -2;

  Call(Function* function);
  ~Call();

  void SetValue(int index, const void* data, BoundType* boundType);
  void SetValueUnchecked(int index, const void* data, size_t size);
  void SetPointer(int index, const void* dataPtr, BoundType* boundType);
  void SetPointerUnchecked(int index, const void* dataPtr);

  template <typename T>
  T Get(int index)
  {
    using ArgType = UnqualifiedType<T>::type;
    char* location = GetLocationChecked(index, StaticTypeId<ArgType>::GetBoundType());
    return *reinterpret_cast<ArgType*>(location);
  }

  template <typename T>
  void Set(int index, const T& data)
  {
    using ArgType = UnqualifiedType<T>::type;
    char* location = GetLocationChecked(index, StaticTypeId<ArgType>::GetBoundType());
    *reinterpret_cast<ArgType*>(location) = data;
  }

  bool Invoke();

  char* GetLocationChecked(int index, BoundType* boundType);
  char* GetLocationUnChecked(int index);

private:
  Function*  mFunction = nullptr;
  char* mBuffer = nullptr;
};
