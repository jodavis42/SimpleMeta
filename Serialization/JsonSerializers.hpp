#pragma once

#include "Serializer.hpp"

class JsonInternalData;

class JsonSaver : public Serializer
{
public:
  // Force all overloads to be included
  using Serializer::SerializePrimitive;
  using Serializer::BeginObject;
  using Serializer::BeginMember;

  JsonSaver();
  ~JsonSaver();
  std::string ToString();

  template <typename T>
  void Serialize(const T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeObject(*boundType, (char*)(&data));
  }

  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, bool& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, char& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, int& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, float& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, double& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

  virtual bool BeginObject() override;
  virtual bool BeginObject(PolymorphicInfo& info) override;
  virtual bool BeginMembers(size_t& count) override;
  virtual bool BeginMember(const std::string& name) override;
  virtual bool BeginMember(size_t index, std::string& name) override;
  virtual bool BeginArray(size_t& count) override;
  virtual bool BeginArrayItem(size_t index) override;
  virtual bool EndObject() override;
  virtual bool EndMember() override;
  virtual bool EndMembers() override;
  virtual bool EndArray() override;
  virtual bool EndArrayItem() override;
  
  virtual bool SerializeObject(BoundType& boundType, char* data) override;

  bool WriteKey(const std::string& name);
  bool WritePrimitive(bool data);
  bool WritePrimitive(char data);
  bool WritePrimitive(int data);
  bool WritePrimitive(float data);
  bool WritePrimitive(double data);
  bool WritePrimitive(const std::string& data);

  JsonInternalData* mData;
};

class JsonLoader : public Serializer
{
public:
  // Force all overloads to be included
  using Serializer::SerializePrimitive;
  using Serializer::BeginObject;
  using Serializer::BeginMember;

  JsonLoader();
  ~JsonLoader();

  void Load(const std::string& jsonData);
  void LoadFromFile(const std::string& filePath);

  template <typename T>
  void Serialize(const std::string& jsonData, T& data)
  {
    Load(jsonData);
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeProperties(*boundType, (char*)(&data));
  }

  bool SerializeProperties(BoundType& boundType, char* data);

  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, bool& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, char& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, int& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, float& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, double& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

  virtual bool BeginObject() override;
  virtual bool BeginObject(PolymorphicInfo& info) override;
  virtual bool BeginMembers(size_t& count) override;
  virtual bool BeginMember(const std::string& name) override;
  virtual bool BeginMember(size_t index, std::string& name) override;
  virtual bool BeginArray(size_t& count) override;
  virtual bool BeginArrayItem(size_t index) override;
  virtual bool EndObject() override;
  virtual bool EndMembers() override;
  virtual bool EndMember() override;
  virtual bool EndArray() override;
  virtual bool EndArrayItem() override;

  bool ReadPrimitive(bool& data);
  bool ReadPrimitive(char& data);
  bool ReadPrimitive(int& data);
  bool ReadPrimitive(float& data);
  bool ReadPrimitive(double& data);
  bool ReadPrimitive(std::string& data);

  bool BeginMember();
  bool End();

  JsonInternalData* mData;
};