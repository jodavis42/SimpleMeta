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
  static bool Save(Serializer& serializer, BoundType& boundType, char* objData)
  {
    boundType.mMetaSerialization->Serialize(serializer, boundType, objData);
    return true;
  }

  static bool Load(Serializer& serializer, BoundType& boundType, char*& objData)
  {
    objData = boundType.mMetaSerialization->Allocate();
    boundType.mMetaSerialization->Serialize(serializer, boundType, objData);
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

    if (serializer.mDirection == SerializerDirection::Saving)
    {
        int size = (int)array.size();
        serializer.BeginStringTable(size);
        for (size_t i = 0; i < size; ++i)
        {
            char* itemData = (char*)array[i];
            BoundType* subType = RuntimeTypeId<T*>::GetVirtualBoundType(itemData);
            serializer.BeginStringTableEntry(subType->mName);
            PolymorphicReflection<T*>::Save(serializer, *subType, itemData);
            serializer.EndStringTableEntry();
        }
        serializer.EndStringTable();
    }
    else
    {
        int size = 0;
        array.clear();
        serializer.BeginStringTable(size);
        serializer.ForAllMembers(size, [&array](Serializer& serializer, const std::string& memberName) {
            std::string name(memberName);
            serializer.BeginStringTableEntry(name);
            char* itemData = nullptr;
            BoundType* boundType = MetaLibrary::FindBoundType(name);
            if (boundType != nullptr) {
                if (PolymorphicReflection<T*>::Load(serializer, *boundType, itemData)) {
                    array.push_back((T*)itemData);
                }
            }
            serializer.EndStringTableEntry();
        });
        serializer.EndStringTable();
    }

    return true;
  }
};

template <typename Value, typename ... Extra>
struct SerializationPolicy<std::unordered_map<std::string, Value, Extra...>>
{
    typedef std::unordered_map<std::string, Value, Extra...> MapType;
    static bool Serialize(Serializer& serializer, MapType& data)
    {
        if (serializer.mDirection == SerializerDirection::Saving)
        {
            int size = (int)data.size();
            serializer.BeginStringTable(size);
            int i = 0;
            for (auto it = data.begin(); it != data.end(); ++it, ++i)
            {
                std::string name(it->first);
                serializer.BeginStringTableEntry(name);
                SerializationPolicy<Value>::Serialize(serializer, it->second);
                serializer.EndStringTableEntry();
            }
            serializer.EndStringTable();
        }
        else
        {
            int size = 0;
            serializer.BeginStringTable(size);
            serializer.ForAllMembers(size, [&data](Serializer& serializer, const std::string& memberName) {
                std::string name(memberName);
                serializer.BeginStringTableEntry(name);
                Value v;
                SerializationPolicy<Value>::Serialize(serializer, v);
                data[name] = v;
                serializer.EndStringTableEntry();
            });
            serializer.EndStringTable();
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
