#include "Any.hpp"

#include "BoundType.hpp"
#include "Call.hpp"

Any::Any()
{
  Clear();
}

Any::Any(const Any& any)
{
  AssignFrom(any.GetRawData(), any.mStoredType);
}

Any::Any(const void* data, BoundType* boundType)
{
  AssignFrom(data, boundType);
}

Any::~Any()
{
  Clear();
}

char* Any::GetRawData()
{
  if(mIsAllocated == false)
    return mBuffer;
  return *((char**)mBuffer);
}

const char* Any::GetRawData() const
{
  if(mIsAllocated == false)
    return mBuffer;
  return *((char**)mBuffer);
}

bool Any::IsStoredType(const BoundType* boundType) const
{
  return mStoredType == boundType;
}

BoundType* Any::GetStoredType() const
{
  return mStoredType;
}

void Any::AssignFrom(const void* data, BoundType* boundType)
{
  ErrorIf(boundType == nullptr, "Cannot store a null bound type");

  // Clear any old values
  Clear();

  mStoredType = boundType;
  char* rawDataStart = mBuffer;
  if(boundType->mSizeInBytes < mBufferSize)
  {
    mIsAllocated = false;
  }
  else
  {
    rawDataStart = new char[boundType->mSizeInBytes];
    *((char**)mBuffer) = rawDataStart;
    mIsAllocated = true;
  }

  if(boundType->mCopyConstructor)
  {
    Call call(boundType->mCopyConstructor);
    call.SetValueUnchecked(Call::This, &rawDataStart, sizeof(void*));
    call.SetValueUnchecked(0, data, boundType->mSizeInBytes);
    call.Invoke();
  }
  else
  {
    memcpy(rawDataStart, data, boundType->mSizeInBytes);
  }
}

void Any::operator=(const Any& any)
{
  AssignFrom(any.GetRawData(), any.mStoredType);
}

bool Any::operator==(const Any& any) const
{
  if(mStoredType != any.mStoredType)
    return false;

  const char* myData = GetRawData();
  const char* otherData = any.GetRawData();
  int state = memcmp(myData, otherData, mStoredType->mSizeInBytes);
  return state == 0;
}

void Any::Clear()
{
  char* bufferData = GetRawData();

  // Needs to invoke the destructor on bufferData if it exists
  if(mStoredType != nullptr && mStoredType->mDestructor != nullptr)
  {
    Call call(mStoredType->mDestructor);
    call.SetValueUnchecked(Call::This, &bufferData, sizeof(void*));
    call.Invoke();
  }

  // If the data was actually allocated, then free the extra memory
  if(mIsAllocated)
    delete bufferData;

  memset(mBuffer, 0, mBufferSize);
  mIsAllocated = false;
}
