#include "MetaSerialization.hpp"

#include "JsonSerializers.hpp"
#include "BinaryStream.hpp"

bool MetaSerialization::Serialize(Serializer& serializer, BoundType& boundType, char* data)
{
  return SerializeBase(serializer, boundType, data);
}

bool MetaSerialization::Serialize(JsonSaver& serializer, BoundType& boundType, char* data)
{
  return SerializeBase(serializer, boundType, data);
}

bool MetaSerialization::Serialize(JsonLoader& serializer, BoundType& boundType, char* data)
{
  return SerializeBase(serializer, boundType, data);
}

bool MetaSerialization::Serialize(BinarySaver& serializer, BoundType& boundType, char* data)
{
  return SerializeBase(serializer, boundType, data);
}

bool MetaSerialization::Serialize(BinaryLoader& serializer, BoundType& boundType, char* data)
{
  return SerializeBase(serializer, boundType, data);
}
