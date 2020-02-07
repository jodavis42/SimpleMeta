#pragma once

#include "BoundType.hpp"

enum class SerializerDirection
{
  Saving,
  Loading
};

struct ArrayAdapter
{
  virtual BoundType* GetSubType(BoundType& boundType)
  {
    return nullptr;
  }
  virtual void Initialize(char* data)
  {
  }
  virtual size_t GetCount(char* data)
  {
    return 0;
  }
  virtual void SetCount(char* data, size_t count)
  {
  }
  virtual char* GetItem(char* data, size_t index)
  {
    return nullptr;
  }
};

struct Serializer
{
  SerializerDirection mDirection = SerializerDirection::Saving;

  virtual ~Serializer()
  {
  }

  virtual bool SerializePrimitive(BoundType& boundType, char* data)
  {
    return false;
  }

  virtual bool SerializeObject(BoundType& boundType, char* data)
  {
    return false;
  }

  virtual bool SerializeString(BoundType& boundType, std::string& data)
  {
    return false;
  }

  virtual bool SerializeArrayCount(BoundType& boundType, size_t& count)
  {
    return false;
  }
  virtual bool SerializeArray(BoundType& boundType, char* data, size_t count)
  {
    return false;
  }

  virtual bool SerializeArray(BoundType& boundType, char* data, ArrayAdapter* adapter)
  {
    return false;
  }
};
