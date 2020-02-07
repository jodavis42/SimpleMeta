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

  bool operator==(const Vec2& rhs) const = default;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Vec2, x);
    BindField(library, boundType, Vec2, y);
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
  
  bool operator==(const Vec3& rhs) const = default;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Vec3, x);
    BindField(library, boundType, Vec3, y);
    BindField(library, boundType, Vec3, z);
  }

  float x;
  float y;
  float z;
};

struct Vertex
{
  Vertex()
  {
  }
  Vertex(const Vec3& position, const Vec2& uv)
    : mUv(uv)
    , mPosition(position)
  {
  }

  bool operator==(const Vertex& rhs) const = default;

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
  bool operator==(const Mesh& rhs) const = default;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Mesh, mVertices);
    BindField(library, boundType, Mesh, mName);
  }

  std::vector<Vertex> mVertices;
  std::string mName;
};

struct NameId
{
  NameId()
  {
  }
  NameId(int id, const std::string& name) : mId(id), mName(name)
  {
  }

  bool operator==(const NameId& rhs) const = default;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameId, mId);
    BindField(library, boundType, NameId, mName);
  }

  int mId;
  std::string mName;
};

struct NameIdList
{
  bool operator==(const NameIdList& rhs) const = default;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameIdList, mIds);
  }

  std::vector<NameId> mIds;
};

struct Collider
{
  bool operator==(const Collider& rhs) const = default;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Collider, mId);
  }

  int mId;
};

struct BoxCollider : public Collider
{
  bool operator==(const BoxCollider& rhs) const = default;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindField(library, boundType, BoxCollider, mSize);
  }

  Vec3 mSize;
};

struct SphereCollider : public Collider
{
  bool operator==(const SphereCollider& rhs) const = default;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindField(library, boundType, SphereCollider, mRadius);
  }
  float mRadius;
};

struct PhysicsSpace
{
  bool operator==(const PhysicsSpace& rhs) const
  {
    return false;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, PhysicsSpace, mColliders);
  }

  std::vector<Collider*> mColliders;
};

struct Dictionary
{
  bool operator==(const Dictionary& rhs) const = default;
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Dictionary, mData);
  }
  
  void Add(const std::string& key, const std::string& value)
  {
    mData.insert(std::make_pair(key, value));
  }

  std::unordered_map<std::string, std::string> mData;
};

struct MyStruct
{
  int mData;
  float mFloat;
  char mChar1;
  int value = 5;
  char mChar2;
  double mDouble1;
  std::string mString1;

  bool operator==(const MyStruct& rhs) const = default;
};

void BindMyStruct(MetaLibrary& library, BoundType& boundType)
{
  BindField(library, boundType, MyStruct, mData);
  BindField(library, boundType, MyStruct, mFloat);
  BindField(library, boundType, MyStruct, mChar1);
  BindField(library, boundType, MyStruct, mChar2);
  BindField(library, boundType, MyStruct, mDouble1);
  BindField(library, boundType, MyStruct, mString1);
}