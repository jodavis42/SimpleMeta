#pragma once

#include <string>
#include <vector>

#include "TypeId.hpp"
#include "ReflectionComposition.hpp"
#include "Any.hpp"

struct BoundType;
struct MetaSerialization;

struct Field : public ReflectionComposition
{
  char* GetFieldData(char* instanceData) const
  {
    return instanceData + mOffset;
  }

  std::string mName;
  size_t mOffset = 0;
  BoundType* mType = nullptr;
};

struct GetterSetter : public ReflectionComposition
{
  std::string mName;
  BoundType* mType = nullptr;
  virtual void ReadData(char* instanceData, char* outDataStream) const {};
  virtual void WriteData(char* instanceData, char* inDataStream) const {};

  virtual Any Get(char* instanceData) const { return Any(); }
  virtual void Set(char* instanceData, Any any) {}
};

template <typename ClassType, typename GetterType, typename GetType, typename SetterType, typename SetType>
struct GetterSetterTyped : public GetterSetter
{
  GetterType mGetter = nullptr;
  SetterType mSetter = nullptr;

  GetType GetInternal(char* instanceData) const
  {
    ClassType* typedInstance = reinterpret_cast<ClassType*>(instanceData);
    auto&& typedData = (typedInstance->*mGetter)();
    return typedData;
  }
  void SetInternal(char* instanceData, SetType setData) const
  {
    ClassType* typedInstance = reinterpret_cast<ClassType*>(instanceData);
    (typedInstance->*mSetter)(setData);
  }

  virtual Any Get(char* instanceData) const override
  {
    ClassType* typedInstance = reinterpret_cast<ClassType*>(instanceData);
    auto&& typedData = (typedInstance->*mGetter)();
    Any result(&typedData, StaticTypeId<GetType>::GetBoundType());
    return result;
  }

  virtual void Set(char* instanceData, Any any) override
  {
    ClassType* typedInstance = reinterpret_cast<ClassType*>(instanceData);
    (typedInstance->*mSetter)(any.Get<SetType>());
  }
};

struct BoundType : public ReflectionComposition
{
  std::string mName;
  size_t mSizeInBytes = 0;
  std::vector<Field*> mFields;
  std::vector<GetterSetter*> mGetterSetters;
  BoundType* mBaseType = nullptr;
  TypeId mId;
};
