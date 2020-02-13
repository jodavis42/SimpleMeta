#include "Serializer.hpp"

#include "MetaSerialization.hpp"

bool Serializer::SerializeObject(BoundType& boundType, char* data)
{
  return SerializeProperties(*this, boundType, data);
}
