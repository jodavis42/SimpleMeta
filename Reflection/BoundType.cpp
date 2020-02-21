#include "BoundType.hpp"

#include "Function.hpp"
#include "Call.hpp"

char* Field::GetFieldData(char* instanceData) const
{
  return instanceData + mOffset;
}

GetterSetter::~GetterSetter()
{
  delete mGetter;
  delete mSetter;
}

Any GetterSetter::Get(char* instanceData) const
{
  if(mGetter == nullptr)
    return Any();

  Call call(mGetter);
  call.SetPointerUnchecked(Call::This, instanceData);
  call.Invoke();
  char* resultData = call.GetLocationUnChecked(Call::Return);
  return Any(resultData, mGetter->GetReturnType());
}

void GetterSetter::Set(char* instanceData, Any any)
{
  if(mSetter == nullptr)
    return;

  Call call(mSetter);
  call.SetPointerUnchecked(Call::This, instanceData);
  call.SetValueUnchecked(0, any.GetRawData(), any.GetStoredType()->mSizeInBytes);
  call.Invoke();
}

BoundType::~BoundType()
{
  delete mDefaultConstructor;
  delete mCopyConstructor;
  delete mDestructor;
  for(Field* field : mFields)
    delete field;
  mFields.clear();
  for(GetterSetter* getSet : mGetterSetters)
    delete getSet;
  mGetterSetters.clear();
}
