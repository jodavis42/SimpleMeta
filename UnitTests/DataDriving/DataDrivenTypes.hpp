#pragma once

#include <string>
#include <vector>
#include "ReflectionLibrary.hpp"

struct DataDrivenField
{
  std::string mName;
  std::string mType;

  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType);
};

struct DataDrivenType
{
  std::string mName;
  std::string mBaseType;
  std::vector<DataDrivenField> mFields;

  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType);
};

struct DataDrivenTypes
{
  std::vector<DataDrivenType> mTypes;

  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType);
};
