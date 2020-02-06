#pragma once

#include <vector>
#include <unordered_map>

struct BoundType;

struct MetaLibrary
{
  void AddBoundType(BoundType* boundType)
  {
    mBoundTypes.push_back(boundType);
    mBoundTypeMap[boundType->mName] = boundType;
  }
  BoundType* FindBoundType(const std::string& name)
  {
    auto it = mBoundTypeMap.find(name);
    if(it != mBoundTypeMap.end())
      return it->second;
    return nullptr;
  }
  std::vector<BoundType*> mBoundTypes;
  std::unordered_map<std::string, BoundType*> mBoundTypeMap;
};
