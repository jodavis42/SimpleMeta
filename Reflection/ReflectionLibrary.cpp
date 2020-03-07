#include "Precompiled.hpp"

#include "ReflectionLibrary.hpp"
#include "Asserts.hpp"
#include "StaticTypeId.hpp"

namespace SimpleReflection
{

ReflectionLibrary::~ReflectionLibrary()
{
  for(BoundType* boundType : mBoundTypes)
  {
    boundType->Destruct();
    if(!boundType->mNative)
      delete boundType;
  }
}

void ReflectionLibrary::AddBoundType(BoundType* boundType)
{
  if(mIsFinalized)
  {
    ReflectionErrorIf(mIsFinalized, "Can't add to a finalized library");
    return;
  }

  // Already queued to register, skip
  if(mBoundTypesToRegister.find(boundType) != mBoundTypesToRegister.end())
    return;

  // Check if a type was already registered in a dependancy, if so don't register it again
  if(boundType->mId.mId != TypeId::sInvalidId && FindBoundType(boundType->mId) != nullptr)
    return;

  if(boundType->mIsRegistered)
  {
    ReflectionErrorIf(boundType->mIsRegistered, "BoundType '%s' is already registered but is not in a dependent library", boundType->mName.c_str());
    return;
  }

  mBoundTypesToRegister.insert(boundType);
}

void ReflectionLibrary::Finalize()
{
  // Don't double finalize
  if(mIsFinalized)
    return;

  mIsFinalized = true;

  // This id isn't guaranteed to be safe, come up with a better solution...
  static size_t sId = 9999;

  for(BoundType* boundType : mBoundTypesToRegister)
  {
    // Type was dynamic (like a vector) and didn't have an explicit id. Auto-generate it.
    if(boundType->mId.mId == TypeId::sInvalidId)
      boundType->mId.mId = ++sId;

    boundType->mIsRegistered = true;
    mBoundTypes.push_back(boundType);
    mBoundTypeNameMap[boundType->mName] = boundType;
    mBoundTypeIdMap[boundType->mId.mId] = boundType;
  }

  Validate();
  mBoundTypesToRegister.clear();
}

bool ReflectionLibrary::Validate()
{
  for(BoundType* boundType : mBoundTypes)
  {
    ReflectionErrorIf(boundType->mId.mId == TypeId::sInvalidId, "Invalid id");
    // All types need a size (except void)
    ReflectionErrorIf(boundType->mSizeInBytes == 0 && boundType->mName != "void", "Invalid size");
  }
  return true;
}

BoundType* ReflectionLibrary::FindBoundType(const std::string& name, bool recursive)
{
  auto it = mBoundTypeNameMap.find(name);
  if(it != mBoundTypeNameMap.end())
  {
    ReflectionErrorIf(it->second->mId.mId == TypeId::sInvalidId, "Fetching an uninitialized bound type");
    return it->second;
  }

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
  {
    ReflectionErrorIf(it->second->mId.mId == TypeId::sInvalidId, "Fetching an uninitialized bound type");
    return it->second;
  }

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
  ReflectionErrorIf(mShutdown == false, "ReflectionProject must be shutdown before destructing");
}

void ReflectionProject::Shutdown()
{
  // Make sure to destruct in reverse order
  for(size_t i = 0; i < mLibraries.size(); ++i)
  {
    ReflectionLibrary* library = mLibraries[mLibraries.size() - i - 1];
    delete library;
  }
  mLibraries.clear();
  mShutdown = true;
}

ReflectionProject* ReflectionProject::GetInstance()
{
  static ReflectionProject sInstance;
  return &sInstance;
}

ReflectionLibrary& ReflectionProject::CreateLibrary(const std::string& name)
{
  ReflectionProject* instance = GetInstance();
  // If we were already building a library then finalize it so it's locked.
  // This helps prevent errors of creating new types later which are missing important info (names, etc...)
  if(instance->mCurrentLibrary != nullptr)
  {
    instance->mCurrentLibrary->Finalize();
  }

  ReflectionLibrary* library = new ReflectionLibrary();
  ReflectionErrorIf(instance->mCurrentLibrary != nullptr && !instance->mCurrentLibrary->mIsFinalized, "Cannot create a new library before the previous one was finalized");
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

}//namespace SimpleReflection
