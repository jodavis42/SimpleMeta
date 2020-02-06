#pragma once

enum class SerializerDirection
{
Saving,
Loading
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
};
