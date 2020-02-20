#pragma once

#include "Serializer.hpp"
#include "JsonSerializers.hpp"
#include "BinaryStream.hpp"

#include "SerializationPolicy.hpp"

struct MetaSerialization : public ReflectionComponent
{
public:
  virtual bool SerializeBase(Serializer& serializer, BoundType& boundType, char* data) = 0;
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data);
  virtual bool Serialize(JsonSaver& serializer, BoundType& boundType, char* data);
  virtual bool Serialize(JsonLoader& serializer, BoundType& boundType, char* data);
  virtual bool Serialize(BinarySaver& serializer, BoundType& boundType, char* data);
  virtual bool Serialize(BinaryLoader& serializer, BoundType& boundType, char* data);
  virtual BoundType* GetBoundType() override;
  virtual char* Allocate() const = 0;
};

template <typename T>
struct TypedMetaSerialization : public MetaSerialization
{
  template <typename SerializerType>
  bool SerializeTyped(SerializerType& serializer, char* data)
  {
    T* typedData = (T*)data;
    return SerializationPolicy<T>::Serialize<SerializerType>(serializer, *typedData);
  }

  virtual bool SerializeBase(Serializer& serializer, BoundType& boundType, char* data) { return SerializeTyped(serializer, data); }
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) { return SerializeTyped(serializer, data); }
  virtual bool Serialize(JsonSaver& serializer, BoundType& boundType, char* data) { return SerializeTyped(serializer, data); }
  virtual bool Serialize(JsonLoader& serializer, BoundType& boundType, char* data) { return SerializeTyped(serializer, data); }
  virtual bool Serialize(BinarySaver& serializer, BoundType& boundType, char* data) { return SerializeTyped(serializer, data); }
  virtual bool Serialize(BinaryLoader& serializer, BoundType& boundType, char* data) { return SerializeTyped(serializer, data); }

  virtual char* Allocate() const override
  {
    T* result = new T();
    return (char*)result;
  }
};
