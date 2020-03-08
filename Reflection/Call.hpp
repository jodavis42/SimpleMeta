#pragma once

#include "Asserts.hpp"
#include "StaticTypeId.hpp"

namespace SimpleReflection
{

struct BoundType;
struct Function;
struct Call;
struct Any;

/// Helper object to deal with template speciailization on call objects
template <typename T>
struct CallHelper
{
  static T GetChecked(Call& call, int index)
  {
    using ArgType = UnqualifiedType<T>::type;
    char* location = call.GetLocationChecked(index, StaticTypeId<ArgType>::GetBoundType());
    return *reinterpret_cast<ArgType*>(location);
  }

  static void SetChecked(Call& call, int index, const T& data)
  {
    using ArgType = UnqualifiedType<T>::type;
    call.CheckLocationType(index, StaticTypeId<ArgType>::GetBoundType());
    call.SetInternal(index, data, sizeof(data));
  }

  static void SetUnchecked(Call& call, int index, const T& data)
  {
    call.SetInternal(index, data, sizeof(data));
  }
};

/// Specialization of the call helper to deal with reference types. This is achieved by converting
/// the stored data to/from a pointer, this way references point at the same address.
template <typename T>
struct CallHelper<T&> : public CallHelper<T>
{
  using ArgType = typename UnqualifiedType<T>::type;
  using RefType = ArgType&;

  static T& GetChecked(Call& call, int index)
  {
    char* location = call.GetLocationChecked(index, StaticTypeId<RefType>::GetBoundType());
    return **reinterpret_cast<ArgType**>(location);
  }

  static void SetChecked(Call& call, int index, const T& data)
  {
    call.CheckLocationType(index, StaticTypeId<RefType>::GetBoundType());
    call.SetInternal(index, &data, sizeof(&data));
  }

  static void SetUnchecked(Call& call, int index, const T& data)
  {
    call.SetInternal(index, &data, sizeof(&data));
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
  void Set(int index, Any& any);

  template <typename T>
  T Get(int index)
  {
    return CallHelper<T>::GetChecked(*this, index);
  }

  template <typename T>
  void Set(int index, T& data)
  {
    // Choose to set this as a reference or a value type based upon the parameter type.
    // This is done because types passed in by reference will not actually be reference
    // template params (e.g. passing in a float& will always just be float)
    BoundType* expectedType = GetLocationType(index);
    if(expectedType->mIsReferenceType)
      CallHelper<T&>::SetChecked(*this, index, data);
    else
      CallHelper<T>::SetChecked(*this, index, data);
  }

  template <typename T>
  void SetUnchecked(int index, T& data)
  {
    // Choose to set this as a reference or a value type based upon the parameter type.
    // This is done because types passed in by reference will not actually be reference
    // template params (e.g. passing in a float& will always just be float)
    BoundType* expectedType = GetLocationType(index);
    if(expectedType->mIsReferenceType)
      CallHelper<T&>::SetUnchecked(*this, index, data);
    else
      CallHelper<T>::SetUnchecked(*this, index, data);

  }
  template <typename T>
  void SetInternal(int index, const T& data, size_t sizeInBytes)
  {
    using ArgType = UnqualifiedType<T>::type;
    size_t offset = GetLocationOffset(index);
    ReflectionErrorIf(offset + sizeInBytes > mBufferSizeInBytes, "Access out of bounds");
    char* location = mBuffer + offset;
    *reinterpret_cast<ArgType*>(location) = data;
  }

  /// Invoke the function call (assumes all args have been set).
  bool Invoke();

  
  char* GetLocationChecked(int index, BoundType* boundType);
  char* GetLocationUnChecked(int index);
  size_t GetLocationOffset(int index) const;
  BoundType* GetLocationType(int index) const;
  bool CheckLocationType(int index, const BoundType* givenType) const;

private:
  Call(const Call&) {}
  void operator=(const Call&) {}

  Function*  mFunction = nullptr;
  char* mBuffer = nullptr;
  size_t mBufferSizeInBytes = 0;
};

}//namespace SimpleReflection
