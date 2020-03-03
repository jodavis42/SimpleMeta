#include "Attributes.hpp"
#include "StaticTypeId.hpp"

SimpleReflection::BoundType* Attribute::GetBoundType()
{
  return SimpleReflection::StaticTypeId<Attribute>::GetBoundType();
}

SimpleReflection::BoundType* SerializedAttribute::GetBoundType()
{
  return SimpleReflection::StaticTypeId<SerializedAttribute>::GetBoundType();
}
