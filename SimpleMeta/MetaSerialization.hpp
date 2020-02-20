#pragma once

#include "Serializer.hpp"
#include "JsonSerializers.hpp"
#include "BinaryStream.hpp"

#include "SerializationPolicy.hpp"

struct MetaSerialization : public ReflectionComponent
{
public:
  virtual BoundType* GetBoundType() override;
  virtual bool SerializeBase(Serializer& serializer, BoundType& boundType, char* instanceData) = 0;
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(JsonSaver& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(JsonLoader& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(BinarySaver& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(BinaryLoader& serializer, BoundType& boundType, char* instanceData);
  virtual char* Allocate() const = 0;
};

template <typename T>
struct TypedMetaSerialization : public MetaSerialization
{
  template <typename SerializerType>
  bool SerializeTyped(SerializerType& serializer, char* instanceData)
  {
    T* typedData = (T*)instanceData;
    return SerializationPolicy<T>::Serialize<SerializerType>(serializer, *typedData);
  }

  virtual bool SerializeBase(Serializer& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(JsonSaver& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(JsonLoader& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(BinarySaver& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(BinaryLoader& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }

  virtual char* Allocate() const override
  {
    T* result = new T();
    return (char*)result;
  }
};
