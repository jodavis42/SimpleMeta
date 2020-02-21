#include "Serializer.hpp"

#include "MetaSerialization.hpp"
#include "SerializationHelpers.hpp"

bool Serializer::SerializeObject(BoundType& boundType, char* data)
{
  return SerializeProperties(*this, boundType, data);
}
