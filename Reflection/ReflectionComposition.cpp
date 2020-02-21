#include "ReflectionComposition.hpp"

#include "BoundType.hpp"

ReflectionComposition::~ReflectionComposition()
{
  ClearComponents();
}

bool ReflectionComposition::AddComponent(ReflectionComponentPtr component)
{
  BoundType* boundType = component->GetBoundType();
  TypeId typeId = boundType->mId;
  if(mComponents.find(typeId.mId) != mComponents.end())
    return false;

  mComponents[typeId.mId] = component;
  return true;
}

ReflectionComponent* ReflectionComposition::FindComponent(const TypeId& typeId)
{
  auto it = mComponents.find(typeId.mId);
  if(it != mComponents.end())
    return it->second.get();
  return nullptr;
}

const ReflectionComponent* ReflectionComposition::FindComponent(const TypeId& typeId) const
{
  auto it = mComponents.find(typeId.mId);
  if(it != mComponents.end())
    return it->second.get();
  return nullptr;
}

bool ReflectionComposition::RemoveComponent(const TypeId& typeId)
{
  auto it = mComponents.find(typeId.mId);
  if(it == mComponents.end())
    return false;

  mComponents.erase(typeId.mId);
  return true;
}

void ReflectionComposition::ClearComponents()
{
  mComponents.clear();
}
