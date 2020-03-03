#pragma once

#include <unordered_map>
#include <memory>

#include "ReflectionComponent.hpp"

namespace SimpleReflection
{

struct BoundType;
struct TypeId;

/// A composition for reflection objects. The composition is expected to own the lifetime of the added components (albeit via shared pointers).
struct ReflectionComposition
{
  using ReflectionComponentPtr = std::shared_ptr<ReflectionComponent>;

  virtual ~ReflectionComposition();

  bool AddComponent(ReflectionComponentPtr component);
  ReflectionComponent* FindComponent(const TypeId& typeId);
  const ReflectionComponent* FindComponent(const TypeId& typeId) const;
  bool RemoveComponent(const TypeId& typeId);
  void ClearComponents();

  // Template helpers to auto cast to the correct types
  template <typename ComponentType, typename ... ArgTypes>
  bool AddComponentType(ArgTypes ... args)
  {
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
  using ComponentMap = std::unordered_map<size_t, ReflectionComponentPtr>;
  ComponentMap mComponents;
};

}//namespace SimpleReflection
