#pragma once

struct ReflectionLibrary;
struct BoundType;

/// A component that can be attached to any reflection composition object.
/// This allows generically attaching data to reflection objects.
struct ReflectionComponent
{
  virtual ~ReflectionComponent() {}

  /// All reflection components must implement this so that the composition can know what type they are.
  virtual BoundType* GetBoundType() abstract;
};
