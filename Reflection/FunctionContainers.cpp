#include "Precompiled.hpp"

#include "FunctionContainers.hpp"
#include "Function.hpp"

namespace SimpleReflection
{

struct Function;
struct BoundType;

Function* FunctionList::FindFunction(const FunctionType& functionType) const
{
  for(Function* function : *this)
  {
    if(function->mFunctionType.IsA(functionType))
      return function;
  }
  return nullptr;
}

void FunctionList::DeleteAll()
{
  for(Function* function : *this)
    delete function;
  clear();
}

Function* FunctionMultiMap::FindFunction(const std::string& fnName, const FunctionType& functionType) const
{
  auto it = find(fnName);
  if(it == end())
    return nullptr;

  const FunctionList& fnList = it->second;
  return fnList.FindFunction(functionType);
}

void FunctionMultiMap::DeleteAll()
{
  for(auto&& fnPair : *this)
    fnPair.second.DeleteAll();
  clear();
}

Function* ExtensionFunctionMultiMap::FindFunction(const BoundType* boundType, const std::string& fnName, const FunctionType& functionType) const
{
  auto it = find(boundType);
  if(it == end())
    return nullptr;
  
  const FunctionMultiMap& fnMultiMap = it->second;
  return fnMultiMap.FindFunction(fnName, functionType);
  return nullptr;
}

void ExtensionFunctionMultiMap::DeleteAll()
{
  for(auto&& boundTypePair : *this)
    boundTypePair.second.DeleteAll();
  clear();
}

}//namespace SimpleReflection
