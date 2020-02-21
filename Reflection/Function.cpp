#include "Function.hpp"

#include "BoundType.hpp"

size_t Function::ComputeSizeRequired() const
{
  // This information could be baked into the fields/params. Worry about this if it matters
  size_t size = 0;
  if(mReturnType != nullptr)
    size += mReturnType->mSizeInBytes;
  if(mThisType != nullptr)
    size += mThisType->mSizeInBytes;
  for(size_t i = 0; i < mParams.size(); ++i)
    size += mParams[i]->mSizeInBytes;
  return size;
}

BoundType* Function::GetReturnType() const
{
  return mReturnType;
}

void Function::SetReturnType(BoundType* boundType)
{
  mReturnType = boundType;
}

BoundType* Function::GetThisType() const
{
  return mThisType;
}

void Function::SetThisType(BoundType* boundType)
{
  mThisType = boundType;
}

BoundType* Function::GetParamType(size_t index) const
{
  if(index < mParams.size())
    return mParams[index];
  return nullptr;
}

void Function::SetParamType(size_t index, BoundType* boundType)
{
  if(index >= mParams.size())
    mParams.resize(index + 1);
  mParams[index] = boundType;
}

bool Function::IsStatic() const
{
  return mThisType == nullptr;
}
