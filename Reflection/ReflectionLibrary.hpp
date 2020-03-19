#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "BoundType.hpp"
#include "FunctionContainers.hpp"

namespace SimpleReflection
{

struct BoundType;

/// A library of types that depends on other libraries. Facilitates search for types by id or name and owning the memory for bound types.
struct ReflectionLibrary
{
  ~ReflectionLibrary();

  void AddDependency(ReflectionLibrary* dependency);
  void AddBoundType(BoundType* boundType);
  void Finalize();
  bool Validate();

  BoundType* FindBoundType(const std::string& name, bool recursive = true) const;
  BoundType* FindBoundType(const TypeId& id, bool recursive = true) const;
  Function* FindExtensionFunction(const BoundType* owner, const std::string& fnName, const FunctionType& fnType, bool recursive = true) const;

  std::string mName;
  std::vector<ReflectionLibrary*> mDependencies;
  std::vector<BoundType*> mBoundTypes;
  std::unordered_map<std::string, BoundType*> mBoundTypeNameMap;
  std::unordered_map<size_t, BoundType*> mBoundTypeIdMap;
  std::unordered_set<BoundType*> mBoundTypesToRegister;
  ExtensionFunctionMultiMap mExtensionFunctions;

  // For a library to be finalized, it's only able to accept new ref/pointer types, but no new actual direct bound types.
  bool mIsFinalized = false;
};

/// A project that owns all of the reflection libraries and contains a singleton interface to allow easy querying of a type.
struct ReflectionProject
{
  ~ReflectionProject();
  void Shutdown();

  static ReflectionProject* GetInstance();
  static ReflectionLibrary& CreateLibrary(const std::string& name);
  static ReflectionLibrary* FindLibrary(const std::string& name);
  static void DestroyLibrary(ReflectionLibrary& library);

  static BoundType* FindBoundType(const std::string& name, bool recursive = true);
  static BoundType* FindBoundType(const TypeId& id, bool recursive = true);

  ReflectionLibrary* mCurrentLibrary = nullptr;
  std::vector<ReflectionLibrary*> mLibraries;
  bool mShutdown = false;
};

}//namespace SimpleReflection
