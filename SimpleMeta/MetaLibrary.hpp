#pragma once

#include <vector>
#include <unordered_map>
#include "BoundType.hpp"

struct BoundType;

struct MetaLibrary
{
  static void AddBoundType(BoundType* boundType)
  {
    mBoundTypes.push_back(boundType);
    mBoundTypeMap[boundType->mName] = boundType;
    mBoundTypeIdMap[boundType->mId.mId] = boundType;
  }
  static BoundType* FindBoundType(const std::string& name)
  {
    auto it = mBoundTypeMap.find(name);
    if(it != mBoundTypeMap.end())
      return it->second;
    return nullptr;
  }
  static BoundType* FindBoundType(const TypeId& id)
  {
    auto it = mBoundTypeIdMap.find(id.mId);
    if(it != mBoundTypeIdMap.end())
      return it->second;
    return nullptr;
  }
  static std::vector<BoundType*> mBoundTypes;
  static std::unordered_map<std::string, BoundType*> mBoundTypeMap;
  static std::unordered_map<size_t, BoundType*> mBoundTypeIdMap;
};
