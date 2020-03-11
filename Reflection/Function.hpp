#pragma once

#include "StaticTypeId.hpp"

#include <vector>

namespace SimpleReflection
{

struct BoundType;
struct Call;

typedef void(*BoundFunction)(Call& call);

/// Runtime type information about a function. Contains necessary data to check types and lookup unique functions.
struct FunctionType
{
  size_t ComputeSizeRequired() const;
  void ResizeParamCount(size_t count);

  BoundType* GetReturnType() const;
  void SetReturnType(BoundType* boundType);

  BoundType* GetThisType() const;
  void SetThisType(BoundType* boundType);

  BoundType* GetParamType(size_t index) const;
  void SetParamType(size_t index, BoundType* boundType);

  bool IsStatic() const;

  bool operator==(const FunctionType& rhs) const;

private:
  BoundType* mReturnType = nullptr;
  BoundType* mThisType = nullptr;
  std::vector<BoundType*> mParams;
};

/// Runtime reflection information about a bound function. Used to actually invoke functions at runtime
struct Function
{
  size_t ComputeSizeRequired() const;

  BoundType* GetReturnType() const;
  void SetReturnType(BoundType* boundType);

  BoundType* GetThisType() const;
  void SetThisType(BoundType* boundType);

  BoundType* GetParamType(size_t index) const;
  void SetParamType(size_t index, BoundType* boundType);

  bool IsStatic() const;

  BoundFunction mBoundFunction = nullptr;
  FunctionType mFunctionType;
};

}//namespace SimpleReflection
