#pragma once

#include <string>
#include <vector>

#include "TypeId.hpp"
#include "ReflectionComposition.hpp"

struct Field;
struct Function;
struct GetterSetter;

struct BoundType : public ReflectionComposition
{
  ~BoundType();

  std::string mName;
  size_t mSizeInBytes = 0;
  std::vector<Field*> mFields;
  std::vector<GetterSetter*> mGetterSetters;
  BoundType* mBaseType = nullptr;
  TypeId mId;
  Function* mDefaultConstructor = nullptr;
  Function* mCopyConstructor = nullptr;
  Function* mDestructor = nullptr;
};
