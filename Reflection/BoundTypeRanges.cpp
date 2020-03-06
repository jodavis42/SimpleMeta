#include "Precompiled.hpp"

#include "BoundTypeRanges.hpp"

#include "Asserts.hpp"
#include "BoundType.hpp"

namespace SimpleReflection
{

FieldRange::FieldRange(BoundType* boundType)
  : mBoundType(boundType)
  , mMemberIndex(0)
{
  while(boundType != nullptr)
  {
    mStack.push_back(boundType);
    boundType = boundType->mBaseType;
  }
  SkipEmpty();
}

Field* FieldRange::Front()
{
  BoundType* boundType = mStack.back();
  return boundType->mFields[mMemberIndex];
}

void FieldRange::PopFront()
{
  if(mStack.empty())
  {
    ReflectionErrorIf(true, "Cannot pop an empty range");
    return;
  }

  ++mMemberIndex;
  SkipEmpty();
}

bool FieldRange::Empty() const
{
  return mStack.empty();
}

void FieldRange::SkipEmpty()
{
  BoundType* boundType = mStack.back();
  while(mMemberIndex >= boundType->mFields.size())
  {
    mMemberIndex = 0;
    mStack.pop_back();
    if(mStack.empty())
      break;

    boundType = mStack.back();
  }
}

GetterSetterRange::GetterSetterRange(BoundType* boundType)
  : mBoundType(boundType)
  , mMemberIndex(0)
{
  mBoundType = boundType;
  mMemberIndex = 0;
  while(boundType != nullptr)
  {
    mStack.push_back(boundType);
    boundType = boundType->mBaseType;
  }

  SkipEmpty();
}

GetterSetter* GetterSetterRange::Front()
{
  BoundType* boundType = mStack.back();
  return boundType->mGetterSetters[mMemberIndex];
}

void GetterSetterRange::PopFront()
{
  if(mStack.empty())
  {
    ReflectionErrorIf(true, "Cannot pop an empty range");
    return;
  }

  ++mMemberIndex;
  SkipEmpty();
}

bool GetterSetterRange::Empty() const
{
  return mStack.empty();
}

void GetterSetterRange::SkipEmpty()
{
  BoundType* boundType = mStack.back();
  while(mMemberIndex >= boundType->mGetterSetters.size())
  {
    mMemberIndex = 0;
    mStack.pop_back();
    if(mStack.empty())
      break;

    boundType = mStack.back();
  }
}

}//namespace SimpleReflection
