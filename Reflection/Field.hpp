#pragma once

#include <string>
#include "ReflectionComposition.hpp"

namespace SimpleReflection
{

struct BoundType;

struct Field : public ReflectionComposition
{
  char* GetFieldData(char* instanceData) const;

  std::string mName;
  size_t mOffset = 0;
  BoundType* mType = nullptr;
};

}//namespace SimpleReflection
