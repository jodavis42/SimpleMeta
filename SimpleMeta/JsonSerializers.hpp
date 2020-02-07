#pragma once

#include "Serializer.hpp"

class JsonInternalData;

class JsonSaver : public Serializer
{
public:
  JsonSaver();
  ~JsonSaver();
  std::string ToString();

  template <typename T>
  void Serialize(const T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeObject(*boundType, (char*)(&data));
  }


  virtual bool SerializePrimitive(const Field& field, char* data) override;
  virtual bool SerializeObject(const Field& field, char* data) override;
  virtual bool SerializeString(const Field& field, std::string& data) override;
  virtual bool SerializeArray(const Field& field, char* data, ArrayAdapter* adapter) override;
  virtual bool SerializePolymorphicArray(const Field& field, char* data, ArrayAdapter* adapter) override;

  virtual bool SerializeObject(BoundType& boundType, char* data) override;

  JsonInternalData* mData;
};

class JsonLoader : public Serializer
{
public:
  JsonLoader();
  ~JsonLoader();

  template <typename T>
  void Serialize(const std::string& jsonData, T& data)
  {
    SetJson(jsonData);
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeObject(*boundType, (char*)(&data));
  }

  void SetJson(const std::string& jsonData);
  virtual bool SerializePrimitive(const Field& field, char* data) override;
  virtual bool SerializeObject(const Field& field, char* data) override;
  virtual bool SerializeString(const Field& field, std::string& data) override;
  virtual bool SerializeArray(const Field& field, char* data, ArrayAdapter* adapter) override;
  virtual bool SerializePolymorphicArray(const Field& field, char* data, ArrayAdapter* adapter) override;

  virtual bool SerializeObject(BoundType& boundType, char* data) override;

  JsonInternalData* mData;
};