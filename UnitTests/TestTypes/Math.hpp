#pragma once

#include "Bindings.hpp"
#include "Attributes.hpp"

struct Vec2
{
  Vec2()
  {
  }

  Vec2(float _x, float _y)
  {
    x = _x;
    y = _y;
  }

  bool operator==(const Vec2& rhs) const
  {
    return x == rhs.x && y == rhs.y;
  }

  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, Vec2, x);
    BindProperty(library, boundType, Vec2, y);
  }

  float x;
  float y;
};

struct Vec3
{
  Vec3()
  {
  }
  Vec3(float _x, float _y, float _z)
  {
    x = _x;
    y = _y;
    z = _z;
  }

  bool operator==(const Vec3& rhs) const
  {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }

  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, Vec3, x);
    BindProperty(library, boundType, Vec3, y);
    BindProperty(library, boundType, Vec3, z);
  }

  float x;
  float y;
  float z;
};
