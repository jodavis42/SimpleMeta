#pragma once

#include "StaticTypeId.hpp"
#include <unordered_map>

template <typename T>
struct SerializationPolicy
{
  template <typename SerializerType>
  static bool Serialize(SerializerType& serializer, T& obj)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    return serializer.SerializeObject(*boundType, (char*)&obj);
  }
};

#define DeclarePrimitivePolicy(PrimitiveType)                           \
template <>                                                             \
struct SerializationPolicy<PrimitiveType>                               \
{                                                                       \
  template <typename SerializerType>                                    \
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
    SerializationPolicy<T>::Serialize<SerializerType>(serializer, arrayData[i]);
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

template <typename T, typename ... Extra>
struct SerializationPolicy<std::vector<T, Extra...>>
{
  typedef std::vector<T, Extra...> ArrayType;

 
  template <typename SerializerType>
  static bool Serialize(SerializerType& serializer, ArrayType& data)
  {
	  BoundType& subType = *StaticTypeId<T>::GetBoundType();
	  if (serializer.mDirection == SerializerDirection::Saving)
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

  template <>
  static bool Serialize<BinarySaver>(BinarySaver& serializer, ArrayType& data)
  {
	  BoundType& subType = *StaticTypeId<T>::GetBoundType();
	  size_t count = data.size();
	  serializer.BeginArray(count);
	  SerializeArrayItems(serializer, data, count);
	  serializer.EndArray();
	  return true;
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

template <typename T, typename ... Extra>
struct SerializationPolicy<std::vector<T*, Extra...>>
{
  typedef std::vector<T*, Extra...> ArrayType;

  template <typename SerializerType>
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

template <typename Key, typename Value, typename ... Extra>
struct SerializationPolicy<std::unordered_map<Key, Value, Extra...>>
{
  typedef std::unordered_map<Key, Value, Extra...> MapType;

  template <typename SerializerType>
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
        SerializationPolicy<Key>::Serialize<SerializerType>(serializer, key);
        serializer.EndMember();

        serializer.BeginMember("Value");
        SerializationPolicy<Value>::Serialize<SerializerType>(serializer, it->second);
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
        SerializationPolicy<Key>::Serialize<SerializerType>(serializer, key);
        serializer.EndMember();

        serializer.BeginMember("Value");
        SerializationPolicy<Value>::Serialize<SerializerType>(serializer, value);
        serializer.EndMember();
        data[key] = value;

        //serializer.EndObject();
        serializer.EndArrayItem();
      }
      serializer.EndArray();
    }

    return true;
  }

  template <>
  static bool Serialize<JsonSaver>(JsonSaver& serializer, MapType& data)
  {
	  BoundType* keyBoundType = StaticTypeId<Key>::GetBoundType();
	  BoundType* valueBoundType = StaticTypeId<Value>::GetBoundType();

	  size_t count = data.size();
	  serializer.BeginArray(count);
	  int i = 0;
	  for (auto it = data.begin(); it != data.end(); ++it, ++i)
	  {
		  serializer.BeginArrayItem(i);
		  serializer.BeginObject();

		  serializer.BeginMember("Key");
		  Key key = it->first;
		  SerializationPolicy<Key>::Serialize<JsonSaver>(serializer, key);
		  serializer.EndMember();

		  serializer.BeginMember("Value");
		  SerializationPolicy<Value>::Serialize<JsonSaver>(serializer, it->second);
		  serializer.EndMember();

		  serializer.EndObject();
		  serializer.EndArrayItem();
	  }
	  serializer.EndArray();

	  return true;
  }
};

template <typename Value, typename ... Extra>
struct SerializationPolicy<std::unordered_map<std::string, Value, Extra...>>
{
  typedef std::string Key;
  typedef std::unordered_map<std::string, Value, Extra...> MapType;

  template <typename SerializerType>
  static bool Serialize(Serializer& serializer, MapType& data)
  {
    BoundType* keyBoundType = StaticTypeId<Key>::GetBoundType();
    BoundType* valueBoundType = StaticTypeId<Value>::GetBoundType();
    if(serializer.mDirection == SerializerDirection::Saving)
    {
      size_t count = data.size();
      serializer.BeginMembers(count);
      size_t i = 0;
      for(auto it = data.begin(); it != data.end(); ++it, ++i)
      {
        std::string key = it->first;
        serializer.BeginMember(i, key);
        SerializationPolicy<Value>::Serialize(serializer, it->second);
        serializer.EndMember();
      }
      serializer.EndMembers();
    }
    else
    {
      size_t count;
      serializer.BeginMembers(count);
      for(size_t i = 0; i < count; ++i)
      {
        std::string key;
        serializer.BeginMember(i, key);

        Value value;
        SerializationPolicy<Value>::Serialize(serializer, value);

        data[key] = value;

        serializer.EndMember();

      }
      serializer.EndMembers();
    }

    return true;
  }
};
