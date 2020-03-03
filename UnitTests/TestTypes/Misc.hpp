#pragma once

#include "Bindings.hpp"

struct NameId
{
  NameId()
  {
  }
  NameId(int id, const std::string& name) : mId(id), mName(name)
  {
  }

  bool operator==(const NameId& rhs) const
  {
    return mId == rhs.mId && mName == rhs.mName;
  }
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, NameId, mId);
    BindProperty(library, boundType, NameId, mName);
  }

  int mId;
  std::string mName;
};

struct NameIdList
{
  bool operator==(const NameIdList& rhs) const
  {
    return mIds == rhs.mIds;
  }
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, NameIdList, mIds);
  }

  std::vector<NameId> mIds;
};

struct NumberList
{
  bool operator==(const NumberList& rhs) const
  {
    return mData == rhs.mData;
  }
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, NumberList, mData);
  }

  std::vector<int> mData;
};

struct Dictionary
{
  bool operator==(const Dictionary& rhs) const
  {
    return mData == rhs.mData;
  }
  static void Bind(ReflectionLibrary& library, BoundType& boundType)
  {
    BindProperty(library, boundType, Dictionary, mData);
  }
  
  void Add(const std::string& key, const std::string& value)
  {
    mData.insert(std::make_pair(key, value));
  }

  std::unordered_map<std::string, std::string> mData;
};

struct MyStruct
{
  int mData;
  float mFloat;
  char mChar1;
  int value = 5;
  char mChar2;
  double mDouble1;
  std::string mString1;

  bool operator==(const MyStruct& rhs) const
  {
    return mData == rhs.mData &&
      mFloat == rhs.mFloat &&
      mChar1 == rhs.mChar1 &&
      value == rhs.value &&
      mChar2 == rhs.mChar2 &&
      mDouble1 == rhs.mDouble1 &&
      mString1 == rhs.mString1;
  }
};

inline void BindMyStruct(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
{
  BindProperty(library, boundType, MyStruct, mData);
  BindProperty(library, boundType, MyStruct, mFloat);
  BindProperty(library, boundType, MyStruct, mChar1);
  BindProperty(library, boundType, MyStruct, mChar2);
  BindProperty(library, boundType, MyStruct, mDouble1);
  BindProperty(library, boundType, MyStruct, mString1);
}