#pragma once

#include "BoundType.hpp"

namespace SimpleReflection
{

template <typename... Args>
struct RecursiveStaticTypeId;

BoundType* RegisterBoundType(BoundType* boundType);
BoundType* RegisterPointerBoundType(BoundType* unqualifiedBase, BoundType* boundType);
BoundType* RegisterReferenceBoundType(BoundType* unqualifiedBase, BoundType* boundType);

template <typename T>
BoundType InitializeNativeBoundType()
{
  BoundType boundType;
  boundType.mSizeInBytes = sizeof(T);
  boundType.mName = typeid(T).name();
  boundType.mNative = true;
  return std::move(boundType);
}

template <>
inline BoundType InitializeNativeBoundType<void>()
{
  BoundType boundType;
  boundType.mSizeInBytes = 0;
  boundType.mName = "void";
  boundType.mNative = true;
  return std::move(boundType);
}

template <typename T>
struct StaticTypeId
{
  // This assumes that all important types have already been built and will auto-register
  // a type otherwise (to deal with dynamic types like arrays).
  static BoundType* GetBoundType()
  {
    static BoundType sInstance = InitializeNativeBoundType<T>();
    RegisterBoundType(&sInstance);
    return &sInstance;
  }
};

// Speciailize pointer types so we can properly track pointers to base types and so on
template <typename T>
struct StaticTypeId<T*> : public StaticTypeId<T>
{
  static BoundType* GetBoundType()
  {
    static BoundType sInstance = InitializeNativeBoundType<T*>();
    RegisterPointerBoundType(StaticTypeId<T>::GetBoundType(), &sInstance);
    return &sInstance;
  }
};

// Do the same for reference types
template <typename T>
struct StaticTypeId<T&> : public StaticTypeId<T>
{
  static BoundType* GetBoundType()
  {
    static BoundType sInstance = InitializeNativeBoundType<T&>();
    RegisterReferenceBoundType(StaticTypeId<T>::GetBoundType(), &sInstance);
    return &sInstance;
  }
};

// Deal with const types...
template <typename T>
struct StaticTypeId<const T> : public StaticTypeId<T>
{
  static BoundType* GetBoundType()
  {
    return StaticTypeId<T>::GetBoundType();
  }
};
template <typename T>
struct StaticTypeId<const T*> : public StaticTypeId<T>
{
  static BoundType* GetBoundType()
  {
    return StaticTypeId<T*>::GetBoundType();
  }
};
template <typename T>
struct StaticTypeId<const T&> : public StaticTypeId<T>
{
  static BoundType* GetBoundType()
  {
    return StaticTypeId<T&>::GetBoundType();
  }
};

// Deal with templated types (have to visit the sub-types)
template <template <typename...> typename ContainerType, typename ... ParamTypes>
struct StaticTypeId< ContainerType<ParamTypes...> >
{
  static BoundType Initialize()
  {
    RecursiveStaticTypeId<ParamTypes...>::Recurse();
    BoundType boundType = InitializeNativeBoundType<ContainerType<ParamTypes...>>();
    return boundType;
  }
  static BoundType* GetBoundType()
  {
    static BoundType sInstance = Initialize();
    return &sInstance;
  }
};

/// Recursive template to call StaticTypeId::GetBoundType() on all args in a variadic
template <typename First, typename... Args>
struct RecursiveStaticTypeId<First, Args...>
{
  static void Recurse()
  {
    StaticTypeId<First>::GetBoundType();
    RecursiveStaticTypeId<Args...>::Recurse();
  }
};
/// Base case to terminate recursion on no params
template <>
struct RecursiveStaticTypeId<>
{
  static void Recurse()
  {
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

#define ReflectionBoundTypeOf(Type) SimpleReflection::StaticTypeId<Type>::GetBoundType()

}//namespace SimpleReflection
