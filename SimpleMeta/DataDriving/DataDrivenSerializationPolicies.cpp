#include "DataDrivenSerializationPolicies.hpp"

GenericBoundTypeMetaSerialization::GenericBoundTypeMetaSerialization(BoundType* boundtype)
  : mBoundType(boundtype)
{

}

bool GenericBoundTypeMetaSerialization::SerializeBase(Serializer& serializer, BoundType& boundType, char* data)
{
  return serializer.SerializeObject(boundType, data);
}

char* GenericBoundTypeMetaSerialization::Allocate() const
{
  return (char*)malloc(mBoundType->mSizeInBytes);
}


GenericArrayBoundTypeMetaSerialization::GenericArrayBoundTypeMetaSerialization(BoundType* boundtype)
  : mArrayTemplateBoundType(boundtype)
{

}

bool GenericArrayBoundTypeMetaSerialization::SerializeBase(Serializer& serializer, BoundType& boundType, char* data)
{
  MetaSerialization* metaSerialization = mArrayTemplateBoundType->QueryComponentType<MetaSerialization>();

  std::vector<char>& array = *(std::vector<char>*)data;
  if(serializer.mDirection == SerializerDirection::Saving)
  {
    size_t count = array.size() / mArrayTemplateBoundType->mSizeInBytes;
    serializer.BeginArray(count);
    for(size_t i = 0; i < count; ++i)
    {
      serializer.BeginArrayItem(i);
      metaSerialization->Serialize(serializer, *mArrayTemplateBoundType, &array[i * mArrayTemplateBoundType->mSizeInBytes]);
      serializer.EndArrayItem();
    }
    serializer.EndArray();
  }
  else
  {
    new (data)(std::vector<char*>);
    size_t count;
    serializer.BeginArray(count);
    array.resize(count * mArrayTemplateBoundType->mSizeInBytes);
    for(size_t i = 0; i < count; ++i)
    {
      serializer.BeginArrayItem(i);
      metaSerialization->Serialize(serializer, *mArrayTemplateBoundType, &array[i * mArrayTemplateBoundType->mSizeInBytes]);
      serializer.EndArrayItem();
    }
    serializer.EndArray();
  }
  return true;
}

char* GenericArrayBoundTypeMetaSerialization::Allocate() const
{
  return (char*)new std::vector<char*>();
}
