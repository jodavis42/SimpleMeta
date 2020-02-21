#pragma once

#include <string>
#include <vector>

#include "TypeId.hpp"
#include "ReflectionComposition.hpp"
#include "Any.hpp"

struct BoundType;
struct Call;
struct Function;

typedef void(*BoundFunction)(Call& call);

struct Field : public ReflectionComposition
{
  char* GetFieldData(char* instanceData) const;

  std::string mName;
  size_t mOffset = 0;
  BoundType* mType = nullptr;
};

struct GetterSetter : public ReflectionComposition
{
  Any Get(char* instanceData) const;
  void Set(char* instanceData, Any any);

  std::string mName;
  BoundType* mType = nullptr;
  Function* mGetter = nullptr;
  Function* mSetter = nullptr;
};

struct BoundType : public ReflectionComposition
{
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
