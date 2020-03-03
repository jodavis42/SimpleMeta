#pragma once

#include <string>
#include <vector>
#include "ReflectionComponent.hpp"

struct Attribute : public SimpleReflection::ReflectionComponent
{
  Attribute() {}
  Attribute(const std::string& name) : mName(name) {}

  virtual SimpleReflection::BoundType* GetBoundType() override;

  std::string mName;
};

struct SerializedAttribute : public Attribute
{
  SerializedAttribute() : Attribute("Serialized") {}

  virtual SimpleReflection::BoundType* GetBoundType() override;
};
