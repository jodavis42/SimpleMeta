#include "DataDrivenTypes.hpp"

#include "Bindings.hpp"
#include "MetaSerialization.hpp"
#include "ContainerSerializationPolicies.hpp"

void DataDrivenField::Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
{
  BindFieldAs(library, boundType, DataDrivenField, mName, "Name");
  BindFieldAs(library, boundType, DataDrivenField, mType, "Type");
}

void DataDrivenType::Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
{
  BindFieldAs(library, boundType, DataDrivenType, mName, "Name");
  BindFieldAs(library, boundType, DataDrivenType, mBaseType, "BaseType");
  BindFieldAs(library, boundType, DataDrivenType, mFields, "Fields");
}

void DataDrivenTypes::Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
{
  BindFieldAs(library, boundType, DataDrivenTypes, mTypes, "Types");
}
