#include "MetaSerialization.hpp"

#include "JsonSerializers.hpp"
#include "BinaryStream.hpp"

BoundType* MetaSerialization::GetBoundType()
{
  return StaticTypeId<MetaSerialization>::GetBoundType();
}

bool MetaSerialization::Serialize(Serializer& serializer, BoundType& boundType, char* instanceData)
{
  return SerializeBase(serializer, boundType, instanceData);
}

bool MetaSerialization::Serialize(JsonSaver& serializer, BoundType& boundType, char* instanceData)
{
  return SerializeBase(serializer, boundType, instanceData);
}

bool MetaSerialization::Serialize(JsonLoader& serializer, BoundType& boundType, char* instanceData)
{
  return SerializeBase(serializer, boundType, instanceData);
}

bool MetaSerialization::Serialize(BinarySaver& serializer, BoundType& boundType, char* instanceData)
{
  return SerializeBase(serializer, boundType, instanceData);
}

bool MetaSerialization::Serialize(BinaryLoader& serializer, BoundType& boundType, char* instanceData)
{
  return SerializeBase(serializer, boundType, instanceData);
}

bool MetaSerialization::SerializeProperty(Serializer& serializer, GetterSetter& getSet, char* instanceData)
{
  return SerializePropertyBase(serializer, getSet, instanceData);
}

bool MetaSerialization::SerializeProperty(JsonSaver& serializer, GetterSetter& getSet, char* instanceData)
{
  return SerializePropertyBase(serializer, getSet, instanceData);
}

bool MetaSerialization::SerializeProperty(JsonLoader& serializer, GetterSetter& getSet, char* instanceData)
{
  return SerializePropertyBase(serializer, getSet, instanceData);
}

bool MetaSerialization::SerializeProperty(BinarySaver& serializer, GetterSetter& getSet, char* instanceData)
{
  return SerializePropertyBase(serializer, getSet, instanceData);
}

bool MetaSerialization::SerializeProperty(BinaryLoader& serializer, GetterSetter& getSet, char* instanceData)
{
  return SerializePropertyBase(serializer, getSet, instanceData);
}
