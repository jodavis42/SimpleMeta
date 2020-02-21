#pragma once

struct TypeId
{
  TypeId() : mId(0) {}
  TypeId(size_t id) : mId(id) {}
  size_t mId;
};
