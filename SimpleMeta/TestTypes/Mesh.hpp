#pragma once

#include "Math.hpp"

struct Vertex
{
  Vertex()
  {
  }
  Vertex(const Vec3& position, const Vec2& uv) : mUv(uv), mPosition(position)
  {
  }

  bool operator==(const Vertex& rhs) const
  {
    return mUv == rhs.mUv && mPosition == rhs.mPosition;
  }

  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindProperty(library, boundType, Vertex, mUv);
    BindProperty(library, boundType, Vertex, mPosition);
  }

  Vec2 mUv;
  Vec3 mPosition;
};

struct Mesh
{
  bool operator==(const Mesh& rhs) const
  {
    return mVertices == rhs.mVertices && mName == rhs.mName;
  }

  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindProperty(library, boundType, Mesh, mVertices);
    BindProperty(library, boundType, Mesh, mName);
  }

  std::vector<Vertex> mVertices;
  std::string mName;
};
