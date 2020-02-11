#pragma once

#include "Serializer.hpp"
#include "SerializationPolicy.hpp"

#include <unordered_map>

struct MetaSerialization
{
public:
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) = 0;
  virtual char* Allocate() const = 0;
};

template <typename T>
struct TypedMetaSerialization : public MetaSerialization
{
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* data) override
  {
    T* typedData = (T*)data;
    return SerializationPolicy<T>::Serialize(serializer, *typedData);
  }

  virtual char* Allocate() const override
  {
    T* result = new T();
    return (char*)result;
  }
};
