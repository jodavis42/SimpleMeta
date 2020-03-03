#pragma once

namespace SimpleReflection
{

struct TypeId
{
  TypeId() : mId(sInvalidId) {}
  TypeId(size_t id) : mId(id) {}
  size_t mId;

  static constexpr size_t sInvalidId = static_cast<size_t>(-1);
};

}//namespace SimpleReflection
