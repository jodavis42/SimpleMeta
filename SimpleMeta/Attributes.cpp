#include "Attributes.hpp"
#include "StaticTypeId.hpp"

BoundType* Attribute::GetBoundType()
{
  return StaticTypeId<Attribute>::GetBoundType();
}

BoundType* SerializedAttribute::GetBoundType()
{
  return StaticTypeId<SerializedAttribute>::GetBoundType();
}
