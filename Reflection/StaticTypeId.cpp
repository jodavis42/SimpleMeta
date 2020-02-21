#include "StaticTypeId.hpp"

#include "MetaLibrary.hpp"

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
