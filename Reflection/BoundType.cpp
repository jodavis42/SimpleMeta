#include "Precompiled.hpp"

#include "BoundType.hpp"

#include "Field.hpp"
#include "GetterSetter.hpp"
#include "Function.hpp"

namespace SimpleReflection
{

BoundType::~BoundType()
{
  Destruct();
}

void BoundType::Destruct()
{
  delete mDefaultConstructor;
  mDefaultConstructor = nullptr;
  delete mCopyConstructor;
  mCopyConstructor = nullptr;
  delete mDestructor;
  mDestructor = nullptr;

  for(Field* field : mFields)
    delete field;
  mFields.clear();

  for(GetterSetter* getSet : mGetterSetters)
    delete getSet;
  mGetterSetters.clear();

  for(auto&& pair : mFunctionMap)
  {
    for(Function* function : pair.second)
    {
      delete function;
    }
  }
  mFunctionMap.clear();

  mIsSetup = false;
  mIsRegistered = false;
}

FieldRange BoundType::GetFields()
{
  return FieldRange(this);
}

GetterSetterRange BoundType::GetGetterSetters()
{
  return GetterSetterRange(this);
}

bool BoundType::FindFunctions(const std::string& fnName, std::vector<Function*>& functions, bool recursive)
{
  auto it = mFunctionMap.find(fnName);
  if(it != mFunctionMap.end())
  {
    functions = it->second;
    return true;
  }
    
  if(recursive && mBaseType != nullptr)
    return mBaseType->FindFunctions(fnName, functions, recursive);
  return false;
}

}//namespace SimpleReflection
