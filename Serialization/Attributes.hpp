#pragma once

#include <string>
#include <vector>
#include "ReflectionComponent.hpp"

struct Attribute : public ReflectionComponent
{
  Attribute() {}
  Attribute(const std::string& name) : mName(name) {}

  virtual BoundType* GetBoundType() override;

  std::string mName;
};

struct SerializedAttribute : public Attribute
{
  SerializedAttribute() : Attribute("Serialized") {}

  virtual BoundType* GetBoundType() override;
};
