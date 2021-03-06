#include "Precompiled.hpp"

#include "StaticTypeId.hpp"

#include "Asserts.hpp"
#include "ReflectionLibrary.hpp"

namespace SimpleReflection
{

BoundType* RegisterBoundType(BoundType* boundType)
{
  if(!boundType->mIsRegistered)
  {
    ReflectionProject* instance = ReflectionProject::GetInstance();
    instance->mCurrentLibrary->AddBoundType(boundType);
  }
  return boundType;
}

BoundType* RegisterPointerBoundType(BoundType* unqualifiedBase, BoundType* boundType)
{
  RegisterBoundType(boundType);
  boundType->mBaseType = unqualifiedBase;
  boundType->mName = unqualifiedBase->mName + "*";
  boundType->mIsPointerType = true;
  boundType->mSizeInBytes = sizeof(void*);
  return boundType;
}

BoundType* RegisterReferenceBoundType(BoundType* unqualifiedBase, BoundType* boundType)
{
  RegisterBoundType(boundType);
  boundType->mBaseType = unqualifiedBase;
  boundType->mName = unqualifiedBase->mName + "&";
  boundType->mIsReferenceType = true;
  boundType->mSizeInBytes = sizeof(void*);
  return boundType;
}

}//namespace SimpleReflection
