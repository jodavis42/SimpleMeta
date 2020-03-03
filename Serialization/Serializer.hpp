#pragma once

#include "BoundType.hpp"
#include "Attributes.hpp"

using SimpleReflection::TypeId;
using SimpleReflection::BoundType;
using SimpleReflection::GetterSetter;
using SimpleReflection::ReflectionLibrary;

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
  virtual bool BeginMembers(size_t& count) { return true; }
  virtual bool BeginMember(const std::string& name) { return true; }
  virtual bool BeginMember(size_t index, std::string& name) { return true; }
  virtual bool BeginArray(size_t& count) { return true; }
  virtual bool BeginArrayItem(size_t index) { return true; }
  virtual bool EndArray() { return true; }
  virtual bool EndObject() { return true; }
  virtual bool EndMembers() { return true; }
  virtual bool EndMember() { return true; }
  virtual bool EndArrayItem() { return true; }
  virtual bool SerializeObject(BoundType& boundType, char* data);
};
