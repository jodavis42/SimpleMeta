#include "Precompiled.hpp"

#include "Function.hpp"

#include "BoundType.hpp"

namespace SimpleReflection
{

size_t FunctionType::ComputeSizeRequired() const
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

void FunctionType::ResizeParamCount(size_t count)
{
  mParams.resize(count);
}

BoundType* FunctionType::GetReturnType() const
{
  return mReturnType;
}

void FunctionType::SetReturnType(BoundType* boundType)
{
  mReturnType = boundType;
}

BoundType* FunctionType::GetThisType() const
{
  return mThisType;
}

void FunctionType::SetThisType(BoundType* boundType)
{
  mThisType = boundType;
}

size_t FunctionType::GetParamCount() const
{
  return mParams.size();
}

BoundType* FunctionType::GetParamType(size_t index) const
{
  if(index < mParams.size())
    return mParams[index];
  return nullptr;
}

void FunctionType::SetParamType(size_t index, BoundType* boundType)
{
  if(index >= mParams.size())
    mParams.resize(index + 1);
  mParams[index] = boundType;
}

bool FunctionType::IsA(const FunctionType& baseType) const
{
  bool isSame = BoundType::IsA(mReturnType, baseType.mReturnType);
  isSame &= BoundType::IsA(mThisType, baseType.mThisType);
  isSame &= mParams.size() == baseType.mParams.size();
  if(!isSame)
    return false;
  
  for(size_t i = 0; i < mParams.size(); ++i)
  {
    if(BoundType::IsA(mParams[i], baseType.mParams[i]) == false)
    {
      isSame = false;
      break;
    }
  }
  return isSame;
}

bool FunctionType::IsStatic() const
{
  return mThisType == nullptr;
}

size_t Function::ComputeSizeRequired() const
{
  return mFunctionType.ComputeSizeRequired();
}

BoundType* Function::GetReturnType() const
{
  return mFunctionType.GetReturnType();
}

void Function::SetReturnType(BoundType* boundType)
{
  mFunctionType.SetReturnType(boundType);
}

BoundType* Function::GetThisType() const
{
  return mFunctionType.GetThisType();
}

void Function::SetThisType(BoundType* boundType)
{
  mFunctionType.SetThisType(boundType);
}

size_t Function::GetParamCount() const
{
  return mFunctionType.GetParamCount();
}

BoundType* Function::GetParamType(size_t index) const
{
  return mFunctionType.GetParamType(index);
}

void Function::SetParamType(size_t index, BoundType* boundType)
{
  mFunctionType.SetParamType(index, boundType);
}

bool Function::IsStatic() const
{
  return mFunctionType.IsStatic();
}

}//namespace SimpleReflection
