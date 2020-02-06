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
struct ArrayMetaSerialization : public MetaSerialization, public ArrayAdapter
{
public:
  typedef std::vector<T> ArrayType;
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    return serializer.SerializeArray(boundType, data, this);
  }

  virtual BoundType* GetSubType(BoundType& boundType)
  {
    return StaticTypeId<T>::GetBoundType();
  }
  virtual void Initialize(char* data) override
  {
    new(data) ArrayType();
  }
  virtual size_t GetCount(char* data) override
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    return array.size();
  }
  virtual void SetCount(char* data, size_t count) override
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    array.resize(count);
  }
  virtual char* GetItem(char* data, size_t index) override
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    return (char*)&array[index];
  }
};

template <typename KeyType, typename ValueType>
struct MapMetaSerialization : public MetaSerialization, public ArrayAdapter
{
public:
  typedef std::unordered_map<KeyType, ValueType> MapType;
  typedef std::pair<KeyType, ValueType> PairType;

  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    return serializer.SerializeArray(boundType, data, this);
  }

  virtual BoundType* GetSubType(BoundType& boundType)
  {
    return StaticTypeId<PairType>::GetBoundType();
  }
  virtual void Initialize(char* data) override
  {
    new(data) MapType();
  }
  virtual size_t GetCount(char* data) override
  {
    MapType& map = *reinterpret_cast<MapType*>(data);
    return map.size();
  }
  virtual void SetCount(char* data, size_t count) override
  {
    MapType& map = *reinterpret_cast<MapType*>(data);
    //map.resize(count);
  }
  virtual char* GetItem(char* data, size_t index) override
  {
    MapType& map = *reinterpret_cast<MapType*>(data);
    
    return nullptr;
    //return (char*)&map[index];
  }
};
