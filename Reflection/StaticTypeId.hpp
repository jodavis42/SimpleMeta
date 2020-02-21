#pragma once

#include "BoundType.hpp"

BoundType* RegisterBoundType(BoundType* boundType);

template <typename T>
struct StaticTypeId
{
  // This returns the raw bound type and should only be used when building types the first time.
  static BoundType*& GetBoundTypeRaw()
  {
    static BoundType* sInstance = new BoundType();
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

template <typename T>
struct StaticTypeId<T*> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    return StaticTypeId<T>::GetBoundType();
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

template<typename T>
struct UnqualifiedType
{
  using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};