#pragma once

#include "Math.hpp"

struct Collider
{
  bool operator==(const Collider& rhs) const
  {
    return mId == rhs.mId;
  }

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Collider, mId);
  }

  int mId;
};

struct BoxCollider : public Collider
{
  bool operator==(const BoxCollider& rhs) const
  {
    return __super::operator==(rhs) && mSize == rhs.mSize;
  }

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindField(library, boundType, BoxCollider, mSize);
  }

  Vec3 mSize;
};

struct SphereCollider : public Collider
{
  bool operator==(const SphereCollider& rhs) const
  {
    return __super::operator==(rhs) && mRadius == rhs.mRadius;
  }
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
