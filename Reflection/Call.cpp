#include "Precompiled.hpp"

#include "Call.hpp"

#include "Asserts.hpp"
#include "BoundType.hpp"
#include "Function.hpp"
#include "Any.hpp"

namespace SimpleReflection
{

Call::Call(Function* function)
{
  mFunction = function;
  mBufferSizeInBytes = function->ComputeSizeRequired();
  mBuffer = new char[mBufferSizeInBytes];
}

Call::~Call()
{
  delete mBuffer;
  mBuffer = nullptr;
}

void Call::SetValue(int index, const void* data, BoundType* boundType)
{
  char* location = GetLocationChecked(index, boundType);
  memcpy(location, data, boundType->mSizeInBytes);
}

void Call::SetValueUnchecked(int index, const void* data, size_t size)
{
  char* location = GetLocationUnChecked(index);
  memcpy(location, data, size);
}

void Call::SetPointer(int index, const void* dataPtr, BoundType* boundType)
{
  SetValue(index, &dataPtr, boundType);
}

void Call::SetPointerUnchecked(int index, const void* dataPtr)
{
  SetValueUnchecked(index, &dataPtr, sizeof(&dataPtr));
}

void Call::Set(int index, Any& any)
{
  // Set the value from the any based upon the expected type
  BoundType* expectedType = GetLocationType(index);
  char* rawData = any.GetRawData();
  if(expectedType->mIsReferenceType)
  {
    char* location = GetLocationUnChecked(index);
    SetInternal(index, rawData, expectedType->mSizeInBytes);
  }
  else
  {
    char* location = GetLocationUnChecked(index);
    SetInternal(index, *rawData, expectedType->mSizeInBytes);
  }
}

char* Call::GetLocationChecked(int index, BoundType* boundType)
{
  BoundType* expectedType = GetLocationType(index);
  ReflectionErrorIf(boundType != expectedType, "Invalid expected type");
  return GetLocationUnChecked(index);
}

char* Call::GetLocationUnChecked(int index)
{
  size_t offset = GetLocationOffset(index);
  return mBuffer + offset;
}

size_t Call::GetLocationOffset(int index) const
{
  size_t offset = 0;
  if(mFunction->GetReturnType() != nullptr && index > Return)
    offset += mFunction->GetReturnType()->mSizeInBytes;
  if(!mFunction->IsStatic() && index > This)
    offset += mFunction->GetThisType()->mSizeInBytes;
  for(int i = 0; i < index; ++i)
    offset += mFunction->GetParamType(i)->mSizeInBytes;
  return offset;
}

BoundType* Call::GetLocationType(int index) const
{
  BoundType* expectedType = nullptr;
  if(index == Return)
    expectedType = mFunction->GetReturnType();
  else if(index == This)
    expectedType = mFunction->GetThisType();
  else
    expectedType = mFunction->GetParamType(index);
  return expectedType;
}

bool Call::Invoke()
{
  mFunction->mBoundFunction(*this);
  return true;
}

}//namespace SimpleReflection
