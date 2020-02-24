#pragma once

#include "Math.hpp"

struct Collider
{
  enum
  {
    GhostBit = 1 << 0,
    SendsEventsBit = 1 << 1
  };

  bool operator==(const Collider& rhs) const
  {
    return mId == rhs.mId && mFlags == rhs.mFlags;
  }

  virtual BoundType* VirtualGetBoundType() const
  {
    return StaticTypeId<Collider>::GetBoundType();
  }
  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindProperty(library, boundType, Collider, mId);
    BindGetterSetter(library, boundType, Collider, Ghost);
    BindGetterSetterAs(library, boundType, Collider, "SendsEvents", GetSendsEvents, SetSendsEvents);
  }

  bool GetGhost() const
  {
    return mFlags & GhostBit;
  }
  void SetGhost(bool state)
  {
    if(state)
      mFlags |= GhostBit;
    else
      mFlags &= ~GhostBit;
  }
  bool GetSendsEvents() const
  {
    return mFlags & SendsEventsBit;
  }
  void SetSendsEvents(bool state)
  {
    if(state)
      mFlags |= SendsEventsBit;
    else
      mFlags &= ~SendsEventsBit;
  }

  int mFlags = 0;
  int mId = 0;
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
  ~PhysicsSpace()
  {
    for(Collider* collider : mColliders)
    {
      delete collider;
    }
  }
  bool operator==(const PhysicsSpace& rhs) const
  {
    if(mColliders.size() != rhs.mColliders.size())
      return false;

    bool isEqual = true;
    for(size_t i = 0; i < mColliders.size(); ++i)
    {
      isEqual &= (*mColliders[i] == *rhs.mColliders[i]);
    }
    return isEqual;
  }
  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindProperty(library, boundType, PhysicsSpace, mColliders);
  }

  std::vector<Collider*> mColliders;
};
