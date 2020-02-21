#include "Call.hpp"

#include "BoundType.hpp"
#include "Function.hpp"

Call::Call(Function* function)
{
  mFunction = function;
  mBuffer = new char[function->ComputeSizeRequired()];
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

char* Call::GetLocationChecked(int index, BoundType* boundType)
{
  BoundType* expectedType = nullptr;
  if(index == Return)
    expectedType = mFunction->GetReturnType();
  else if(index == This)
    expectedType = mFunction->GetThisType();
  else
    expectedType = mFunction->GetParamType(index);
  ErrorIf(boundType != expectedType, "Invalid expected type");
  return GetLocationUnChecked(index);
}

char* Call::GetLocationUnChecked(int index)
{
  size_t offset = 0;
  if(mFunction->GetReturnType() != nullptr && index > Return)
    offset += mFunction->GetReturnType()->mSizeInBytes;
  if(!mFunction->IsStatic() && index > This)
    offset += mFunction->GetThisType()->mSizeInBytes;
  for(int i = 0; i < index; ++i)
    offset += mFunction->GetParamType(i)->mSizeInBytes;
  return mBuffer + offset;
}

bool Call::Invoke()
{
  mFunction->mBoundFunction(*this);
  return true;
}
