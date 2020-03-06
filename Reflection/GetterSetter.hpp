#pragma once

#include <string>
#include "ReflectionComposition.hpp"
#include "Any.hpp"

namespace SimpleReflection
{

struct BoundType;
struct Call;
struct Function;

struct GetterSetter : public ReflectionComposition
{
  ~GetterSetter();

  Any Get(void* instanceData) const;
  void Set(void* instanceData, Any any);

  std::string mName;
  BoundType* mType = nullptr;
  Function* mGetter = nullptr;
  Function* mSetter = nullptr;
};

}//namespace SimpleReflection
