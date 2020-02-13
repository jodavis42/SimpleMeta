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
    BeginObject();
    SerializeProperties(*boundType, (char*)(&data));
    EndObject();
  }

  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, bool& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, char& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, int& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, float& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, double& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

  virtual bool BeginStringTable(int& count) override;
  virtual bool EndStringTable() override;
  virtual bool BeginStringTableEntry(std::string& name) override;
  virtual bool EndStringTableEntry() override;

  virtual bool BeginObject() override;
  virtual bool BeginObject(PolymorphicInfo& info) override;
  virtual bool BeginMember(const std::string& name) override;
  virtual bool BeginArray(size_t& count) override;
  virtual bool BeginArrayItem(size_t index) override;
  virtual bool EndObject() override;
  virtual bool EndMember() override;
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
  JsonLoader();
  ~JsonLoader();

  template <typename T>
  void Serialize(const std::string& jsonData, T& data)
  {
    SetJson(jsonData);
    BoundType* boundType = StaticTypeId<T>::GetBoundType();

    SerializeProperties(*boundType, (char*)(&data));
  }

  void SetJson(const std::string& jsonData);

  virtual bool SerializePrimitive(const BoundType& boundType, char* data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, bool& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, char& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, int& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, float& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, double& data) override;
  virtual bool SerializePrimitive(const BoundType& boundType, std::string& data) override;

  virtual bool BeginStringTable(int& count) override;
  virtual bool EndStringTable() override;
  virtual bool BeginStringTableEntry(std::string& name) override;
  virtual bool EndStringTableEntry() override;
  virtual bool ForAllMembers(int size, std::function<void(Serializer&, const std::string&)> callback) override;

  virtual bool BeginObject() override;
  virtual bool BeginObject(PolymorphicInfo& info) override;
  virtual bool BeginMember(const std::string& name) override;
  virtual bool BeginArray(size_t& count) override;
  virtual bool BeginArrayItem(size_t index) override;
  virtual bool EndObject() override;
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