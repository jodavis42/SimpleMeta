#pragma once

#include "BoundType.hpp"

BoundType* ValidateRawBoundType(BoundType* boundType);
BoundType* RegisterBoundType(BoundType* boundType);
BoundType* RegisterPointerBoundType(BoundType* unqualifiedBase, BoundType* boundType);
BoundType* RegisterReferenceBoundType(BoundType* unqualifiedBase, BoundType* boundType);

template <typename T>
struct StaticTypeId
{
  // This returns the raw bound type and should only be used when building types the first time.
  static BoundType*& GetBoundTypeRaw()
  {
    static BoundType* sInstance = ValidateRawBoundType(new BoundType());
    return sInstance;
  }

  // This assumes that all important types have already been built and will auto-register
  // a type otherwise (to deal with dynamic types like arrays).
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = RegisterBoundType(GetBoundTypeRaw());
    return sInstance;
  }
};

// Speciailize pointer types so we can properly track pointers to base types and so on
template <typename T>
struct StaticTypeId<T*> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = RegisterPointerBoundType(StaticTypeId<T>::GetBoundType(), new BoundType());
    return sInstance;
  }
};

// Do the same for reference types
template <typename T>
struct StaticTypeId<T&> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = RegisterReferenceBoundType(StaticTypeId<T>::GetBoundType(), new BoundType());
    return sInstance;
  }
};

// Deal with const types...
template <typename T>
struct StaticTypeId<const T*> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    return StaticTypeId<T*>::GetBoundType();
  }
};
template <typename T>
struct StaticTypeId<const T&> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    return StaticTypeId<T&>::GetBoundType();
  }
};

template <typename T>
struct RuntimeTypeId
{
  static BoundType* GetVirtualBoundType(char* data)
  {
    T typedObj = (T)data;
    return typedObj->VirtualGetBoundType();
  }

  static char* Allocate()
  {
    return new T();
  }

  static void AllocateInPlace(char* data)
  {
    new(data) T();
  }
};

/// Helper to string types down to their base type (e.g. turn const float& -> float). This is necessary
/// during function binding so that input/output types can be declared correctly.
template<typename T>
struct UnqualifiedType
{
  using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};