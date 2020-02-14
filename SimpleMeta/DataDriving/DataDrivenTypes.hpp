#pragma once

#include <string>
#include <vector>
#include "MetaLibrary.hpp"

struct DataDrivenField
{
  std::string mName;
  std::string mType;

  static void Bind(ReflectionLibrary& library, BoundType& boundType);
};

struct DataDrivenType
{
  std::string mName;
  std::string mBaseType;
  std::vector<DataDrivenField> mFields;

  static void Bind(ReflectionLibrary& library, BoundType& boundType);
};

struct DataDrivenTypes
{
  std::vector<DataDrivenType> mTypes;

  static void Bind(ReflectionLibrary& library, BoundType& boundType);
};
