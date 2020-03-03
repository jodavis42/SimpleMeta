#include "Precompiled.hpp"

#include "GetterSetter.hpp"

#include "Asserts.hpp"
#include "Function.hpp"
#include "Call.hpp"

namespace SimpleReflection
{

GetterSetter::~GetterSetter()
{
  delete mGetter;
  delete mSetter;
}

Any GetterSetter::Get(char* instanceData) const
{
  if(mGetter == nullptr)
    return Any();

  // Set the 'this' then invoke the call
  Call call(mGetter);
  call.SetPointerUnchecked(Call::This, instanceData);
  call.Invoke();

  // Store the result of into the any and return
  char* resultData = call.GetLocationUnChecked(Call::Return);
  return Any(resultData, mGetter->GetReturnType());
}

void GetterSetter::Set(char* instanceData, Any any)
{
  if(mSetter == nullptr)
    return;

  // Set the 'this' and the any arg then invoke
  Call call(mSetter);
  call.SetPointerUnchecked(Call::This, instanceData);
  call.SetValueUnchecked(0, any.GetRawData(), any.GetStoredType()->mSizeInBytes);
  call.Invoke();
}

}//namespace SimpleReflection
