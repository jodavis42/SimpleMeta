#include "BinaryStream.hpp"

#include "MetaSerialization.hpp"
#include "SerializationHelpers.hpp"

BinarySaver::BinarySaver(std::ostream& stream) : mStream(stream.rdbuf())
{
  mDirection = SerializerDirection::Saving;
}

bool BinarySaver::SerializeProperties(BoundType& boundType, char* data)
{
  return ::SerializeProperties<BinarySaver>(*this, boundType, data);
}

bool BinarySaver::SerializePrimitive(const BoundType& boundType, char* data)
{
  Write(data, boundType.mSizeInBytes);
  return true;
}

bool BinarySaver::SerializePrimitive(const BoundType& boundType, std::string& data)
{
  Write(data.size());
  mStream.write(data.data(), data.size());
  return true;
}

bool BinarySaver::BeginMembers(size_t& count)
{
  return Write(count);
}

bool BinarySaver::BeginMember(size_t index, std::string& name)
{
  return SerializePrimitive(*StaticTypeId<std::string>::GetBoundType(), name);
}

bool BinarySaver::BeginObject(PolymorphicInfo& info)
{
  Write(info.mId.mId);
  return true;
}

bool BinarySaver::BeginArray(size_t& count)
{
  Write(count);
  return true;
}

bool BinarySaver::Write(char* data, size_t sizeInBytes)
{
  mStream.write(data, sizeInBytes);
  return true;
}

bool BinarySaver::Write(size_t data, size_t sizeInBytes)
{
  Write((char*)&data, sizeInBytes);
  return true;
}

bool BinarySaver::Write(size_t data)
{
  Write(data, sizeof(data));
  return true;
}

BinaryLoader::BinaryLoader(std::istream& stream) : mStream(stream.rdbuf())
{
  mDirection = SerializerDirection::Loading;
}

bool BinaryLoader::SerializeProperties(BoundType& boundType, char* data)
{
  return ::SerializeProperties<BinaryLoader>(*this, boundType, data);
}

bool BinaryLoader::SerializePrimitive(const BoundType& boundType, char* data)
{
  mStream.read(data, boundType.mSizeInBytes);
  return true;
}

bool BinaryLoader::SerializePrimitive(const BoundType& boundType, std::string& data)
{
  new(&data) std::string();
  size_t sizeInBytes = 0;
  Read(sizeInBytes);
  data.resize(sizeInBytes);
  mStream.read((char*)data.data(), sizeInBytes);
  return false;
}

bool BinaryLoader::BeginMembers(size_t& count)
{
  return Read(count);
}

bool BinaryLoader::BeginMember(size_t index, std::string& name)
{
  return SerializePrimitive(*StaticTypeId<std::string>::GetBoundType(), name);
}

bool BinaryLoader::BeginObject()
{
  return false;
}

bool BinaryLoader::BeginObject(PolymorphicInfo& info)
{
  return Read(info.mId.mId);
}

bool BinaryLoader::BeginArray(size_t& count)
{
  Read(count);
  return true;
}

bool BinaryLoader::EndArray()
{
  return true;
}

bool BinaryLoader::EndObject()
{
  return true;
}

bool BinaryLoader::SerializeObject(BoundType& boundType, char* data)
{
  return SerializeProperties(boundType, data);
}

bool BinaryLoader::Read(char* data, size_t sizeInBytes)
{
  mStream.read(data, sizeInBytes);
  return true;
}

bool BinaryLoader::Read(size_t& data, size_t sizeInBytes)
{
  data = 0;
  Read((char*)&data, sizeInBytes);
  return true;
}

bool BinaryLoader::Read(size_t& data)
{
  Read(data, sizeof(data));
  return true;
}
