#include "StaticTypeId.hpp"

#include "Asserts.hpp"
#include "MetaLibrary.hpp"

BoundType* ValidateRawBoundType(BoundType* boundType)
{
  ReflectionProject* instance = ReflectionProject::GetInstance();
  ErrorIf(instance->mCurrentLibrary->mIsFinalized, "Library is finalized, can't create a new raw bound type.");
  return boundType;
}

BoundType* RegisterBoundType(BoundType* boundType)
{
  ReflectionProject* instance = ReflectionProject::GetInstance();
  if(instance->FindBoundType(boundType->mId))
    return boundType;

  // This isn't guaranteed to work, a real solution is needed here...
  static size_t index = 999999;
  boundType->mId = index++;
  instance->mCurrentLibrary->AddBoundType(boundType);
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
