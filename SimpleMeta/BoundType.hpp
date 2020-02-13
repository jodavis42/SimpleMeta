#pragma once

#include <string>
#include <vector>

#include "TypeId.hpp"
#include "ReflectionComposition.hpp"

struct BoundType;
struct MetaSerialization;

struct Field : public ReflectionComposition
{
  char* GetFieldData(char* instanceData)
  {
    return instanceData + mOffset;
  }

  std::string mName;
  size_t mOffset = 0;
  BoundType* mType = nullptr;
};

struct BoundType : public ReflectionComposition
{
  std::string mName;
  size_t mSizeInBytes = 0;
  std::vector<Field*> mFields;
  MetaSerialization* mMetaSerialization = nullptr;
  BoundType* mBaseType = nullptr;
  TypeId mId;
};
