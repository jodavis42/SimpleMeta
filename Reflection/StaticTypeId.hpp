#pragma once

#include "BoundType.hpp"

template <typename T>
BoundType* AllocateBoundType()
{
  BoundType* boundType = RegisterBoundType(new BoundType());
  boundType->mSizeInBytes = sizeof(T);
  boundType->mName = typeid(T).name();
  return boundType;
}

BoundType* ValidateRawBoundType(BoundType* boundType);
BoundType* RegisterBoundType(BoundType* boundType);
BoundType* RegisterPointerBoundType(BoundType* unqualifiedBase, BoundType* boundType);
BoundType* RegisterReferenceBoundType(BoundType* unqualifiedBase, BoundType* boundType);

template <typename T>
struct StaticTypeId
{
  static BoundType* AutoRegisterType(BoundType* boundType)
  {
    boundType = RegisterBoundType(boundType);
    boundType->mSizeInBytes = sizeof(T);
    return boundType;
  }

  // This assumes that all important types have already been built and will auto-register
  // a type otherwise (to deal with dynamic types like arrays).
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = AllocateBoundType<T>();
    return sInstance;
  }
};

template <>
struct StaticTypeId<void>
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = ValidateRawBoundType(new BoundType());
    return sInstance;
  }
};

// Speciailize pointer types so we can properly track pointers to base types and so on
template <typename T>
struct StaticTypeId<T*> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = AllocateBoundType<T>();
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
struct StaticTypeId<const T> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    return StaticTypeId<T>::GetBoundType();
  }
};
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

// Deal with templated types (have to visit the sub-types)
template <template <typename> typename ContainerType, typename ParamType0>
struct StaticTypeId< ContainerType<ParamType0> >
{
  static BoundType* Create()
  {
    StaticTypeId<ParamType0>::GetBoundType();
    return AllocateBoundType<ContainerType<ParamType0>>();
  }
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = Create();
    return sInstance;
  }
};
template <template <typename, typename> typename ContainerType, typename ParamType0, typename ParamType1>
struct StaticTypeId< ContainerType<ParamType0, ParamType1> >
{
  static BoundType* Create()
  {
    StaticTypeId<ParamType0>::GetBoundType();
    StaticTypeId<ParamType1>::GetBoundType();
    return AllocateBoundType<ContainerType<ParamType0, ParamType1>>();
  }
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = Create();
    return sInstance;
  }
};
template <template <typename, typename, typename> typename ContainerType, typename ParamType0, typename ParamType1, typename ParamType2>
struct StaticTypeId< ContainerType<ParamType0, ParamType1, ParamType2> >
{
  static BoundType* Create()
  {
    StaticTypeId<ParamType0>::GetBoundType();
    StaticTypeId<ParamType1>::GetBoundType();
    StaticTypeId<ParamType2>::GetBoundType();
    return AllocateBoundType<ContainerType<ParamType0, ParamType1, ParamType2>>();
  }
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = Create();
    return sInstance;
  }
};
template <template <typename, typename, typename, typename> typename ContainerType, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3>
struct StaticTypeId< ContainerType<ParamType0, ParamType1, ParamType2, ParamType3> >
{
  static BoundType* Create()
  {
    StaticTypeId<ParamType0>::GetBoundType();
    StaticTypeId<ParamType1>::GetBoundType();
    StaticTypeId<ParamType2>::GetBoundType();
    StaticTypeId<ParamType3>::GetBoundType();
    return AllocateBoundType<ContainerType<ParamType0, ParamType1, ParamType2, ParamType3>>();
  }
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = Create();
    return sInstance;
  }
};
template <template <typename, typename, typename, typename, typename> typename ContainerType, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3, typename ParamType4>
struct StaticTypeId< ContainerType<ParamType0, ParamType1, ParamType2, ParamType3, ParamType4> >
{
  static BoundType* Create()
  {
    StaticTypeId<ParamType0>::GetBoundType();
    StaticTypeId<ParamType1>::GetBoundType();
    StaticTypeId<ParamType2>::GetBoundType();
    StaticTypeId<ParamType3>::GetBoundType();
    StaticTypeId<ParamType4>::GetBoundType();
    return AllocateBoundType<ContainerType<ParamType0, ParamType1, ParamType2, ParamType3, ParamType4>>();
  }
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = Create();
    return sInstance;
  }
};
// Deal with templated types (have to visit the sub-types)
template <template <typename...> typename ContainerType, typename ... ParamTypes>
struct StaticTypeId< ContainerType<ParamTypes...> >
{
  static BoundType*& GetBoundType()
  {
    static_assert(false, "Cannot support unknown parameter count");
    static BoundType* sInstance = nullptr;
    return sInstance;
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