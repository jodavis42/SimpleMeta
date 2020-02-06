#pragma once

#include "Serializer.hpp"

struct MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) = 0;
};

template <typename T>
struct PrimitiveMetaSerialization : public MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    return serializer.SerializePrimitive(boundType, data);
  }
};

template <>
struct PrimitiveMetaSerialization<std::string> : public MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    std::string* str = reinterpret_cast<std::string*>(data);
    return serializer.SerializeString(boundType, *str);
  }
};

template <typename T>
struct ArrayMetaSerialization : public MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    std::vector<T>& array = *reinterpret_cast<std::vector<T>*>(data);

    size_t count = array.size();
    serializer.SerializeArrayCount(boundType, count);
    if(serializer.mDirection == SerializerDirection::Saving)
      return serializer.SerializeArray(boundType, (char*)array.data(), count);
    else
    {
      new(&array) std::vector<T>();
      array.resize(count);
      return serializer.SerializeArray(boundType, (char*)array.data(), count);
    }
  }
};