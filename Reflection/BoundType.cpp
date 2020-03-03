#include "BoundType.hpp"

#include "Field.hpp"
#include "GetterSetter.hpp"
#include "Function.hpp"

namespace SimpleReflection
{

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

  for(auto&& pair : mFunctionMap)
  {
    for(Function* function : pair.second)
    {
      delete function;
    }
  }
  mFunctionMap.clear();
}

}//namespace SimpleReflection
