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
  virtual bool BeginArrayItem(const BoundType& boundType, size_t index, char* data) { return true; }
  virtual bool EndArray() { return true; }
  virtual bool EndObject() { return true; }
  virtual bool EndMember() { return true; }
  virtual bool EndArrayItem() { return true; }
  virtual bool SerializeProperties(BoundType& boundType, char* data);
  virtual bool SerializeObject(BoundType& boundType, char* data);
};

