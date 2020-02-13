#pragma once

#include "BoundType.hpp"
#include <functional>

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

struct MemberIterator {
    std::string mMemberName;
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

  virtual bool BeginStringTable(int &count) { return true; }
  virtual bool EndStringTable() { return true; }
  virtual bool BeginStringTableEntry(std::string& name) { return true; }
  virtual bool EndStringTableEntry() { return true; }

  virtual bool BeginObject() { return true; }
  virtual bool BeginObject(PolymorphicInfo& info) { return true; }
  virtual bool ForAllMembers(int size, std::function<void(Serializer&, const std::string&)> callback) { return false; }
  virtual bool GetNextMember(MemberIterator& members) { return false; }
  virtual bool BeginMember(const std::string& name) { return true; }
  virtual bool BeginArray(size_t& count) { return true; }
  virtual bool BeginArrayItem(size_t index) { return true; }
  virtual bool EndArray() { return true; }
  virtual bool EndObject() { return true; }
  virtual bool EndMember() { return true; }
  virtual bool EndArrayItem() { return true; }
  virtual bool SerializeProperties(BoundType& boundType, char* data);
  virtual bool SerializeObject(BoundType& boundType, char* data);


};

