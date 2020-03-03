#pragma once

#include "DataDrivenTypes.hpp"

#include <filesystem>
#include <unordered_map>

class JsonLoader;
class JsonSaver;
struct BinaryLoader;
struct BinarySaver;

typedef std::unordered_map<std::string, DataDrivenType*> DataDrivenTypeNameMap;

SimpleReflection::BoundType* TryCreateArrayType(SimpleReflection::ReflectionLibrary& library, DataDrivenTypeNameMap& typeMap, const std::string& typeName);
SimpleReflection::BoundType* FindOrCreateBoundType(SimpleReflection::ReflectionLibrary& library, DataDrivenTypeNameMap& typeMap, const std::string& typeName);
void BuildType(SimpleReflection::ReflectionLibrary& library, DataDrivenType& type, DataDrivenTypeNameMap& typeMap);
void BuildTypes(SimpleReflection::ReflectionLibrary& library, DataDrivenTypes& types);

char* LoadJson(const std::filesystem::path& filePath, SimpleReflection::BoundType*& rootType);
void SaveJson(const std::filesystem::path& filePath, SimpleReflection::BoundType* rootType, char* data);
char* LoadBinary(const std::filesystem::path& filePath, SimpleReflection::BoundType*& rootType);
void SaveBinary(const std::filesystem::path& filePath, SimpleReflection::BoundType* rootType, char* data);

void RunDataDrivenFile(const std::filesystem::path& filePath, SimpleReflection::ReflectionLibrary& dataDrivenLibrary);
void RunDataDrivenFolder(const std::filesystem::path& folderPath, SimpleReflection::ReflectionLibrary& dataDrivenLibrary);
void RunDataDrivenTests(const std::filesystem::path& testsPath, SimpleReflection::ReflectionLibrary& dataDrivenLibrary);
