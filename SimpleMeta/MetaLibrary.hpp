#pragma once

#include <vector>

struct BoundType;

struct MetaLibrary
{
  void AddBoundType(BoundType* boundType)
  {
    mBoundTypes.push_back(boundType);
  }
  std::vector<BoundType*> mBoundTypes;
};
