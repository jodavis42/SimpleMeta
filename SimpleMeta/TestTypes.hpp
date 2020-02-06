#pragma once

#include "Bindings.hpp"

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
  float x;
  float y;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Vec2, x);
    BindField(library, boundType, Vec2, y);
  }
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
  float x;
  float y;
  float z;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Vec3, x);
    BindField(library, boundType, Vec3, y);
    BindField(library, boundType, Vec3, z);
  }
};

struct Vertex
{
  Vertex()
  {
  }
  Vertex(const Vec3& position, const Vec2& uv) : mUv(uv), mPosition(position)
  {
  }

  Vec2 mUv;
  Vec3 mPosition;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Vertex, mUv);
    BindField(library, boundType, Vertex, mPosition);
  }
};

struct Mesh
{
  std::vector<Vertex> mVertices;
  std::string mName;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Mesh, mVertices);
    BindField(library, boundType, Mesh, mName);
  }
};

struct NameId
{
  NameId()
  {
  }
  NameId(int id, const std::string& name) : mId(id), mName(name)
  {
  }
  int mId;
  std::string mName;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameId, mId);
    BindField(library, boundType, NameId, mName);
  }
};

struct NameIdList
{
  std::vector<NameId> mIds;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameIdList, mIds);
  }
};

struct Collider
{
  int mId;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Collider, mId);
  }
};

struct BoxCollider : public Collider
{
  Vec3 mSize;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindField(library, boundType, BoxCollider, mSize);
  }
};

struct SphereCollider : public Collider
{
  float mRadius;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindField(library, boundType, SphereCollider, mRadius);
  }
};

struct PhysicsSpace
{
  std::vector<Collider*> mColliders;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, PhysicsSpace, mColliders);
  }
};

struct Dictionary
{
  std::unordered_map<std::string, std::string> mData;
  void Add(const std::string& key, const std::string& value)
  {
    mData.insert(std::make_pair(key, value));
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Dictionary, mData);
  }
};
