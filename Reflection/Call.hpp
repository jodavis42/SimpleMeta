#pragma once

#include "StaticTypeId.hpp"

struct BoundType;
struct Function;
struct Call;

/// Helper object to deal with template speciailization on call objects
template <typename T>
struct CallHelper
{
  static T Get(Call& call, int index)
  {
    using ArgType = UnqualifiedType<T>::type;
    char* location = call.GetLocationChecked(index, StaticTypeId<ArgType>::GetBoundType());
    return *reinterpret_cast<ArgType*>(location);
  }

  static void Set(Call& call, int index, const T& data)
  {
    using ArgType = UnqualifiedType<T>::type;
    char* location = call.GetLocationChecked(index, StaticTypeId<ArgType>::GetBoundType());
    *reinterpret_cast<ArgType*>(location) = data;
  }
};

/// Speciailization of the call helper to deal with reference types. This is achieved by converting
/// the stored data to/from a pointer, this way references point at the same address.
template <typename T>
struct CallHelper<T&> : public CallHelper<T>
{
  static T& Get(Call& call, int index)
  {
    using ArgType = UnqualifiedType<T>::type;
    char* location = call.GetLocationUnChecked(index);
    return **reinterpret_cast<ArgType**>(location);
  }

  static void Set(Call& call, int index, const T& data)
  {
    using ArgType = UnqualifiedType<T>::type;
    char* location = call.GetLocationUnChecked(index);
    const ArgType* pointer = &data;
    *reinterpret_cast<const ArgType**>(location) = pointer;
  }
};

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
    return CallHelper<T>::Get(*this, index);
  }
  template <typename T>
  T& GetReference(int index)
  {
    return CallHelper<T&>::Get(*this, index);
  }

  template <typename T>
  void Set(int index, const T& data)
  {
    CallHelper<T>::Set(*this, index, data);
  }
  // If the parameter is a reference type and you explicitly want to pass through the same address
  // then this overload must be used. This is to get around certain limitations of template function speciailization (at least for now).
  template <typename T>
  void SetReference(int index, const T& data)
  {
    CallHelper<T&>::Set(*this, index, data);
  }

  /// Invoke the function call (assumes all args have been set).
  bool Invoke();

  char* GetLocationChecked(int index, BoundType* boundType);
  char* GetLocationUnChecked(int index);

private:
  Function*  mFunction = nullptr;
  char* mBuffer = nullptr;
};
