#pragma once

#include "Serializer.hpp"

#include <unordered_map>

struct MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) = 0;
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
    return serializer.SerializePrimitive(boundType, *(T*)data);
  }
};

template <>
struct PrimitiveMetaSerialization<std::string> : public MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    std::string* str = reinterpret_cast<std::string*>(data);
    return serializer.SerializePrimitive(boundType, *str);
  }
};

inline bool SerializeArrayItems(Serializer& serializer, size_t count, BoundType& subType, char* arrayData)
{
  for(size_t i = 0; i < count; ++i)
  {
    serializer.BeginArrayItem(subType, i, arrayData);
    subType.mMetaSerialization->Serialize(serializer, subType, arrayData + subType.mSizeInBytes * i);
    serializer.EndArrayItem();
  }
  return true;
}

template <typename T>
struct PolymorphicReflection
{
  static BoundType* BeginPolymorphicObject(Serializer& serializer)
  {
    PolymorphicInfo info;
    serializer.BeginObject(info);
    if(!info.mName.empty())
      return MetaLibrary::FindBoundType(info.mName);
    else
      return MetaLibrary::FindBoundType(info.mId);
  }

  static bool Save(Serializer& serializer, BoundType& boundType, char* objData)
  {
    PolymorphicInfo info{boundType.mName, boundType.mId};
    serializer.BeginObject(info);
    boundType.mMetaSerialization->Serialize(serializer, boundType, objData);
    serializer.EndObject();
    return true;
  }

  static bool Load(Serializer& serializer, char*& objData)
  {
    BoundType& boundType = *BeginPolymorphicObject(serializer);
    objData = boundType.mMetaSerialization->Allocate();
    boundType.mMetaSerialization->Serialize(serializer, boundType, objData);
    serializer.EndObject();
    return true;
  }
};

template <typename T>
struct ArrayMetaSerialization : public MetaSerialization
{
public:
  typedef std::vector<T> ArrayType;

  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    BoundType& subType = *StaticTypeId<T>::GetBoundType();
    if(serializer.mDirection == SerializerDirection::Saving)
    {
      size_t count = array.size();
      serializer.BeginArray(count);
      SerializeArrayItems(serializer, count, subType, (char*)array.data());
      serializer.EndArray();
    }
    else
    {
      size_t count;
      serializer.BeginArray(count);
      array.resize(count);
      SerializeArrayItems(serializer, count, subType, (char*)array.data());
      serializer.EndArray();
    }
    
    return true;
  }
};

template <typename T>
struct PolymorphicArrayMetaSerialization : public MetaSerialization
{
public:
  typedef std::vector<T> ArrayType;
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    BoundType& subType = *StaticTypeId<T>::GetBoundType();
    if(serializer.mDirection == SerializerDirection::Saving)
    {
      size_t count = array.size();
      serializer.BeginArray(count);
      for(size_t i = 0; i < count; ++i)
      {
        char* itemData = GetItem(data, i);
        BoundType* subType = RuntimeTypeId<T>::GetVirtualBoundType(itemData);
        serializer.BeginArrayItem(*subType, i, itemData);
        PolymorphicReflection<T>::Save(serializer, *subType, itemData);
        serializer.EndArrayItem();
      }
      serializer.EndArray();
      
    }
    else
    {
      size_t count;
      serializer.BeginArray(count);
      array.resize(count);
      for(size_t i = 0; i < count; ++i)
      {
        serializer.BeginArrayItem(boundType, i, GetItem(data, i));
        char* itemData = nullptr;
        PolymorphicReflection<typename T>::Load(serializer, itemData);
        array[i] = (T)itemData;
        serializer.EndArrayItem();
      }
      serializer.EndArray();
    }
    return true;
  }
  char* GetItem(char* data, size_t index)
  {
    ArrayType& array = *reinterpret_cast<ArrayType*>(data);
    return (char*)array[index];
  }
};

template <typename KeyType, typename ValueType>
struct MapMetaSerialization : public MetaSerialization
{
public:
  typedef std::unordered_map<KeyType, ValueType> MapType;
  typedef std::pair<KeyType, ValueType> PairType;

  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    return false;
  }

  virtual BoundType* GetSubType(BoundType& boundType)
  {
    return StaticTypeId<PairType>::GetBoundType();
  }
};
