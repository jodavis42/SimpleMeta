#pragma once

#include "StaticTypeId.hpp"
#include "Serializer.hpp"
#include <sstream>

struct BinarySaver : public Serializer
{
  BinarySaver(std::ostream& stream);

  template <typename T>
  void Serialize(const T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeObject(*boundType, (char*)(&data));
  }

  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

  virtual bool BeginStringTable(int& count) override;
  virtual bool EndStringTable() override;
  virtual bool BeginStringTableEntry(std::string& name) override;
  virtual bool EndStringTableEntry() override;

  virtual bool BeginObject(PolymorphicInfo& info) override;
  virtual bool BeginArray(size_t& count) override;

  bool Write(char* data, size_t sizeInBytes);
  bool Write(size_t data, size_t sizeInBytes);
  bool Write(size_t data);

  std::ostream mStream;
};

struct BinaryLoader : public Serializer
{
  BinaryLoader(std::istream& stream);

  template <typename T>
  void Serialize(T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeObject(*boundType, (char*)(&data));
  }

  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

  virtual bool BeginStringTable(int& count) override;
  virtual bool EndStringTable() override;
  virtual bool BeginStringTableEntry(std::string& name) override;
  virtual bool EndStringTableEntry() override;
  virtual bool ForAllMembers(int size, std::function<void(Serializer&, const std::string&)> callback) override;

  virtual bool BeginObject() override;
  virtual bool BeginObject(PolymorphicInfo& info) override;
  virtual bool BeginArray(size_t& count) override;
  virtual bool EndArray() override;
  virtual bool EndObject() override;
  virtual bool SerializeObject(BoundType& boundType, char* data) override;

  bool Read(char* data, size_t sizeInBytes);
  bool Read(size_t& data, size_t sizeInBytes);
  bool Read(size_t& data);

  std::istream mStream;
};
