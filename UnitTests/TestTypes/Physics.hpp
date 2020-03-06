#pragma once

#include "Math.hpp"

enum class ColliderType
{
  None,
  Sphere,
  Box
};

struct Collider
{
  enum
  {
    GhostBit = 1 << 0,
    SendsEventsBit = 1 << 1
  };

  bool operator==(const Collider& rhs) const
  {
    return IsEqual(rhs);
  }
  virtual bool IsEqual(const Collider& rhs) const
  {
    return mId == rhs.mId && mFlags == rhs.mFlags;
  }
  virtual ColliderType GetType() const
  {
    return ColliderType::None;
  }

  virtual BoundType* VirtualGetBoundType() const
  {
    return SimpleReflection::StaticTypeId<Collider>::GetBoundType();
  }
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
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
  virtual bool IsEqual(const Collider& rhs) const override
  {
    if(rhs.GetType() != GetType())
      return false;
    const BoxCollider& castedRhs = reinterpret_cast<const BoxCollider&>(rhs);
    return IsEqual(castedRhs);
  }
  bool IsEqual(const BoxCollider& rhs) const
  {
    return __super::IsEqual(rhs) && mSize == rhs.mSize;
  }
  virtual ColliderType GetType() const override
  {
    return ColliderType::Box;
  }

  virtual BoundType* VirtualGetBoundType() const
  {
    return SimpleReflection::StaticTypeId<BoxCollider>::GetBoundType();
  }
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindBase(library, boundType, Collider);
    BindProperty(library, boundType, BoxCollider, mSize);
  }

  Vec3 mSize;
};

struct SphereCollider : public Collider
{
  virtual bool IsEqual(const Collider& rhs) const override
  {
    if(rhs.GetType() != GetType())
      return false;
    const SphereCollider& castedRhs = reinterpret_cast<const SphereCollider&>(rhs);
    return IsEqual(castedRhs);
  }
  bool IsEqual(const SphereCollider& rhs) const
  {
    return __super::IsEqual(rhs) && mRadius == rhs.mRadius;
  }

  virtual ColliderType GetType() const override
  {
    return ColliderType::Sphere;
  }

  virtual BoundType* VirtualGetBoundType() const
  {
    return SimpleReflection::StaticTypeId<SphereCollider>::GetBoundType();
  }
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
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
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, PhysicsSpace, mColliders);
  }

  std::vector<Collider*> mColliders;
};
