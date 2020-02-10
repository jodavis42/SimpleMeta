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

  virtual bool BeginObject() override;
  virtual bool BeginObject(PolymorphicInfo& info) override;
  virtual bool BeginArray(size_t& count) override;
  virtual bool BeginArrayItem(const BoundType& boundType, size_t index, char* data) override;
  virtual bool EndArray() override;
  virtual bool EndObject() override;
  virtual bool EndArrayItem() override;
  virtual bool SerializeObject(BoundType& boundType, char* data) override;

  virtual bool SerializeArrayCount(BoundType& boundType, size_t& count) override;
  virtual bool SerializeArray(BoundType& boundType, char* data, size_t count) override;
  virtual bool SerializeArray(BoundType& boundType, char* data, ArrayAdapter* adapter);

  bool Read(char* data, size_t sizeInBytes);
  bool Read(size_t& data, size_t sizeInBytes);
  bool Read(size_t& data);

  std::istream mStream;
};
