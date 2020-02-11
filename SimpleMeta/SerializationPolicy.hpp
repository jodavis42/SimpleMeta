#pragma once

#include "StaticTypeId.hpp"
#include "MetaSerialization.hpp"

template <typename T>
struct SerializationPolicy
{
  static bool Serialize(Serializer& serializer, T& obj)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    return serializer.SerializeObject(*boundType, (char*)&obj);
  }
};

#define DeclarePrimitivePolicy(PrimitiveType)                           \
template <>                                                             \
struct SerializationPolicy<PrimitiveType>                               \
{                                                                       \
  static bool Serialize(Serializer& serializer, PrimitiveType& obj)     \
  {                                                                     \
    BoundType* boundType = StaticTypeId<PrimitiveType>::GetBoundType(); \
    return serializer.SerializePrimitive(*boundType, obj);              \
  }                                                                     \
};

DeclarePrimitivePolicy(bool)
DeclarePrimitivePolicy(char)
DeclarePrimitivePolicy(int)
DeclarePrimitivePolicy(float)
DeclarePrimitivePolicy(double)
DeclarePrimitivePolicy(std::string)

template <typename T>
bool SerializeArrayItems(Serializer& serializer, T* arrayData, size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    serializer.BeginArrayItem(i);
    SerializationPolicy<T>::Serialize(serializer, arrayData[i]);
    serializer.EndArrayItem();
  }
  return true;
}

template <typename T>
bool SerializeArrayItems(Serializer& serializer, std::vector<T>& arrayData, size_t count)
{
  return SerializeArrayItems(serializer, arrayData.data(), count);
}

template <typename T, size_t FixedSize>
bool SerializeArrayItems(Serializer& serializer, T (&arrayData)[FixedSize])
{
  return SerializeArrayItems(serializer, arrayData, FixedSize);
}


template <typename T, typename ... Extra>
struct SerializationPolicy<std::vector<T, Extra...>>
{
  typedef std::vector<T, Extra...> ArrayType;
  static bool Serialize(Serializer& serializer, ArrayType& data)
  {
    BoundType& subType = *StaticTypeId<T>::GetBoundType();
    if(serializer.mDirection == SerializerDirection::Saving)
    {
      size_t count = data.size();
      serializer.BeginArray(count);
      SerializeArrayItems(serializer, data, count);
      serializer.EndArray();
    }
    else
    {
      size_t count;
      serializer.BeginArray(count);
      data.resize(count);
      SerializeArrayItems(serializer, data, count);
      serializer.EndArray();
    }

    return true;
  }
};

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

template <typename T, typename ... Extra>
struct SerializationPolicy<std::vector<T*, Extra...>>
{
  typedef std::vector<T*, Extra...> ArrayType;
  static bool Serialize(Serializer& serializer, ArrayType& array)
  {
    BoundType& boundType = *StaticTypeId<T>::GetBoundType();
    BoundType& subType = *StaticTypeId<T>::GetBoundType();
    if(serializer.mDirection == SerializerDirection::Saving)
    {
      size_t count = array.size();
      serializer.BeginArray(count);
      for(size_t i = 0; i < count; ++i)
      {
        char* itemData = (char*)array[i];
        BoundType* subType = RuntimeTypeId<T*>::GetVirtualBoundType(itemData);
        serializer.BeginArrayItem(i);
        PolymorphicReflection<T*>::Save(serializer, *subType, itemData);
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
        serializer.BeginArrayItem(i);
        char* itemData = nullptr;
        PolymorphicReflection<T*>::Load(serializer, itemData);
        array[i] = (T*)itemData;
        serializer.EndArrayItem();
      }
      serializer.EndArray();
    }
    return true;
  }
};

template <typename Key, typename Value, typename ... Extra>
struct SerializationPolicy<std::unordered_map<Key, Value, Extra...>>
{
  typedef std::unordered_map<Key, Value, Extra...> MapType;
  static bool Serialize(Serializer& serializer, MapType& data)
  {
    BoundType* keyBoundType = StaticTypeId<Key>::GetBoundType();
    BoundType* valueBoundType = StaticTypeId<Value>::GetBoundType();
    if(serializer.mDirection == SerializerDirection::Saving)
    {
      size_t count = data.size();
      serializer.BeginArray(count);
      int i = 0;
      for(auto it = data.begin(); it != data.end(); ++it, ++i)
      {
        serializer.BeginArrayItem(i);
        serializer.BeginObject();

        serializer.BeginMember("Key");
        Key key = it->first;
        SerializationPolicy<Key>::Serialize(serializer, key);
        serializer.EndMember();

        serializer.BeginMember("Value");
        SerializationPolicy<Value>::Serialize(serializer, it->second);
        serializer.EndMember();

        serializer.EndObject();
        serializer.EndArrayItem();
      }
      serializer.EndArray();
    }
    else
    {
      size_t count = data.size();
      serializer.BeginArray(count);
      for(size_t i = 0; i < count; ++i)
      {
        Key key;
        Value value;
        serializer.BeginArrayItem(i);
        //serializer.BeginObject();

        serializer.BeginMember("Key");
        SerializationPolicy<Key>::Serialize(serializer, key);
        serializer.EndMember();

        serializer.BeginMember("Value");
        SerializationPolicy<Value>::Serialize(serializer, value);
        serializer.EndMember();
        data[key] = value;

        //serializer.EndObject();
        serializer.EndArrayItem();
      }
      serializer.EndArray();
    }

    return true;
  }
};
