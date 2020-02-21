#pragma once

#include "BoundType.hpp"
#include "Attributes.hpp"
#include "Serializer.hpp"
#include "MetaSerialization.hpp"

template <typename SerializerType>
bool SerializeProperties(SerializerType& serializer, BoundType& boundType, char* data)
{
  for(size_t i = 0; i < boundType.mFields.size(); ++i)
  {
    const Field* field = boundType.mFields[i];
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
  for(size_t i = 0; i < boundType.mGetterSetters.size(); ++i)
  {
    GetterSetter* getSet = boundType.mGetterSetters[i];
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
