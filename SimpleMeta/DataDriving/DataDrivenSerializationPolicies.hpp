#pragma once

#include "BoundType.hpp"
#include "MetaSerialization.hpp"
#include "ContainerSerializationPolicies.hpp"

struct GenericBoundTypeMetaSerialization : public MetaSerialization
{
  GenericBoundTypeMetaSerialization(BoundType* boundtype);

  virtual bool SerializeBase(Serializer& serializer, BoundType& boundType, char* data) override;
  virtual char* Allocate() const override;

  BoundType* mBoundType;
};

struct GenericArrayBoundTypeMetaSerialization : public MetaSerialization
{
  GenericArrayBoundTypeMetaSerialization(BoundType* boundtype);

  virtual bool SerializeBase(Serializer& serializer, BoundType& boundType, char* data) override;
  virtual char* Allocate() const override;

  BoundType* mArrayTemplateBoundType;
};