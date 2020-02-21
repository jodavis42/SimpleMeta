#include "MetaLibrary.hpp"

ReflectionLibrary::~ReflectionLibrary()
{
  for(BoundType* boundType : mBoundTypes)
  {
    delete boundType;
  }
}

void ReflectionLibrary::AddBoundType(BoundType* boundType)
{
  // Don't double register
  if(FindBoundType(boundType->mId) != nullptr)
    return;

  mBoundTypes.push_back(boundType);
  mBoundTypeNameMap[boundType->mName] = boundType;
  mBoundTypeIdMap[boundType->mId.mId] = boundType;
}

BoundType* ReflectionLibrary::FindBoundType(const std::string& name, bool recursive)
{
  auto it = mBoundTypeNameMap.find(name);
  if(it != mBoundTypeNameMap.end())
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

ReflectionProject::~ReflectionProject()
{
  // Make sure to destruct in reverse order
  for(size_t i = 0; i < mLibraries.size(); ++i)
  {
    ReflectionLibrary* library = mLibraries[mLibraries.size() - i - 1];
    delete library;
  }
  mLibraries.clear();
}

ReflectionProject* ReflectionProject::GetInstance()
{
  static ReflectionProject sInstance;
  return &sInstance;
}

ReflectionLibrary& ReflectionProject::CreateLibrary(const std::string& name)
{
  ReflectionProject* instance = GetInstance();
  ReflectionLibrary* library = new ReflectionLibrary();
  instance->mCurrentLibrary = library;
  library->mName = name;
  instance->mLibraries.push_back(library);
  return *library;
}

ReflectionLibrary* ReflectionProject::FindLibrary(const std::string& name)
{
  ReflectionProject* instance = GetInstance();
  for(ReflectionLibrary* library : instance->mLibraries)
  {
    if(library->mName == name)
      return library;
  }
  return nullptr;
}

void ReflectionProject::DestroyLibrary(ReflectionLibrary& library)
{
  ReflectionProject* instance = GetInstance();
  for(auto it = instance->mLibraries.begin(); it != instance->mLibraries.end(); ++it)
  {
    if(*it == &library)
    {
      delete *it;
      instance->mLibraries.erase(it);
      break;
    }
  }

  instance->mCurrentLibrary = nullptr;
  if(!instance->mLibraries.empty())
    instance->mCurrentLibrary = instance->mLibraries.back();
}

BoundType* ReflectionProject::FindBoundType(const std::string& name, bool recursive)
{
  ReflectionProject* instance = GetInstance();
  if(instance->mLibraries.empty())
    return nullptr;

  ReflectionLibrary* topLibrary = instance->mCurrentLibrary;
  return topLibrary->FindBoundType(name);
}

BoundType* ReflectionProject::FindBoundType(const TypeId& id, bool recursive)
{
  ReflectionProject* instance = GetInstance();
  if(instance->mLibraries.empty())
    return nullptr;

  ReflectionLibrary* topLibrary = instance->mCurrentLibrary;
  return topLibrary->FindBoundType(id);
}
