#pragma once

#include "StaticTypeId.hpp"

struct BinaryStream
{
  template <typename T>
  void Write(const T& data, std::vector<char>& outData)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    size_t totalSize = 0;
    for(size_t i = 0; i < boundType->mFields.size(); ++i)
      totalSize += boundType->mFields[i].mType->mSizeInBytes;

    outData.resize(totalSize);
    for(size_t i = 0; i < boundType->mFields.size(); ++i)
    {
      Field& field = boundType->mFields[i];
      const char* src = reinterpret_cast<const char*>(&data);
      char* dest = outData.data();
      memcpy(dest + field.mOffset, src + field.mOffset, field.mType->mSizeInBytes);
    }
  }

  template <typename T>
  void Read(std::vector<char>& data, T& outData)
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    for(size_t i = 0; i < boundType->mFields.size(); ++i)
    {
      Field& field = boundType->mFields[i];
      char* src = data.data();
      char* dest = reinterpret_cast<char*>(&outData);
      
      memcpy(dest + field.mOffset, src + field.mOffset, field.mType->mSizeInBytes);
    }
  }
};