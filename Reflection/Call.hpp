#pragma once

#include "StaticTypeId.hpp"

struct BoundType;
struct Function;

/// Used to call functions generically. This class doesn't maintain ownership of any memory
struct Call
{
  // These expressions are used to mark special locations for writing.
  static constexpr int This = -1;
  static constexpr int Return = -2;

  Call(Function* function);
  ~Call();

  /// Sets the given data as if it's a value type (assumes the data is mem copyable).
  /// Will assert if the given bound type is not correct for the location.
  void SetValue(int index, const void* data, BoundType* boundType);
  void SetValueUnchecked(int index, const void* data, size_t size);
  /// Sets the given data as if it's a pointer (aka a this). The pointer is not owned so make sure the scope is valid.
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

  /// Invoke the function call (assumes all args have been set).
  bool Invoke();

  char* GetLocationChecked(int index, BoundType* boundType);
  char* GetLocationUnChecked(int index);

private:
  Function*  mFunction = nullptr;
  char* mBuffer = nullptr;
};
