#include "BoundType.hpp"

#include "Field.hpp"
#include "GetterSetter.hpp"

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
