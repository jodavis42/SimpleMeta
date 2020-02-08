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

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Vertex, mUv);
    BindField(library, boundType, Vertex, mPosition);
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

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Mesh, mVertices);
    BindField(library, boundType, Mesh, mName);
  }

  std::vector<Vertex> mVertices;
  std::string mName;
};
