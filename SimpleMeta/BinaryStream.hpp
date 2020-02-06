#pragma once

#include "StaticTypeId.hpp"
#include "Serializer.hpp"
#include <sstream>

struct BinarySaver : public Serializer
{
  BinarySaver()
  {
    mDirection = SerializerDirection::Saving;
  }

  template <typename T>
  void Serialize(const T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeObject(*boundType, (char*)(&data));
  }

  virtual bool SerializePrimitive(BoundType& boundType, char* data) override
  {
    mStream.write(data, boundType.mSizeInBytes);
    return true;
  }

  virtual bool SerializeObject(BoundType& boundType, char* data) override
  {
    for(size_t i = 0; i < boundType.mFields.size(); ++i)
    {
      const Field& field = boundType.mFields[i];
      BoundType* fieldType = field.mType;
      char* fieldSrc = data + field.mOffset;
      MetaSerialization* fieldSerialization = fieldType->mMetaSerialization;
      if(fieldSerialization != nullptr)
        fieldSerialization->Serialize(*this, *fieldType, fieldSrc);
      else
        SerializeObject(*fieldType, fieldSrc);
    }
    return true;
  }

  virtual bool SerializeString(BoundType& boundType, std::string& data) override
  {
    mStream << data.size();
    mStream.write(data.data(), data.size());
    return false;
  }

  virtual bool SerializeArrayCount(BoundType& boundType, size_t& count) override
  {
    mStream << count;
    return true;
  }
  virtual bool SerializeArray(BoundType& boundType, char* data, size_t count) override
  {
    BoundType* subType = boundType.mFields[0].mType;
    MetaSerialization* subTypeSerialization = subType->mMetaSerialization;
    for(size_t i = 0; i < count; ++i)
    {
      char* itemSrc = data + i * subType->mSizeInBytes;
      if(subTypeSerialization != nullptr)
        subTypeSerialization->Serialize(*this, *subType, itemSrc);
      else
        SerializeObject(*subType, itemSrc);
    }
    return false;
  }

  std::stringstream mStream;
};

struct BinaryLoader : public Serializer
{
  BinaryLoader()
  {
    mDirection = SerializerDirection::Loading;
  }

  template <typename T>
  void Serialize(T& data)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    SerializeObject(*boundType, (char*)(&data));
  }

  virtual bool SerializePrimitive(BoundType& boundType, char* data) override
  {
    mStream.read(data, boundType.mSizeInBytes);
    return true;
  }

  virtual bool SerializeObject(BoundType& boundType, char* data) override
  {
    for(size_t i = 0; i < boundType.mFields.size(); ++i)
    {
      const Field& field = boundType.mFields[i];
      BoundType* fieldType = field.mType;
      char* fieldSrc = data + field.mOffset;
      MetaSerialization* fieldSerialization = fieldType->mMetaSerialization;
      if(fieldSerialization != nullptr)
        fieldSerialization->Serialize(*this, *fieldType, fieldSrc);
      else
        SerializeObject(*fieldType, fieldSrc);
    }
    return true;
  }

  virtual bool SerializeString(BoundType& boundType, std::string& data) override
  {
    new(&data) std::string();
    size_t sizeInBytes = 0;
    mStream >> sizeInBytes;
    data.resize(sizeInBytes);
    mStream.read((char*)data.data(), sizeInBytes);
    return false;
  }
  virtual bool SerializeArrayCount(BoundType& boundType, size_t& count) override
  {
    mStream >> count;
    return true;
  }
  virtual bool SerializeArray(BoundType& boundType, char* data, size_t count) override
  {
    BoundType* subType = boundType.mFields[0].mType;
    MetaSerialization* subTypeSerialization = subType->mMetaSerialization;
    for(size_t i = 0; i < count; ++i)
    {
      char* itemSrc = data + i * subType->mSizeInBytes;
      if(subTypeSerialization != nullptr)
        subTypeSerialization->Serialize(*this, *subType, itemSrc);
      else
        SerializeObject(*subType, itemSrc);
    }
    return false;
  }

  std::stringstream mStream;
};
