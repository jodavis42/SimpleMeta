#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "TypeId.hpp"
#include "ReflectionComposition.hpp"
#include "BoundTypeRanges.hpp"

namespace SimpleReflection
{

struct Field;
struct Function;
struct GetterSetter;

struct BoundType : public ReflectionComposition
{
  ~BoundType();

  void Destruct();

  std::string mName;
  TypeId mId;

  FieldRange GetFields();
  GetterSetterRange GetGetterSetters();
  bool FindFunctions(const std::string& fnName, std::vector<Function*>& functions, bool recursive);

  BoundType* mBaseType = nullptr;
  std::vector<Field*> mFields;
  std::vector<GetterSetter*> mGetterSetters;
  std::unordered_map<std::string, std::vector<Function*>> mFunctionMap;
  size_t mSizeInBytes = 0;
  
  Function* mDefaultConstructor = nullptr;
  Function* mCopyConstructor = nullptr;
  Function* mDestructor = nullptr;
  bool mIsPointerType = false;
  bool mIsReferenceType = false;
  bool mNative = false;
  bool mIsRegistered = false;
  bool mIsSetup = false;
};

}//namespace SimpleReflection
