#pragma once

struct ReflectionLibrary;
struct BoundType;

struct ReflectionComponent
{
  virtual ~ReflectionComponent() {}

  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {

  }
};
