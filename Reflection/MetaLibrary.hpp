#pragma once

#include <vector>
#include <unordered_map>
#include "BoundType.hpp"

struct BoundType;

struct ReflectionLibrary
{
  ~ReflectionLibrary();

  void AddDependency(ReflectionLibrary* dependency);
  void AddBoundType(BoundType* boundType);

  BoundType* FindBoundType(const std::string& name, bool recursive = true);
  BoundType* FindBoundType(const TypeId& id, bool recursive = true);

  std::string mName;
  std::vector<ReflectionLibrary*> mDependencies;
  std::vector<BoundType*> mBoundTypes;
  std::unordered_map<std::string, BoundType*> mBoundTypeNameMap;
  std::unordered_map<size_t, BoundType*> mBoundTypeIdMap;
};

struct ReflectionProject
{
  ~ReflectionProject();

  static ReflectionProject* GetInstance();
  static ReflectionLibrary& CreateLibrary(const std::string& name);
  static ReflectionLibrary* FindLibrary(const std::string& name);
  static void DestroyLibrary(ReflectionLibrary& library);

  static BoundType* FindBoundType(const std::string& name, bool recursive = true);
  static BoundType* FindBoundType(const TypeId& id, bool recursive = true);

  ReflectionLibrary* mCurrentLibrary = nullptr;
  std::vector<ReflectionLibrary*> mLibraries;
};
