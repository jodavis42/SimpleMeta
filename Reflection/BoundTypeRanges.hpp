#pragma once

#include <vector>

namespace SimpleReflection
{

struct BoundType;
struct Field;
struct GetterSetter;

/// Iterator that will visit all fields in a bound type. This starts with the base class and walks down.
struct FieldRange
{
public:
  FieldRange(BoundType* boundType);

  Field* Front();
  void PopFront();
  bool Empty() const;

private:
  void SkipEmpty();

  size_t mMemberIndex;
  std::vector<BoundType*> mStack;
  BoundType* mBoundType;
};

/// Iterator that will visit all getter/setters in a bound type. This starts with the base class and walks down.
struct GetterSetterRange
{
public:
  GetterSetterRange(BoundType* boundType);

  GetterSetter* Front();
  void PopFront();
  bool Empty() const;

private:
  void SkipEmpty();

  size_t mMemberIndex;
  std::vector<BoundType*> mStack;
  BoundType* mBoundType;
};

}//namespace SimpleReflection
