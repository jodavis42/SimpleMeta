#include "JsonSerializers.hpp"
#include "StaticTypeId.hpp"

#include "rapidjson.h"
#include "TestTypes/Math.hpp"

#include "document.h"
#include "prettywriter.h"
#include "writer.h"
#include "stringbuffer.h"
#include <stack>
#include <fstream>
#include <sstream>

class JsonInternalData
{
public:
  JsonInternalData() : mWriter(mBuffer)
  {
  }
  rapidjson::Document mDocument;
  rapidjson::StringBuffer mBuffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> mWriter;
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

bool JsonSaver::SerializePrimitive(const BoundType& boundType, char* data)
{
  __debugbreak();
  return false;
}

bool JsonSaver::SerializePrimitive(const BoundType& boundType, bool& data)
{
  return WritePrimitive(data);
}

bool JsonSaver::SerializePrimitive(const BoundType& boundType, char& data)
{
  return WritePrimitive(data);
}

bool JsonSaver::SerializePrimitive(const BoundType& boundType, int& data)
{
  return WritePrimitive(data);
}

bool JsonSaver::SerializePrimitive(const BoundType& boundType, float& data)
{
  return WritePrimitive(data);
}

bool JsonSaver::SerializePrimitive(const BoundType& boundType, double& data)
{
  return WritePrimitive(data);
}

bool JsonSaver::SerializePrimitive(const BoundType& boundType, std::string& data)
{
  return WritePrimitive(data);
}

bool JsonSaver::BeginObject()
{
  return mData->mWriter.StartObject();
}

bool JsonSaver::BeginObject(PolymorphicInfo& info)
{
  BeginObject();
  WriteKey("Typename");
  WritePrimitive(info.mName);
  WriteKey("Value");
  return true;
}

bool JsonSaver::BeginMembers(size_t& count)
{
  return BeginObject();
}

bool JsonSaver::BeginMember(const std::string& name)
{
  return WriteKey(name);
}

bool JsonSaver::BeginMember(size_t index, std::string& name)
{
  return WriteKey(name);
}

bool JsonSaver::BeginArray(size_t& count)
{
  return mData->mWriter.StartArray();
}

bool JsonSaver::BeginArrayItem(size_t index)
{
  //return BeginObject();
  return true;
}

bool JsonSaver::EndObject()
{
  return mData->mWriter.EndObject();
}

bool JsonSaver::EndMembers()
{
  return EndObject();
}

bool JsonSaver::EndMember()
{
  return true;
}

bool JsonSaver::EndArray()
{
  return mData->mWriter.EndArray();
}

bool JsonSaver::EndArrayItem()
{
  //return EndObject();
  return true;
}

bool JsonSaver::SerializeObject(BoundType& boundType, char* data)
{
  BeginObject();
  SerializeProperties(*this, boundType, data);
  EndObject();
  return true;
}

bool JsonSaver::WriteKey(const std::string& name)
{
  return mData->mWriter.Key(name.c_str());
}

bool JsonSaver::WritePrimitive(bool data)
{
  return mData->mWriter.Bool(data);
}

bool JsonSaver::WritePrimitive(char data)
{
  return mData->mWriter.Int(data);
}

bool JsonSaver::WritePrimitive(int data)
{
  return mData->mWriter.Int(data);
}

bool JsonSaver::WritePrimitive(float data)
{
  return mData->mWriter.Double(data);
}

bool JsonSaver::WritePrimitive(double data)
{
  return mData->mWriter.Double(data);
}

bool JsonSaver::WritePrimitive(const std::string& data)
{
  return mData->mWriter.String(data.c_str());
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

void JsonLoader::Load(const std::string& jsonData)
{
  mData->mDocument.Parse(jsonData.c_str());
  mData->mStack.push(&mData->mDocument);
}

void JsonLoader::LoadFromFile(const std::string& filePath)
{
  std::ifstream stream;
  stream.open(filePath, std::ifstream::in);

  std::stringstream strStream;
  strStream << stream.rdbuf();

  Load(strStream.str());
}

bool JsonLoader::SerializePrimitive(const BoundType& boundType, char* data)
{
  __debugbreak();
  return false;
}

bool JsonLoader::SerializePrimitive(const BoundType& boundType, bool& data)
{
  return ReadPrimitive(data);
}

bool JsonLoader::SerializePrimitive(const BoundType& boundType, char& data)
{
  return ReadPrimitive(data);
}

bool JsonLoader::SerializePrimitive(const BoundType& boundType, int& data)
{
  return ReadPrimitive(data);
}

bool JsonLoader::SerializePrimitive(const BoundType& boundType, float& data)
{
  return ReadPrimitive(data);
}

bool JsonLoader::SerializePrimitive(const BoundType& boundType, double& data)
{
  return ReadPrimitive(data);
}

bool JsonLoader::SerializePrimitive(const BoundType& boundType, std::string& data)
{
  return ReadPrimitive(data);
}

bool JsonLoader::BeginObject()
{
  return BeginMember();
}

bool JsonLoader::BeginObject(PolymorphicInfo& info)
{
  rapidjson::Value* node = mData->mStack.top();

  auto typenameIt = node->FindMember("Typename");
  rapidjson::Value& typenameNode = typenameIt->value;
  info.mName = typenameNode.GetString();

  auto valueIt = node->FindMember("Value");
  rapidjson::Value& valueNode = valueIt->value;
  mData->mStack.push(&valueNode);

  return true;
}

bool JsonLoader::BeginMembers(size_t& count)
{
  rapidjson::Value* node = mData->mStack.top();
  count = node->MemberCount();
  return true;
}

bool JsonLoader::BeginMember(const std::string& name)
{
  rapidjson::Value* node = mData->mStack.top();
  auto it = node->FindMember(name.c_str());
  if(it == node->MemberEnd())
    return false;

  rapidjson::Value& valueNode = it->value;
  mData->mStack.push(&valueNode);
  return true;
}

bool JsonLoader::BeginMember(size_t index, std::string& name)
{
  rapidjson::Value* node = mData->mStack.top();
  if(index >= node->MemberCount())
    return false;
  auto it = node->MemberBegin();
  it += index;

  name = it->name.GetString();

  rapidjson::Value& valueNode = it->value;
  mData->mStack.push(&valueNode);
  return true;
}

bool JsonLoader::BeginArray(size_t& count)
{
  rapidjson::Value* arrayNode = mData->mStack.top();
  auto array = arrayNode->GetArray();
  count = array.Size();
  return false;
}

bool JsonLoader::BeginArrayItem(size_t index)
{
  rapidjson::Value* arrayNode = mData->mStack.top();
  auto array = arrayNode->GetArray();
  if(index >= array.Size())
    return false;

  rapidjson::Value& arrayItemNode = array[(int)index];
  mData->mStack.push(&arrayItemNode);
  return true;
}

bool JsonLoader::EndObject()
{
  return End();
}

bool JsonLoader::EndMembers()
{
  return End();
}

bool JsonLoader::EndMember()
{
  return End();
}

bool JsonLoader::EndArrayItem()
{
  return EndObject();
}

bool JsonLoader::EndArray()
{
  return true;
}

bool JsonLoader::ReadPrimitive(bool& data)
{
  rapidjson::Value* node = mData->mStack.top();
  if(!node->IsBool())
    return false;

  data = node->GetBool();
  return true;
}

bool JsonLoader::ReadPrimitive(char& data)
{
  rapidjson::Value* node = mData->mStack.top();
  if(!node->IsInt())
    return false;

  data = node->GetInt();
  return true;
}

bool JsonLoader::ReadPrimitive(int& data)
{
  rapidjson::Value* node = mData->mStack.top();
  if(!node->IsInt())
    return false;

  data = node->GetInt();
  return true;
}

bool JsonLoader::ReadPrimitive(float& data)
{
  rapidjson::Value* node = mData->mStack.top();
  if(!node->IsFloat())
    return false;

  data = node->GetFloat();
  return true;
}

bool JsonLoader::ReadPrimitive(double& data)
{
  rapidjson::Value* node = mData->mStack.top();
  if(!node->IsDouble())
    return false;

  data = node->GetDouble();
  return true;
}

bool JsonLoader::ReadPrimitive(std::string& data)
{
  rapidjson::Value* node = mData->mStack.top();
  if(!node->IsString())
    return false;

  data = node->GetString();
  return true;
}

bool JsonLoader::BeginMember()
{
  rapidjson::Value* node = mData->mStack.top();

  auto it = node->MemberBegin();
  if(it == node->MemberEnd())
    return false;

  rapidjson::Value& valueNode = it->value;
  mData->mStack.push(&valueNode);
  return true;
}

bool JsonLoader::End()
{
  if(mData->mStack.empty())
    return false;
  mData->mStack.pop();
  return true;
}
