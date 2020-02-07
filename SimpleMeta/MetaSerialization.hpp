#pragma once

#include "Serializer.hpp"

struct MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) = 0;
  virtual bool Serialize(Serializer& serializer, const Field& field, char* data)
  {
    return false;
  }
  virtual char* Allocate() const
  {
    return nullptr;
  }
};

template <typename T>
struct ObjectMetaSerialization : public MetaSerialization
{
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    return serializer.SerializeObject(boundType, data);
  }

  virtual bool Serialize(Serializer& serializer, const Field& field, char* data) override
  {
    return serializer.SerializeObject(field, data);
  }
  virtual char* Allocate() const override
  {
    return (char*)new T();
  }
};

template <typename T>
struct PrimitiveMetaSerialization : public MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    return serializer.SerializePrimitive(boundType, data);
  }

  virtual bool Serialize(Serializer& serializer, const Field& field, char* data) override
  {
    return serializer.SerializePrimitive(field, data);
  }
};

template <>
struct PrimitiveMetaSerialization<std::string> : public MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    std::string* str = reinterpret_cast<std::string*>(data);
    // return SerializationPolicy(serializer, boundType, *str);
    return serializer.SerializeString(boundType, *str);
  }

  virtual bool Serialize(Serializer& serializer, const Field& field, char* data) override
  {
    std::string* str = reinterpret_cast<std::string*>(data);
    // return SerializationPolicy(serializer, field, *str);
    return serializer.SerializeString(field, *str);
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
  virtual bool Serialize(Serializer& serializer, const Field& field, char* data) override
  {
    return serializer.SerializeArray(field, data, this);
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

template <typename T>
struct PolymorphicArrayMetaSerialization : public MetaSerialization, public ArrayAdapter
{
public:
  typedef std::vector<T> ArrayType;
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    return serializer.SerializeArray(boundType, data, this);
  }
  virtual bool Serialize(Serializer& serializer, const Field& field, char* data) override
  {
    return serializer.SerializePolymorphicArray(field, data, this);
  }
  virtual BoundType* GetPolymorphicItemType(char* data, size_t index)
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    return array[index]->VirtualGetBoundType();
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
    return (char*)array[index];
  }
  virtual void SetItem(char* data, size_t index, char* itemData)
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    array[index] = (T)itemData;
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
    // map.resize(count);
  }
  virtual char* GetItem(char* data, size_t index) override
  {
    MapType& map = *reinterpret_cast<MapType*>(data);

    return nullptr;
    // return (char*)&map[index];
  }
};
