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
  TypeId mId;

  BoundType* mBaseType = nullptr;
  std::vector<Field*> mFields;
  std::vector<GetterSetter*> mGetterSetters;
  size_t mSizeInBytes = 0;
  
  Function* mDefaultConstructor = nullptr;
  Function* mCopyConstructor = nullptr;
  Function* mDestructor = nullptr;
};
