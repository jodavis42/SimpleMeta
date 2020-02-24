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
  virtual bool SerializePropertyBase(Serializer& serializer, GetterSetter& getSet, char* instanceData) = 0;
  
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(JsonSaver& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(JsonLoader& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(BinarySaver& serializer, BoundType& boundType, char* instanceData);
  virtual bool Serialize(BinaryLoader& serializer, BoundType& boundType, char* instanceData);

  virtual bool SerializeProperty(Serializer& serializer, GetterSetter& getSet, char* instanceData);
  virtual bool SerializeProperty(JsonSaver& serializer, GetterSetter& getSet, char* instanceData);
  virtual bool SerializeProperty(JsonLoader& serializer, GetterSetter& getSet, char* instanceData);
  virtual bool SerializeProperty(BinarySaver& serializer, GetterSetter& getSet, char* instanceData);
  virtual bool SerializeProperty(BinaryLoader& serializer, GetterSetter& getSet, char* instanceData);

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

  template <typename SerializerType>
  bool SerializePropertyTyped(SerializerType& serializer, GetterSetter& getSet, char* instanceData)
  {
    bool result = false;
    if(serializer.mDirection == SerializerDirection::Loading)
    {
      T data;
      result = SerializationPolicy<T>::Serialize<Serializer>(serializer, data);

      Any any;
      any.Set(data);
      getSet.Set(instanceData, any);
    }
    else
    {
      Any any = getSet.Get(instanceData);
      T data = any.Get<T>();
      result = SerializationPolicy<T>::Serialize<Serializer>(serializer, data);
    }
    return result;
  }

  virtual bool SerializeBase(Serializer& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(Serializer& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(JsonSaver& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(JsonLoader& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(BinarySaver& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }
  virtual bool Serialize(BinaryLoader& serializer, BoundType& boundType, char* instanceData) override { return SerializeTyped(serializer, instanceData); }

  virtual bool SerializePropertyBase(Serializer& serializer, GetterSetter& getSet, char* instanceData) override { return SerializePropertyTyped(serializer, getSet, instanceData); }
  virtual bool SerializeProperty(Serializer& serializer, GetterSetter& getSet, char* instanceData) override { return SerializePropertyTyped(serializer, getSet, instanceData); }
  virtual bool SerializeProperty(JsonSaver& serializer, GetterSetter& getSet, char* instanceData) override { return SerializePropertyTyped(serializer, getSet, instanceData); }
  virtual bool SerializeProperty(JsonLoader& serializer, GetterSetter& getSet, char* instanceData) override { return SerializePropertyTyped(serializer, getSet, instanceData); }
  virtual bool SerializeProperty(BinarySaver& serializer, GetterSetter& getSet, char* instanceData) override { return SerializePropertyTyped(serializer, getSet, instanceData); }
  virtual bool SerializeProperty(BinaryLoader& serializer, GetterSetter& getSet, char* instanceData) override { return SerializePropertyTyped(serializer, getSet, instanceData); }

  virtual char* Allocate() const override
  {
    T* result = new T();
    return (char*)result;
  }
};

template <typename ClassType>
void DefaultTypeSetup(ReflectionLibrary& library, BoundType& ownerType)
{
  if(ownerType.QueryComponentType<MetaSerialization>() == nullptr)
  {
    ownerType.AddComponentType<TypedMetaSerialization<ClassType>>();
  }
}

template <typename ClassType, typename FieldType>
void DefaultFieldSetup(ReflectionLibrary& library, BoundType& ownerType, BoundType& fieldType)
{
  if(fieldType.QueryComponentType<MetaSerialization>() == nullptr)
  {
    fieldType.AddComponentType<TypedMetaSerialization<FieldType>>();
  }
}

template <typename ClassType, typename FieldType>
void DefaultGetterSetterSetup(ReflectionLibrary& library, BoundType& ownerType, BoundType& getterSetterType)
{
  if(getterSetterType.QueryComponentType<MetaSerialization>() == nullptr)
  {
    getterSetterType.AddComponentType<TypedMetaSerialization<FieldType>>();
  }
}
