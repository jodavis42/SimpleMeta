#pragma once

#include "DataDrivenTypes.hpp"

#include <filesystem>
#include <unordered_map>

class JsonLoader;
class JsonSaver;
struct BinaryLoader;
struct BinarySaver;

typedef std::unordered_map<std::string, DataDrivenType*> DataDrivenTypeNameMap;

BoundType* TryCreateArrayType(ReflectionLibrary& library, DataDrivenTypeNameMap& typeMap, const std::string& typeName);
BoundType* FindOrCreateBoundType(ReflectionLibrary& library, DataDrivenTypeNameMap& typeMap, const std::string& typeName);
void BuildType(ReflectionLibrary& library, DataDrivenType& type, DataDrivenTypeNameMap& typeMap);
void BuildTypes(ReflectionLibrary& library, DataDrivenTypes& types);

char* LoadJson(const std::filesystem::path& filePath, BoundType*& rootType);
void SaveJson(const std::filesystem::path& filePath, BoundType* rootType, char* data);
char* LoadBinary(const std::filesystem::path& filePath, BoundType*& rootType);
void SaveBinary(const std::filesystem::path& filePath, BoundType* rootType, char* data);

void RunDataDrivenFile(const std::filesystem::path& filePath, ReflectionLibrary& dataDrivenLibrary);
void RunDataDrivenFolder(const std::filesystem::path& folderPath, ReflectionLibrary& dataDrivenLibrary);
void RunDataDrivenTests(const std::filesystem::path& testsPath, ReflectionLibrary& dataDrivenLibrary);
