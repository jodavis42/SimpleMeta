#include "MetaLibrary.hpp"

std::vector<ReflectionLibrary*> ReflectionProject::mLibraries;

void ReflectionLibrary::AddBoundType(BoundType* boundType)
{
  mBoundTypes.push_back(boundType);
  mBoundTypeMap[boundType->mName] = boundType;
  mBoundTypeIdMap[boundType->mId.mId] = boundType;
}

BoundType* ReflectionLibrary::FindBoundType(const std::string& name, bool recursive)
{
  auto it = mBoundTypeMap.find(name);
  if(it != mBoundTypeMap.end())
    return it->second;

  for(ReflectionLibrary* dependency : mDependencies)
  {
    BoundType* result = dependency->FindBoundType(name, recursive);
    if(result != nullptr)
      return result;
  }

  return nullptr;
}

BoundType* ReflectionLibrary::FindBoundType(const TypeId& id, bool recursive)
{
  auto it = mBoundTypeIdMap.find(id.mId);
  if(it != mBoundTypeIdMap.end())
    return it->second;

  for(ReflectionLibrary* dependency : mDependencies)
  {
    BoundType* result = dependency->FindBoundType(id, recursive);
    if(result != nullptr)
      return result;
  }

  return nullptr;
}

void ReflectionLibrary::AddDependency(ReflectionLibrary* dependency)
{
  mDependencies.push_back(dependency);
}

ReflectionLibrary& ReflectionProject::CreateLibrary(const std::string& name)
{
  ReflectionLibrary* library = new ReflectionLibrary();
  library->mName = name;
  mLibraries.push_back(library);
  return *library;
}

ReflectionLibrary* ReflectionProject::FindLibrary(const std::string& name)
{
  for(ReflectionLibrary* library : mLibraries)
  {
    if(library->mName == name)
      return library;
  }
  return nullptr;
}

void ReflectionProject::DestroyLibrary(ReflectionLibrary& library)
{
  for(auto it = mLibraries.begin(); it != mLibraries.end(); ++it)
  {
    if(*it == &library)
    {
      mLibraries.erase(it);
      return;
    }
  }
}

BoundType* ReflectionProject::FindBoundType(const std::string& name, bool recursive)
{
  if(mLibraries.empty())
    return nullptr;

  ReflectionLibrary* topLibrary = mLibraries.back();
  return topLibrary->FindBoundType(name);
}

BoundType* ReflectionProject::FindBoundType(const TypeId& id, bool recursive)
{
  if(mLibraries.empty())
    return nullptr;

  ReflectionLibrary* topLibrary = mLibraries.back();
  return topLibrary->FindBoundType(id);
}
