#include "DataDrivenTesting.hpp"
#include "DataDrivenSerializationPolicies.hpp"

#include "JsonSerializers.hpp"
#include "BinaryStream.hpp"

#include "Field.hpp"
#include "GetterSetter.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>

SimpleReflection::BoundType* TryCreateArrayType(ReflectionLibrary& library, DataDrivenTypeNameMap& typeMap, const std::string& typeName)
{
  // Check if the name of this type was an array
  std::regex regex("Array<([\\d\\w]+)>");
  std::smatch match;
  if(!std::regex_match(typeName, match, regex))
    return nullptr;

  // Extract the template param
  std::string templateParam = match[1];
  // If the T type wasn't already created then try and create it
  BoundType* templateParamType = library.FindBoundType(templateParam);
  if(templateParamType == nullptr && typeMap.find(templateParam) != typeMap.end())
  {
    BuildType(library, *typeMap[templateParam], typeMap);
  }
  if(templateParamType == nullptr)
  {
    // Failed to create template param. Typo?
    __debugbreak();
  }
  
  BoundType* boundType = new BoundType();
  boundType->mName = typeName;
  boundType->AddComponentType<GenericArrayBoundTypeMetaSerialization>(templateParamType);
  boundType->mSizeInBytes = sizeof(std::vector<char>);
  return boundType;
}

SimpleReflection::BoundType* FindOrCreateBoundType(SimpleReflection::ReflectionLibrary& library, DataDrivenTypeNameMap& typeMap, const std::string& typeName)
{
  // If the type already exists, just return it
  SimpleReflection::BoundType* boundType = library.FindBoundType(typeName);
  if(boundType != nullptr)
    return boundType;
  
  // If the type is an array then create it
  boundType = TryCreateArrayType(library, typeMap, typeName);
  if(boundType != nullptr)
    return boundType;

  // Otherwise just build the type
  BuildType(library, *typeMap[typeName], typeMap);
  boundType = library.FindBoundType(typeName);

  return boundType;
}

void BuildType(SimpleReflection::ReflectionLibrary& library, DataDrivenType& type, DataDrivenTypeNameMap& typeMap)
{
  // New dynamic ids to use
  static size_t dynamicId = 1000;

  // If this type has already been built, then skip this
  BoundType* boundType = library.FindBoundType(type.mName);
  if (boundType != nullptr)
    return;

  // Setup the basic type data
  boundType = new BoundType();
  boundType->mName = type.mName;
  boundType->mId.mId = dynamicId;
  ++dynamicId;

  size_t fieldOffset = 0;
  boundType->AddComponentType<GenericBoundTypeMetaSerialization>(boundType);
  // If there's a base type, append the base type fields
  if (!type.mBaseType.empty())
  {
    boundType->mBaseType = FindOrCreateBoundType(library, typeMap, type.mBaseType);
    for (SimpleReflection::Field* baseField : boundType->mBaseType->mFields)
    {
      SimpleReflection::Field* newField = new SimpleReflection::Field();
      *newField = *baseField;
      boundType->mFields.push_back(newField);
      fieldOffset += baseField->mOffset;
    }
  }
  
  // Build all of the fields for this type
  for (DataDrivenField& dataDrivenField : type.mFields)
  {
    SimpleReflection::Field* newField = new SimpleReflection::Field();
    newField->mName = dataDrivenField.mName;
    newField->mType = FindOrCreateBoundType(library, typeMap, dataDrivenField.mType);
    newField->mOffset = fieldOffset;
    fieldOffset += newField->mType->mSizeInBytes;
    boundType->mFields.push_back(newField);
  }
  boundType->mSizeInBytes = fieldOffset;
  library.AddBoundType(boundType);
}

void BuildTypes(SimpleReflection::ReflectionLibrary& library, DataDrivenTypes& types)
{
  DataDrivenTypeNameMap typeMap;
  // Build a map of all typenames in this library so we can deal with them being in a random order
  for (DataDrivenType& type : types.mTypes)
  {
    typeMap[type.mName] = &type;
  }

  for (DataDrivenType& type : types.mTypes)
  {
    // If this type was already built then skip it
    if (library.FindBoundType(type.mName, true) != nullptr)
      continue;

    BuildType(library, type, typeMap);
  }
}

bool LoadDataDrivenTypes(const std::filesystem::path& boundTypesPath, SimpleReflection::ReflectionLibrary& library)
{
  if(!std::filesystem::exists(boundTypesPath))
    return false;
  
  std::ifstream inStream;
  inStream.open(boundTypesPath.c_str(), std::ifstream::binary | std::ifstream::in);
  std::stringstream strStream;
  strStream << inStream.rdbuf();

  JsonLoader loader;
  DataDrivenTypes types;
  loader.Serialize(strStream.str(), types);
  BuildTypes(library, types);
  return true;
}

char* LoadJson(const std::filesystem::path& filePath, SimpleReflection::BoundType*& rootType)
{
  JsonLoader loader;
  loader.LoadFromFile(filePath.string());
  
  std::string rootTypeName;
  loader.BeginMember("RootType");
  loader.SerializePrimitive(*StaticTypeId<std::string>::GetBoundType(), rootTypeName);
  loader.EndMember();

  rootType = SimpleReflection::ReflectionProject::FindBoundType(rootTypeName);
  MetaSerialization* metaSerialization = rootType->QueryComponentType<MetaSerialization>();
  char* data = metaSerialization->Allocate();

  loader.BeginMember(rootType->mName);
  loader.SerializeObject(*rootType, data);
  loader.EndMember();

  return data;
}

void SaveJson(const std::filesystem::path& filePath, SimpleReflection::BoundType* rootType, char* data)
{
  JsonSaver saver;

  saver.BeginObject();
  saver.BeginMember("RootType");
  saver.SerializePrimitive(*StaticTypeId<std::string>::GetBoundType(), rootType->mName);
  saver.EndMember();

  saver.BeginMember(rootType->mName);
  saver.SerializeObject(*rootType, data);
  saver.EndMember();
  saver.EndObject();

  std::string outData = saver.ToString();

  std::ofstream outStream;
  outStream.open(filePath.c_str(), std::ofstream::out);
  outStream << outData;
  outStream.close();
}

char* LoadBinary(const std::filesystem::path& filePath, SimpleReflection::BoundType*& rootType)
{
  std::ifstream inStream;
  inStream.open(filePath.c_str(), std::ifstream::binary | std::ifstream::in);
  std::stringstream memoryStream;

  memoryStream << inStream.rdbuf();
  BinaryLoader loader(memoryStream);

  MetaSerialization* metaSerialization = rootType->QueryComponentType<MetaSerialization>();
  char* data = metaSerialization->Allocate();
  loader.SerializeObject(*rootType, data);
  return data;
}

void SaveBinary(const std::filesystem::path& filePath, SimpleReflection::BoundType* rootType, char* data)
{
  std::stringstream memoryStream;
  BinarySaver saver(memoryStream);
  saver.SerializeObject(*rootType, data);

  std::ofstream outStream;
  outStream.open(filePath.c_str(), std::ofstream::binary | std::ofstream::out);
  outStream << memoryStream.rdbuf();
}

void RunDataDrivenFile(const std::filesystem::path& filePath, ReflectionLibrary& dataDrivenLibrary)
{
  std::filesystem::path jsonOutFilePath = filePath;
  jsonOutFilePath.replace_extension("out.json");

  BoundType* boundType;
  char* loadedJsonData = LoadJson(filePath, boundType);

  JsonSaver saver;
  SaveJson(jsonOutFilePath, boundType, loadedJsonData);

  std::filesystem::path binaryOutFilePath = filePath;
  binaryOutFilePath.replace_extension("out.bin");
  SaveBinary(binaryOutFilePath, boundType, loadedJsonData);
  char* binaryData = LoadBinary(binaryOutFilePath, boundType);
  memcmp(binaryData, loadedJsonData, boundType->mSizeInBytes);
}

void RunDataDrivenFolder(const std::filesystem::path& folderPath, SimpleReflection::ReflectionLibrary& dataDrivenLibrary)
{
  ReflectionLibrary& scriptLibrary = SimpleReflection::ReflectionProject::CreateLibrary("Script");
  scriptLibrary.AddDependency(&dataDrivenLibrary);

  std::filesystem::path boundTypesPath = folderPath;
  // Load the list of bound types
  boundTypesPath.append("BoundTypes.json");
  if(!LoadDataDrivenTypes(boundTypesPath, scriptLibrary))
    return;

  // Find any json file that wasn't the bound tyeps and run it as a test
  for(const std::filesystem::directory_entry& dir : std::filesystem::directory_iterator(folderPath))
  {
    std::filesystem::path filePath = dir;
    std::string extension = filePath.extension().string();
    std::string stem = filePath.stem().extension().string();
    if(dir.is_directory() || dir.path() == boundTypesPath || extension != ".json" || stem == ".out")
      continue;

    RunDataDrivenFile(filePath, scriptLibrary);
  }

  SimpleReflection::ReflectionProject::DestroyLibrary(scriptLibrary);
}

void RunDataDrivenTests(const std::filesystem::path& testsPath, SimpleReflection::ReflectionLibrary& dataDrivenLibrary)
{
  if(!std::filesystem::is_directory(testsPath))
    return;

  for(const std::filesystem::directory_entry& dir : std::filesystem::directory_iterator(testsPath))
  {
    std::filesystem::path path = dir;
    if(std::filesystem::is_directory(path))
    {
      RunDataDrivenFolder(path, dataDrivenLibrary);
      RunDataDrivenTests(path, dataDrivenLibrary);
    }
  }
}
