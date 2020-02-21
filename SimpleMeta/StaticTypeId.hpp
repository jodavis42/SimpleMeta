#pragma once

#include "BoundType.hpp"

template <typename T>
struct StaticTypeId
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = new BoundType();
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