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

  virtual BoundType* VirtualGetBoundType() const
  {
    return StaticTypeId<BoxCollider>::GetBoundType();
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    boundType.mMetaSerialization = new ObjectMetaSerialization<BoxCollider>();
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
  virtual BoundType* VirtualGetBoundType() const
  {
    return StaticTypeId<SphereCollider>::GetBoundType();
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    boundType.mMetaSerialization = new ObjectMetaSerialization<SphereCollider>();
    BindBase(library, boundType, Collider);
    BindField(library, boundType, SphereCollider, mRadius);
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
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    Field* field = BindField(library, boundType, PhysicsSpace, mColliders);
    field->mType->mMetaSerialization = new PolymorphicArrayMetaSerialization<Collider*>();
  }

  std::vector<Collider*> mColliders;
};
