#pragma once

#include "StaticTypeId.hpp"
#include <unordered_map>

template <typename SerializerType, typename T>
struct SerializationPolicy
{
  static bool Serialize(SerializerType& serializer, T& obj)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    return serializer.SerializeObject(*boundType, (char*)&obj);
  }
};

#define DeclarePrimitivePolicy(PrimitiveType)                           \
template <typename SerializerType>                                      \
struct SerializationPolicy<SerializerType, PrimitiveType>               \
{                                                                       \
  static bool Serialize(SerializerType& serializer, PrimitiveType& obj) \
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

template <typename SerializerType, typename T>
bool SerializeArrayItems(SerializerType& serializer, T* arrayData, size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    serializer.BeginArrayItem(i);
    SerializationPolicy<SerializerType, T>::Serialize(serializer, arrayData[i]);
    serializer.EndArrayItem();
  }
  return true;
}

template <typename SerializerType, typename T>
bool SerializeArrayItems(SerializerType& serializer, std::vector<T>& arrayData, size_t count)
{
  return SerializeArrayItems(serializer, arrayData.data(), count);
}

template <typename SerializerType, typename T, size_t FixedSize>
bool SerializeArrayItems(SerializerType& serializer, T (&arrayData)[FixedSize])
{
  return SerializeArrayItems(serializer, arrayData, FixedSize);
}

template <typename SerializerType, typename ArrayType, typename T>
struct ArraySerializationPolicy
{
  static bool Serialize(SerializerType& serializer, ArrayType& data)
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

template <typename ArrayType, typename T>
struct ArraySerializationPolicy<BinarySaver, ArrayType, T>
{
  static bool Serialize(BinarySaver& serializer, ArrayType& data)
  {
    BoundType& subType = *StaticTypeId<T>::GetBoundType();
    size_t count = data.size();
    serializer.BeginArray(count);
    SerializeArrayItems(serializer, data, count);
    serializer.EndArray();
    return true;
  }
};

template <typename SerializerType, typename T, typename ... Extra>
struct SerializationPolicy<SerializerType, std::vector<T, Extra...>>
{
  typedef std::vector<T, Extra...> ArrayType;
  static bool Serialize(SerializerType& serializer, ArrayType& data)
  {
    return ArraySerializationPolicy<SerializerType, ArrayType, T>::Serialize(serializer, data);
  }
};


template <typename SerializerType, typename T>
struct PolymorphicReflection
{
  static BoundType* BeginPolymorphicObject(SerializerType& serializer)
  {
    PolymorphicInfo info;
    serializer.BeginObject(info);
    if(!info.mName.empty())
      return MetaLibrary::FindBoundType(info.mName);
    else
      return MetaLibrary::FindBoundType(info.mId);
  }

  static bool Save(SerializerType& serializer, BoundType& boundType, char* objData)
  {
    PolymorphicInfo info{boundType.mName, boundType.mId};
    serializer.BeginObject(info);
    boundType.mMetaSerialization->Serialize(serializer, boundType, objData);
    serializer.EndObject();
    return true;
  }

  static bool Load(SerializerType& serializer, char*& objData)
  {
    BoundType& boundType = *BeginPolymorphicObject(serializer);
    objData = boundType.mMetaSerialization->Allocate();
    boundType.mMetaSerialization->Serialize(serializer, boundType, objData);
    serializer.EndObject();
    return true;
  }
};

template <typename SerializerType, typename T, typename ... Extra>
struct SerializationPolicy<SerializerType, std::vector<T*, Extra...>>
{
  typedef std::vector<T*, Extra...> ArrayType;
  static bool Serialize(SerializerType& serializer, ArrayType& array)
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
        PolymorphicReflection<SerializerType, T*>::Save(serializer, *subType, itemData);
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
        PolymorphicReflection<SerializerType, T*>::Load(serializer, itemData);
        array[i] = (T*)itemData;
        serializer.EndArrayItem();
      }
      serializer.EndArray();
    }
    return true;
  }
};

template <typename SerializerType, typename Key, typename Value, typename ... Extra>
struct SerializationPolicy<SerializerType, std::unordered_map<Key, Value, Extra...>>
{
  typedef std::unordered_map<Key, Value, Extra...> MapType;
  static bool Serialize(SerializerType& serializer, MapType& data)
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
        SerializationPolicy<SerializerType, Key>::Serialize(serializer, key);
        serializer.EndMember();

        serializer.BeginMember("Value");
        SerializationPolicy<SerializerType, Value>::Serialize(serializer, it->second);
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
        SerializationPolicy<SerializerType, Key>::Serialize(serializer, key);
        serializer.EndMember();

        serializer.BeginMember("Value");
        SerializationPolicy<SerializerType, Value>::Serialize(serializer, value);
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

template <typename Key, typename Value, typename ... Extra>
struct SerializationPolicy<JsonSaver, std::unordered_map<Key, Value, Extra...>>
{
  typedef std::unordered_map<Key, Value, Extra...> MapType;
  static bool Serialize(JsonSaver& serializer, MapType& data)
  {
    BoundType* keyBoundType = StaticTypeId<Key>::GetBoundType();
    BoundType* valueBoundType = StaticTypeId<Value>::GetBoundType();
   
    size_t count = data.size();
    serializer.BeginArray(count);
    int i = 0;
    for(auto it = data.begin(); it != data.end(); ++it, ++i)
    {
      serializer.BeginArrayItem(i);
      serializer.BeginObject();

      serializer.BeginMember("Key");
      Key key = it->first;
      SerializationPolicy<JsonSaver, Key>::Serialize(serializer, key);
      serializer.EndMember();

      serializer.BeginMember("Value");
      SerializationPolicy<JsonSaver, Value>::Serialize(serializer, it->second);
      serializer.EndMember();

      serializer.EndObject();
      serializer.EndArrayItem();
    }
    serializer.EndArray();

    return true;
  }
};
