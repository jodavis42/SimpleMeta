#pragma once

#include "Math.hpp"

struct Collider
{
  bool operator==(const Collider& rhs) const
  {
    return mId == rhs.mId;
  }

  virtual BoundType* VirtualGetBoundType() const
  {
    return StaticTypeId<Collider>::GetBoundType();
  }
  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindProperty(library, boundType, Collider, mId);
  }

  int mId;
};

struct BoxCollider : public Collider
{
  bool operator==(const BoxCollider& rhs) const
  {
    return __super::operator==(rhs) && mSize == rhs.mSize;
  }

  virtual BoundType* VirtualGetBoundType() const
  {
    return StaticTypeId<BoxCollider>::GetBoundType();
  }
  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindProperty(library, boundType, BoxCollider, mSize);
  }

  Vec3 mSize;
};

struct SphereCollider : public Collider
{
  bool operator==(const SphereCollider& rhs) const
  {
    return __super::operator==(rhs) && mRadius == rhs.mRadius;
  }
  virtual BoundType* VirtualGetBoundType() const
  {
    return StaticTypeId<SphereCollider>::GetBoundType();
  }
  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindProperty(library, boundType, SphereCollider, mRadius);
  }
  float mRadius;
};

struct PhysicsSpace
{
  bool operator==(const PhysicsSpace& rhs) const
  {
    if(mColliders.size() != rhs.mColliders.size())
      return false;

    bool isEqual = true;
    for(size_t i = 0; i < mColliders.size(); ++i)
    {
      isEqual |= (*mColliders[i] == *rhs.mColliders[i]);
    }
    return isEqual;
  }
  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindProperty(library, boundType, PhysicsSpace, mColliders);
  }

  std::vector<Collider*> mColliders;
};
