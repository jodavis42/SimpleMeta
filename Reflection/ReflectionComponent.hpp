#pragma once

struct ReflectionLibrary;
struct BoundType;

struct ReflectionComponent
{
  virtual ~ReflectionComponent() {}

  virtual BoundType* GetBoundType() abstract;

  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {

  }
};
