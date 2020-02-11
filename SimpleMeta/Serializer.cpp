#include "Serializer.hpp"

#include "MetaSerialization.hpp"

bool Serializer::SerializeProperties(BoundType& boundType, char* data)
{
  for(size_t i = 0; i < boundType.mFields.size(); ++i)
  {
    const Field& field = boundType.mFields[i];
    BoundType* fieldType = field.mType;
    char* fieldSrc = data + field.mOffset;

    MetaSerialization* fieldSerialization = fieldType->mMetaSerialization;
    if(fieldSerialization != nullptr && BeginMember(field.mName))
    {
      fieldSerialization->Serialize(*this, *fieldType, fieldSrc);
      EndMember();
    }
  }

  return true;
}

bool Serializer::SerializeObject(BoundType& boundType, char* data)
{
  return SerializeProperties(boundType, data);
}
