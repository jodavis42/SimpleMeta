#include "JsonSerializers.hpp"
#include "StaticTypeId.hpp"

#include "rapidjson.h"
#include "TestTypes/Math.hpp"

#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <stack>

class JsonInternalData
{
public:
  JsonInternalData() : mWriter(mBuffer)
  {
  }
  rapidjson::Document mDocument;
  rapidjson::StringBuffer mBuffer;
  rapidjson::Writer<rapidjson::StringBuffer> mWriter;
  rapidjson::Writer<rapidjson::StringBuffer> mReader;
  std::stack<rapidjson::Value*> mStack;
};

JsonSaver::JsonSaver()
{
  mData = new JsonInternalData();
}

JsonSaver::~JsonSaver()
{
  delete mData;
}

std::string JsonSaver::ToString()
{
  return mData->mBuffer.GetString();
}

bool JsonSaver::SerializePrimitive(const Field& field, char* data)
{
  BoundType* boundType = field.mType;
  mData->mWriter.Key(field.mName.c_str());
  if(boundType == StaticTypeId<char>::GetBoundType())
    mData->mWriter.Int(*data);
  else if(boundType == StaticTypeId<int>::GetBoundType())
    mData->mWriter.Int(*(int*)data);
  else if(boundType == StaticTypeId<float>::GetBoundType())
    mData->mWriter.Double(*(float*)data);
  else if(boundType == StaticTypeId<double>::GetBoundType())
    mData->mWriter.Double(*(double*)data);
  else if(boundType == StaticTypeId<Vec2>::GetBoundType())
  {
    Vec2* v2 = (Vec2*)data;
    mData->mWriter.StartObject();
    mData->mWriter.Key("x");
    mData->mWriter.Double(v2->x);
    mData->mWriter.Key("y");
    mData->mWriter.Double(v2->y);
    mData->mWriter.EndObject();
  }
  else if(boundType == StaticTypeId<Vec3>::GetBoundType())
  {
    Vec3* v3 = (Vec3*)data;
    mData->mWriter.StartArray();
    mData->mWriter.Double(v3->x);
    mData->mWriter.Double(v3->y);
    mData->mWriter.Double(v3->z);
    mData->mWriter.EndArray();
  }
  return true;
}

bool JsonSaver::SerializeObject(const Field& field, char* data)
{
  mData->mWriter.Key(field.mName.c_str());
  mData->mWriter.StartObject();

  BoundType& boundType = *field.mType;
  for(size_t i = 0; i < boundType.mFields.size(); ++i)
  {
    const Field& field = boundType.mFields[i];
    BoundType* fieldType = field.mType;
    char* fieldSrc = data + field.mOffset;
    MetaSerialization* fieldSerialization = fieldType->mMetaSerialization;
    if(fieldSerialization != nullptr)
      fieldSerialization->Serialize(*this, field, fieldSrc);
    else
      SerializeObject(field, fieldSrc);
  }
  mData->mWriter.EndObject();

  return true;
}

bool JsonSaver::SerializeString(const Field& field, std::string& data)
{
  mData->mWriter.Key(field.mName.c_str());
  mData->mWriter.String(data.c_str());
  return false;
}

bool JsonSaver::SerializeArray(const Field& field, char* data, ArrayAdapter* adapter)
{
  size_t count = adapter->GetCount(data);

  mData->mWriter.Key(field.mName.c_str());
  mData->mWriter.StartArray();
  BoundType* subType = adapter->GetSubType(*field.mType);
  MetaSerialization* subTypeSerialization = subType->mMetaSerialization;
  for(size_t i = 0; i < count; ++i)
  {
    char* itemSrc = adapter->GetItem(data, i);
    if(subTypeSerialization != nullptr)
      subTypeSerialization->Serialize(*this, *subType, itemSrc);
    else
      SerializeObject(*subType, itemSrc);
  }
  mData->mWriter.EndArray();
  return true;
}

bool JsonSaver::SerializeObject(BoundType& boundType, char* data)
{
  mData->mWriter.StartObject();

  for(size_t i = 0; i < boundType.mFields.size(); ++i)
  {
    const Field& field = boundType.mFields[i];
    BoundType* fieldType = field.mType;
    char* fieldSrc = data + field.mOffset;
    MetaSerialization* fieldSerialization = fieldType->mMetaSerialization;
    if(fieldSerialization != nullptr)
      fieldSerialization->Serialize(*this, field, fieldSrc);
    else
      SerializeObject(field, fieldSrc);
  }
  mData->mWriter.EndObject();
  return true;
}

JsonLoader::JsonLoader()
{
  mData = new JsonInternalData();
  mDirection = SerializerDirection::Loading;
}

JsonLoader::~JsonLoader()
{
  delete mData;
}

void JsonLoader::SetJson(const std::string& jsonData)
{
  mData->mDocument.Parse(jsonData.c_str());
  mData->mStack.push(&mData->mDocument);
}

bool JsonLoader::SerializePrimitive(const Field& field, char* data)
{
  auto valueNode = mData->mStack.top();

  BoundType* boundType = field.mType;
  if(boundType == StaticTypeId<char>::GetBoundType())
    *data = valueNode->GetInt();
  else if(boundType == StaticTypeId<int>::GetBoundType())
    *(int*)data = valueNode->GetInt();
  else if(boundType == StaticTypeId<float>::GetBoundType())
    *(float*)data = valueNode->GetFloat();
  else if(boundType == StaticTypeId<double>::GetBoundType())
    *(double*)data = valueNode->GetDouble();
  return true;
}

bool JsonLoader::SerializeObject(const Field& field, char* data)
{
  SerializeObject(*field.mType, data);
  return true;
}

bool JsonLoader::SerializeString(const Field& field, std::string& data)
{
  auto valueNode = mData->mStack.top();
  data = valueNode->GetString();
  return true;
}

bool JsonLoader::SerializeArray(const Field& field, char* data, ArrayAdapter* adapter)
{
  auto arrayNode = mData->mStack.top();
  auto array = arrayNode->GetArray();
  size_t count = array.Size();

  adapter->SetCount(data, count);
  
  BoundType& boundType = *field.mType;
  BoundType* subType = boundType.mFields[0].mType;
  MetaSerialization* subTypeSerialization = subType->mMetaSerialization;
  for(size_t i = 0; i < count; ++i)
  {
    auto itemNode = &array[i];
    mData->mStack.push(itemNode);

    char* itemSrc = adapter->GetItem(data, i);
    if(subTypeSerialization != nullptr)
      subTypeSerialization->Serialize(*this, *subType, itemSrc);
    else
      SerializeObject(*subType, itemSrc);

    mData->mStack.pop();
  }

  return true;
}

bool JsonLoader::SerializeObject(BoundType& boundType, char* data)
{
  auto objNode = mData->mStack.top();

  for(size_t i = 0; i < boundType.mFields.size(); ++i)
  {
    const Field& field = boundType.mFields[i];

    if(!objNode->HasMember(field.mName.c_str()))
      continue;
    
    auto it = objNode->FindMember(field.mName.c_str());
    mData->mStack.push(&it->value);
    
    BoundType* fieldType = field.mType;
    char* fieldSrc = data + field.mOffset;
    MetaSerialization* fieldSerialization = fieldType->mMetaSerialization;
    if(fieldSerialization != nullptr)
      fieldSerialization->Serialize(*this, field, fieldSrc);
    else
      SerializeObject(field, fieldSrc);
    mData->mStack.pop();
  }

  return true;
}
