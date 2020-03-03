#pragma once

#include "StaticTypeId.hpp"

using SimpleReflection::StaticTypeId;
using SimpleReflection::BoundType;

template <typename T>
struct SerializationPolicy
{
  template <typename SerializerType>
  static bool Serialize(SerializerType& serializer, T& obj)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    return serializer.SerializeObject(*boundType, (char*)&obj);
  }
};

#define DeclarePrimitivePolicy(PrimitiveType)                           \
template <>                                                             \
struct SerializationPolicy<PrimitiveType>                               \
{                                                                       \
  template <typename SerializerType>                                    \
  static bool Serialize(SerializerType& serializer, PrimitiveType& obj) \
  {                                                                     \
    BoundType* boundType = StaticTypeId<PrimitiveType>::GetBoundType(); \
    return serializer.SerializePrimitive(*boundType, obj);              \
  }                                                                     \
};

DeclarePrimitivePolicy(bool)
DeclarePrimitivePolicy(char)
DeclarePrimitivePolicy(int)
DeclarePrimitivePolicy(float)
DeclarePrimitivePolicy(double)
DeclarePrimitivePolicy(std::string)
