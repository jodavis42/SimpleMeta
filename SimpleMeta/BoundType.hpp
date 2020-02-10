#pragma once

#include <string>
#include <vector>

struct BoundType;
struct MetaSerialization;

struct TypeId
{
  TypeId() : mId(0) {}
  TypeId(int id) : mId(id) {}
  size_t mId;
};

enum class BoundTypePrimitiveType
{
Primitive,
Class,
Array
};

struct Field
{
  std::string mName;
  size_t mOffset = 0;
  BoundType* mType = nullptr;
};

struct BoundType
{
  BoundTypePrimitiveType mPrimitiveType = BoundTypePrimitiveType::Primitive;
  std::string mName;
  size_t mSizeInBytes = 0;
  size_t mStride = 0;
  std::vector<Field> mFields;
  MetaSerialization* mMetaSerialization = nullptr;
  BoundType* mBaseType = nullptr;
  TypeId mId;
};
