#pragma once

#include "BoundType.hpp"

enum class SerializerDirection
{
  Saving,
  Loading
};

struct PolymorphicInfo
{
  std::string mName;
  TypeId mId;
};

struct Serializer
{
  SerializerDirection mDirection = SerializerDirection::Saving;

  virtual ~Serializer()
  {
  }

  virtual bool SerializePrimitive(const BoundType& boundType, char* data) { return false; }
  virtual bool SerializePrimitive(const BoundType& boundType, bool& data) { return SerializePrimitive(boundType, (char*)&data); }
  virtual bool SerializePrimitive(const BoundType& boundType, char& data) { return SerializePrimitive(boundType, &data); }
  virtual bool SerializePrimitive(const BoundType& boundType, int& data) { return SerializePrimitive(boundType, (char*)&data); }
  virtual bool SerializePrimitive(const BoundType& boundType, float& data) { return SerializePrimitive(boundType, (char*)&data); }
  virtual bool SerializePrimitive(const BoundType& boundType, double& data) { return SerializePrimitive(boundType, (char*)&data); }
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) { return false; }

  virtual bool BeginObject() { return true; }
  virtual bool BeginObject(PolymorphicInfo& info) { return true; }
  virtual bool BeginMember(const std::string& name) { return true; }
  virtual bool BeginArray(size_t& count) { return true; }
  virtual bool BeginArrayItem(size_t index) { return true; }
  virtual bool EndArray() { return true; }
  virtual bool EndObject() { return true; }
  virtual bool EndMember() { return true; }
  virtual bool EndArrayItem() { return true; }
  virtual bool SerializeObject(BoundType& boundType, char* data);
};

template <typename SerializerType>
bool SerializeProperties(SerializerType& serializer, BoundType& boundType, char* data)
{
  for(size_t i = 0; i < boundType.mFields.size(); ++i)
  {
    const Field& field = boundType.mFields[i];
    BoundType* fieldType = field.mType;
    char* fieldSrc = data + field.mOffset;

    MetaSerialization* fieldSerialization = fieldType->mMetaSerialization;
    if(fieldSerialization != nullptr && serializer.BeginMember(field.mName))
    {
      fieldSerialization->Serialize(serializer, *fieldType, fieldSrc);
      serializer.EndMember();
    }
  }

  return true;
}

template <typename SerializerType>
bool SerializeObject(SerializerType& serializer, BoundType& boundType, char* data)
{
  return SerializeProperties(serializer, boundType, data);
}

