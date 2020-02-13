#pragma once

#include "StaticTypeId.hpp"
#include "Serializer.hpp"
#include <sstream>

struct BinarySaver : public Serializer
{
public:
  using Serializer::SerializePrimitive;

  BinarySaver(std::ostream& stream);

  template <typename T>
  void Serialize(const T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeProperties(*this, *boundType, (char*)(&data));
  }

  
  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

  using Serializer::BeginObject;
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
    SerializeProperties(*this, *boundType, (char*)(&data));
  }

  using Serializer::SerializePrimitive;
  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

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
