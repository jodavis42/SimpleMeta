#include "Precompiled.hpp"

#include "BoundType.hpp"

#include "Field.hpp"
#include "GetterSetter.hpp"
#include "Function.hpp"
#include "Call.hpp"

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

  for(Function* function : mConstructors)
    delete function;
  mConstructors.clear();

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

Function* BoundType::FindDefaultConstructor(bool checkInherited)
{
  if(mDefaultConstructor != nullptr)
    return mDefaultConstructor;
  if(mBaseType != nullptr)
    return mBaseType->FindDefaultConstructor(checkInherited);
  return nullptr;
}

Function* BoundType::FindConstructor(const FunctionType& functionType, bool checkInherited)
{
  if(mDefaultConstructor != nullptr && functionType.IsA(mDefaultConstructor->mFunctionType))
    return mDefaultConstructor;

  for(Function* function : mConstructors)
  {
    if(functionType.IsA(function->mFunctionType))
      return function;
  }

  if(mBaseType != nullptr)
    return mBaseType->FindConstructor(functionType, checkInherited);
  return nullptr;
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

Function* BoundType::FindFunction(const std::string& fnName, const FunctionType& functionType, bool checkInherited)
{
  auto it = mFunctionMap.find(fnName);
  // Check derived types before base types
  if(it != mFunctionMap.end())
  {
    auto&& functions = it->second;
    for(Function* function : functions)
    {
      if(function->mFunctionType.IsA(functionType))
        return function;
    }
  }

  if(checkInherited && mBaseType != nullptr)
    return mBaseType->FindFunction(fnName, functionType, checkInherited);
  return nullptr;
}

void* BoundType::GenericDefaultConstruct() const
{
  void* self = malloc(mSizeInBytes);
  GenericDefaultConstruct(self);
  return self;
}

void BoundType::GenericDefaultConstruct(void* self) const
{
  if(mDefaultConstructor == nullptr)
  {
    // Clear the data for safety (could be removed for performance)
    memset(self, 0, mSizeInBytes);
  }
  else
  {
    Call call(mDefaultConstructor);
    call.SetPointerUnchecked(Call::This, self);
    call.Invoke();
  }
}

void* BoundType::GenericCopyConstruct(void* data) const
{
  void* self = malloc(mSizeInBytes);
  GenericCopyConstruct(self, data);
  return self;
}

void BoundType::GenericCopyConstruct(void* self, void* data) const
{
  // Invoke the bound copy constructor if it exists, otherwise just copy the memory
  if(mCopyConstructor != nullptr)
  {
    Call call(mCopyConstructor);
    call.SetPointerUnchecked(Call::This, self);
    call.SetInternal(0, data, sizeof(data));
    call.Invoke();
  }
  else
  {
    memcpy(self, data, mSizeInBytes);
  }
}

void BoundType::GenericDestruct(void* self) const
{
  GenericDestructNoFree(self);
  free(self);
}

void BoundType::GenericDestructNoFree(void* self) const
{
  // Invoke the bound destructor if one exists
  if(mDestructor != nullptr)
  {
    Call call(mDestructor);
    call.SetPointerUnchecked(Call::This, self);
    call.Invoke();
  }
}

bool BoundType::IsIndirectionType() const
{
  return mIsPointerType || mIsReferenceType;
}

bool BoundType::IsA(const BoundType* derrivedType, const BoundType* baseType)
{
  if(derrivedType == baseType)
    return true;

  // Handle null cases (if they're both null, we would've returned true above)
  if(derrivedType == nullptr || baseType == nullptr)
    return false;

  if(derrivedType->IsIndirectionType())
    derrivedType = derrivedType->mBaseType;
  if(baseType->IsIndirectionType())
    baseType = baseType->mBaseType;


  while(derrivedType != nullptr)
  {
    if(derrivedType == baseType)
      return true;
    derrivedType = derrivedType->mBaseType;
  }
  return false;
}

}//namespace SimpleReflection
