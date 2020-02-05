#pragma once

#include "StaticTypeId.hpp"

struct BinaryStream
{
  template <typename T>
  void Write(const T& data, std::vector<char>& outData)
  {
      BoundType* boundType = StaticTypeId<T>::GetBoundType();
      Write(boundType, reinterpret_cast<const char*>(&data), outData);
  }

  void Write(const BoundType* boundType, const char* data, std::vector<char>& outData)
  {
      for (size_t i = 0; i < boundType->mFields.size(); ++i)
      {
          const Field& field = boundType->mFields[i];
          const char* fieldSrc = data + field.mOffset;
          if (field.mType == StaticTypeId<char>::GetBoundType())
              Write(*reinterpret_cast<const char*>(fieldSrc), outData);
          else if (field.mType == StaticTypeId<int>::GetBoundType())
              Write(*reinterpret_cast<const int*>(fieldSrc), outData);
          else if (field.mType == StaticTypeId<float>::GetBoundType())
              Write(*reinterpret_cast<const float*>(fieldSrc), outData);
          else if (field.mType == StaticTypeId<double>::GetBoundType())
              Write(*reinterpret_cast<const float*>(fieldSrc), outData);
          else if (field.mType == StaticTypeId<std::string>::GetBoundType())
              Write(*reinterpret_cast<const std::string*>(fieldSrc), outData);
		  else if (field.mType->mPrimitiveType == BoundTypePrimitiveType::Array)
			  WriteArray(field.mType, fieldSrc, outData);
          else
              Write(field.mType, fieldSrc, outData);
      }
  }

  template<typename T>
  void WritePrimitive(const T& data, std::vector<char>& outData)
  {
      size_t dataSize = sizeof(data);
      size_t oldSize = outData.size();
      outData.resize(oldSize + dataSize);
      memcpy(outData.data() + oldSize, &data, dataSize);
  }
  void WriteDynamicData(const void* data, std::vector<char>& outData, size_t dataSize)
  {
      size_t indexSize = sizeof(dataSize);
	  size_t oldSize = outData.size();
	  outData.resize(oldSize + dataSize + indexSize);
      memcpy(outData.data() + oldSize, &dataSize, indexSize);
	  memcpy(outData.data() + oldSize + indexSize, data, dataSize);
  }

  template <> void Write<char>(const char& data, std::vector<char>& outData) { WritePrimitive(data, outData); }
  template <> void Write<int>(const int& data, std::vector<char>& outData) { WritePrimitive(data, outData); }
  template <> void Write<float>(const float& data, std::vector<char>& outData) { WritePrimitive(data, outData); }
  template <> void Write<double>(const double& data, std::vector<char>& outData) { WritePrimitive(data, outData); }
  template <> void Write<std::string>(const std::string& data, std::vector<char>& outData)
  {
      WriteDynamicData(data.data(), outData, data.size());
  }
  void WriteArray(const BoundType* boundType, const char* srcData, std::vector<char>& outData)
  {
      auto dummyArray = reinterpret_cast<const std::vector<char*>*>(srcData);
      size_t size = dummyArray->size();
      const char* arrayDataStart = reinterpret_cast<const char*>(dummyArray->data());
      // This won't work with dynamically sized data inside the array (e.g., array of strings)
      WriteDynamicData(arrayDataStart, outData, size * boundType->mStride);
  }

  template<typename T>
  size_t ReadPrimitive(const std::vector<char>& data, T& outData, size_t offset)
  {
	  size_t dataSize = sizeof(outData);
	  memcpy(&outData, data.data() + offset, dataSize);
      return dataSize;
  }  

  template <typename T>
  size_t Read(const std::vector<char>& data, T& outData)
  {
	  BoundType* boundType = StaticTypeId<T>::GetBoundType();
	  return Read(boundType, data, reinterpret_cast<char*>(&outData), 0);
  }

  size_t Read(const BoundType* boundType, const std::vector<char>& data, char* outData, size_t offset)
  {
      const char* source = data.data() + offset;
	  for (size_t i = 0; i < boundType->mFields.size(); ++i)
	  {
		  const Field& field = boundType->mFields[i];
		  char* fieldDst = outData + field.mOffset;
		  if (field.mType == StaticTypeId<char>::GetBoundType())
			  offset += Read(data, *reinterpret_cast<char*>(fieldDst), offset);
		  else if (field.mType == StaticTypeId<int>::GetBoundType())
              offset += Read(data, *reinterpret_cast<int*>(fieldDst), offset);
		  else if (field.mType == StaticTypeId<float>::GetBoundType())
              offset += Read(data, *reinterpret_cast<float*>(fieldDst), offset);
		  else if (field.mType == StaticTypeId<double>::GetBoundType())
              offset += Read(data, *reinterpret_cast<float*>(fieldDst), offset);
		  else if (field.mType == StaticTypeId<std::string>::GetBoundType())
              offset += Read(data, *reinterpret_cast<std::string*>(fieldDst), offset);
		  else
              offset += Read(field.mType, data, fieldDst, offset);
	  }
      return source + offset - source;
  }

  template <typename T> size_t Read(const std::vector<char>& data, T& outData, size_t offset) { return 0; }
  template <> size_t Read<char>(const std::vector<char>& data, char& outData, size_t offset) { return ReadPrimitive(data, outData, offset); }
  template <> size_t Read<int>(const std::vector<char>& data, int& outData, size_t offset) { return ReadPrimitive(data, outData, offset); }
  template <> size_t Read<float>(const std::vector<char>& data, float& outData, size_t offset) { return ReadPrimitive(data, outData, offset); }
  template <> size_t Read<double>(const std::vector<char>& data, double& outData, size_t offset) { return ReadPrimitive(data, outData, offset); }
  template <> size_t Read<std::string>(const std::vector<char>& data, std::string& outData, size_t offset)
  {
	  size_t count = 0;
	  memcpy(&count, data.data() + offset, sizeof(count));
      new(&outData) std::string();
      outData.resize(count);
      memcpy(const_cast<char*>(outData.data()), data.data() + offset + sizeof(count), count);
      return count + sizeof(size_t);
  }
};