#pragma once

#include "BoundType.hpp"
#include "Field.hpp"
#include "GetterSetter.hpp"
#include "Attributes.hpp"
#include "Serializer.hpp"
#include "MetaSerialization.hpp"

using SimpleReflection::Field;
using SimpleReflection::GetterSetter;

template <typename SerializerType>
bool SerializeProperties(SerializerType& serializer, BoundType& boundType, char* data)
{
  for(auto range = boundType.GetFields(); !range.Empty(); range.PopFront())
  {
    //const Field* field = boundType.mFields[i];
    const Field* field = range.Front();
    // Only serialize fields with the correct attribute
    if(!field->QueryComponentType<SerializedAttribute>())
      continue;

    BoundType* fieldType = field->mType;
    char* fieldSrc = field->GetFieldData(data);

    MetaSerialization* metaSerialization = fieldType->QueryComponentType<MetaSerialization>();
    if(metaSerialization != nullptr && serializer.BeginMember(field->mName))
    {
      metaSerialization->Serialize(serializer, *fieldType, fieldSrc);
      serializer.EndMember();
    }
  }
  for(auto range = boundType.GetGetterSetters(); !range.Empty(); range.PopFront())
  {
    GetterSetter* getSet = range.Front();
    // Only serialize fields with the correct attribute
    if(!getSet->QueryComponentType<SerializedAttribute>())
      continue;

    MetaSerialization* metaSerialization = getSet->mType->QueryComponentType<MetaSerialization>();
    if(metaSerialization != nullptr && serializer.BeginMember(getSet->mName))
    {
      metaSerialization->SerializeProperty(serializer, *getSet, data);
      serializer.EndMember();
    }
  }
  return true;
}

template <typename SerializerType>
bool SerializeObject(SerializerType& serializer, BoundType& boundType, char* data)
{
  return SerializeProperties(serializer, boundType, data);
}
