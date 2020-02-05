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
struct StaticTypeId<std::vector<T>> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = new BoundType();
    sInstance->mPrimitiveType = BoundTypePrimitiveType::Array;
    return sInstance;
  }
};