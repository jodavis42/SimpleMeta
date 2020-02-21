#pragma once

struct TypeId
{
  TypeId() : mId(0) {}
  TypeId(int id) : mId(id) {}
  size_t mId;
};
