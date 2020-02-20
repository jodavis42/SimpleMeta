#pragma once

#include <unordered_map>
#include "ReflectionComponent.hpp"
#include <memory>
//#include "StaticTypeId.hpp"

struct BoundType;
struct TypeId;

struct ReflectionComposition
{
  virtual ~ReflectionComposition();

  bool AddComponent(std::shared_ptr<ReflectionComponent> component);
  ReflectionComponent* FindComponent(const TypeId& typeId);
  const ReflectionComponent* FindComponent(const TypeId& typeId) const;
  bool RemoveComponent(const TypeId& typeId);
  void ClearComponents();

  template <typename ComponentType, typename ... ArgTypes>
  bool AddComponentType(ArgTypes ... args)
  {
    BoundType* boundType = StaticTypeId<ComponentType>::GetBoundType();
    return AddComponent(std::make_shared<ComponentType>(args...));
  }
  template <typename ComponentType, typename ... ArgTypes>
  ReflectionComposition* AddComponentTypeChainable(ArgTypes ... args)
  {
    AddComponentType<ComponentType>(args...);
    return this;
  }
  template <typename ComponentType>
  ComponentType* QueryComponentType()
  {
    BoundType* boundType = StaticTypeId<ComponentType>::GetBoundType();
    return static_cast<ComponentType*>(FindComponent(boundType->mId));
  }
  template <typename ComponentType>
  const ComponentType* QueryComponentType() const
  {
    BoundType* boundType = StaticTypeId<ComponentType>::GetBoundType();
    return static_cast<const ComponentType*>(FindComponent(boundType->mId));
  }

private:
  std::unordered_map<size_t, std::shared_ptr<ReflectionComponent>> mComponents;
};
