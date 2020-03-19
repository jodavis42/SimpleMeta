#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace SimpleReflection
{

struct Function;
struct FunctionType;
struct BoundType;

struct FunctionList : public std::vector<Function*>
{
  Function* FindFunction(const FunctionType& functionType) const;

  void DeleteAll();
};

struct FunctionMultiMap : public std::unordered_map<std::string, FunctionList>
{
  Function* FindFunction(const std::string& fnName, const FunctionType& functionType) const;

  void DeleteAll();
};

struct ExtensionFunctionMultiMap : public std::unordered_map<const BoundType*, FunctionMultiMap>
{
  Function* FindFunction(const BoundType* boundType, const std::string& fnName, const FunctionType& functionType) const;

  void DeleteAll();
};

}//namespace SimpleReflection
